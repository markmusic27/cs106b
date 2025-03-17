#include "GUI/MiniGUI.h"
#include "GUI/Color.h"
#include "Treemap.h"
#include "Demos/DataSource.h"
#include "priorityqueue.h"
#include "ginteractors.h"
#include "gthread.h"
using namespace std;
using namespace MiniGUI;

namespace {
    const Color kBorderColor(0, 0, 0);
    const Color kNullColor = Color::fromHex(0xDDDDDD);

    const Font kHovertextFont(FontFamily::SANS_SERIF, FontStyle::NORMAL, 18, Color::BLACK());
    const auto kHovertextBackgroundColor = Color::fromHex(0xEEEEEE);

    const int kMinLabelWidth    = 50;
    const int kMinLabelHeight   = 15;
    const int kLabelPadding     = 3;
    const int kMinLabelFontSize = 10;

    Vector<Color> standardPaletteFor(int n) {
        if (n < 0) {
            error("standardPaletteFor(): Asking for negative number of colors.");
        }

        /* By looking at angles that are multiples of phi, we ensure good dispersion over
         * the color space.
         */
        double phi = (1 + sqrt(5.0)) / 2.0;

        Vector<Color> result;
        for (int i = 0; i < n; i++) {
            double hue        = fmod(phi * i, 1);
            result += Color::fromHSV(hue, 0.5, 1);
        }

        return result;
    }

    const Vector<Font> kFonts = {
        Font(), // Top-level never displayed
        Font(FontFamily::SANS_SERIF, FontStyle::BOLD, 18,   Color::BLACK()),
        Font(FontFamily::SANS_SERIF, FontStyle::NORMAL, 12, Color::BLACK()),

        // Nothing below this is displayed
    };

    class GUI: public ProblemHandler {
    public:
        GUI(GWindow& window);
        ~GUI();

        void repaint() override;
        void mouseMoved(double x, double y) override;
        void mouseDragged(double x, double y) override;
        void mouseExited() override;

        void windowResized() override;
        void actionPerformed(GObservable* source) override;

    private:
        Node* tree = nullptr;
        Map<Node*, int> levels;
        Map<Node*, Rectangle> treemap;

        /* In case something goes wrong. */
        string errorMsg;

        /* List of text renders for labels. Cached because computing these are expensive. */
        Vector<shared_ptr<TextRender>> labels;

        shared_ptr<DataSourceImpl> source;

        Temporary<GComboBox> demoSelector;
        Temporary<GButton> goButton;

        /* Last mouse position. */
        int mouseX = -1, mouseY = -1;

        void drawTreemap();
        void drawHovertext();
        string hovertext();
        Rectangle treemapBounds();
        void recomputeLabels();
        void setDemo(const string& file);
    };

    GUI::GUI(GWindow& window): ProblemHandler(window) {
        demoSelector = make_temporary<GComboBox>(window, "SOUTH");
        demoSelector->setEditable(false);
        for (const auto& entry: DataSourceRegistry::sources()) {
            demoSelector->addItem(entry.first);
        }
        goButton = make_temporary<GButton>(window, "SOUTH", "Load Data");

        setDemo(DataSourceRegistry::sources().begin()->first);
    }

    void GUI::setDemo(const string& demoName) {
        auto itr = DataSourceRegistry::sources().find(demoName);
        if (itr == DataSourceRegistry::sources().end()) {
            error("Internal error: No demo named '" + demoName + "' exists.");
        }

        /* Construct the data source. */
        source = itr->second();

        /* Clean up old resources. */
        freeTree(tree);
        labels.clear();
        errorMsg = "";

        /* Load the treemap. If we get an error, stash it for later
         * so we can display it.
         */
        try {
            /* Do this first in case the load fails. */
            tree = nullptr;
            tree = source->data();
        } catch (const exception& e) {
            errorMsg = to_string("Error loading data: ") + e.what();
        } catch (...) {
            errorMsg = "An unknown error occurred loading the data.";
        }

        /* If the tree is nullptr, it means that there's nothing to display. */
        if (tree != nullptr) {
            /* Compute data. */
            levels = depthsOf(tree);
            treemap = formTreemapOf(tree, treemapBounds());

            /* Lay out the labels for the display. */
            recomputeLabels();
        }
    }

    void GUI::recomputeLabels() {
        /* Nothing to do on a null tree. */
        if (tree == nullptr) return;

        /* Form text labels. Run this on the GUI thread to avoid Qt warning
         * messages about timers.
         */
        GThread::runOnQtGuiThread([&, this] {
            labels.clear();
            for (auto* node: treemap) {
                /* Don't try fitting into boxes that are too small. */
                auto bounds = treemap[node];
                if (bounds.width - 2 * kLabelPadding < kMinLabelWidth || bounds.height - 2 * kLabelPadding < kMinLabelHeight) continue;

                /* Only care about labels one or two levels deep. */
                if (levels[node] == 1 || levels[node] == 2) {
                    /* Level-1 labels can only use the top half of the box. */
                    if (levels[node] == 1) {
                        bounds.height /= 2;
                    }
                    /* Level-2 labels use the bottom half. */
                    else {
                        bounds.height /= 2;
                        bounds.y += bounds.height;
                    }

                    /* Shrink the box in by a small amount in each direction to leave some buffer. */
                    bounds.x      += kLabelPadding;
                    bounds.y      += kLabelPadding;
                    bounds.width  -= 2 * kLabelPadding;
                    bounds.height -= 2 * kLabelPadding;

                    auto render = TextRender::construct(node->name, GRectangle(bounds.x, bounds.y, bounds.width, bounds.height), kFonts[levels[node]]);

                    /* Make sure the font is legible. */
                    if (render->computedFont().size() < kMinLabelFontSize) continue;

                    /* Level-1 labels drawn at top of box if they fit. */
                    if (levels[node] == 1) {
                        render->alignTop();
                    }
                    /* Everything else goes at the bottom of the box. */
                    else {
                        render->alignBottom();
                    }

                    labels += render;
                }
            }
        });
    }

    void GUI::actionPerformed(GObservable* source) {
        if (source == goButton) {
            setDemo(demoSelector->getSelectedItem());
            requestRepaint();
        }
    }

    GUI::~GUI() {
        freeTree(tree);
    }

    /* Track the mouse location so we can draw the infobox. */
    void GUI::mouseMoved(double x, double y) {
        mouseX = x;
        mouseY = y;
        requestRepaint();
    }

    /* Drag == Move, since we ignore presses. */
    void GUI::mouseDragged(double x, double y) {
        mouseMoved(x, y);
    }

    /* Mouse leaves? Set the cursor position to (-1, -1) as a sentinel. */
    void GUI::mouseExited() {
        mouseX = mouseY = -1;
        requestRepaint();
    }

    /* Window resize requires us to redraw the treemap. */
    void GUI::windowResized() {
        if (tree != nullptr) {
            /* Redo the treemap for the new size. */
            treemap = formTreemapOf(tree, treemapBounds());

            /* Pretend the mouse isn't here so that we can see the result. */
            mouseX = mouseY = -1;

            /* Change where the display labels are. */
            recomputeLabels();
        }

        requestRepaint();
    }

    void GUI::repaint() {
        window().clearCanvas();

        drawTreemap();
        drawHovertext();
    }

    /* Space allocated for the treemap. */
    Rectangle GUI::treemapBounds() {
        return {
            0, 0, int(window().getCanvasWidth()), int(window().getCanvasHeight())
        };
    }

    void GUI::drawTreemap() {
        /* If there's no tree, just display a blank screen. */
        if (tree == nullptr) {
            clearDisplay(window(), kNullColor);

            auto text = TextRender::construct(errorMsg, canvasBounds(), kHovertextFont);
            text->alignCenterVertically();
            text->alignCenterHorizontally();
            text->draw(window());
            return;
        }

        /* Draw backing rectangles for each top-level division. */
        auto palette = standardPaletteFor(tree->children.size());
        for (int i = 0; i < tree->children.size(); i++) {
            auto bounds = treemap[tree->children[i]];
            window().setColor(palette[i].toRGB());
            window().fillRect(bounds.x, bounds.y, bounds.width, bounds.height);
        }

        /* Now draw the outlines of every rectangle. */
        window().setColor(kBorderColor.toRGB());
        for (auto* node: treemap) {
            auto bounds = treemap[node];
            window().drawRect(bounds.x, bounds.y, bounds.width, bounds.height);
        }

        /* And, finally, the labels. */
        for (const auto& label: labels) {
            label->draw(window());
        }
    }

    void GUI::drawHovertext() {
        /* Don't do anything if the mouse isn't here or if there's no tree. */
        if (tree == nullptr || mouseX == -1 || mouseY == -1) return;

        /* Make sure we have some text to disply. If not, do nothing. */
        auto text = hovertext();
        if (text == "") return;

        /* Figure out where to draw this. */
        GRectangle bounds = { 0, 0, window().getCanvasWidth() / 2, window().getCanvasHeight() / 4};
        if (mouseX < window().getCanvasWidth() / 2) {
            bounds.x = window().getCanvasWidth() - bounds.width;
        }
        if (mouseY < window().getCanvasHeight() / 2) {
            bounds.y = window().getCanvasHeight() - bounds.height;
        }

        /* Draw backing rectangle. */
        window().setColor(kHovertextBackgroundColor.toRGB());
        window().fillRect(bounds);

        /* Form the text render. */
        auto render = TextRender::construct(text, bounds, kHovertextFont);
        render->draw(window());
    }

    /* Checks if a point is in a rectangle. The top and left boundaries of
     * the rectangle are inclusive; the right and bottom boundaries are
     * exclusive.
     */
    bool pointInRectangle(int x, int y, const Rectangle& bounds) {
        return x >= bounds.x &&
               y >= bounds.y &&
               x <  bounds.x + bounds.width &&
               y <  bounds.y + bounds.height;
    }

    /* Forms the string that will be displayed in the hovertext. */
    string GUI::hovertext() {
        ostringstream builder;

        /* Find all nodes we hit. This could be made more efficiently by recursively
         * walking the tree, but we'll leave that as an Exercise to the Reader.
         */
        PriorityQueue<Node*> nodes;
        for (Node* node: treemap) {
            if (pointInRectangle(mouseX, mouseY, treemap[node])) {
                /* Sort nodes by depth in the tree. */
                nodes.enqueue(node, levels[node]);
            }
        }

        /* Assemble the string. */
        while (!nodes.isEmpty()) {
            auto* node = nodes.dequeue();
            builder << format("%s (%s)", node->name, source->weightToString(node->weight)) << endl;
        }

        return builder.str();
    }
}

GRAPHICS_HANDLER("Treemapping", GWindow& window) {
    return make_shared<GUI>(window);
}
