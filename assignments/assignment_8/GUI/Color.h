#pragma once

#include <ostream>

namespace MiniGUI {
    class Color {
    public:
        /* Defaults to black. */
        Color() = default;

        /* You can create a color by specifying its red, green, and blue color
         * components. Each value runs from 0 (none of this color) to 255
         * (as much of this color as possible.)
         */
        Color(int red, int green, int blue);

        /* How much red, green, and blue are used. */
        int red() const;
        int green() const;
        int blue() const;

        /* A composed color value represented as a 24-bit integer used by the
         * graphics system. If you know what 0xRRGGBB means, great! That's what
         * this does. If not, no worries! You aren't expected to know or use
         * this function.
         */
        int toRGB() const;

        /* Returns an HTML-friendly string representation of the color. */
        std::string toHTML() const;

        /* Helpful preset colors. You can access them as Color::WHITE(),
         * Color::BLACK(), etc.
         */
        static Color WHITE();
        static Color BLACK();
        static Color RED();
        static Color GREEN();
        static Color BLUE();
        static Color YELLOW();
        static Color CYAN();
        static Color MAGENTA();
        static Color GRAY();

        /* A randomly-chosen color. Call this function as Color::RANDOM(). */
        static Color RANDOM();

        /* Creates a color given its hue, saturation, and value, each of which
         * ranges from 0 to 1, inclusive.
         */
        static Color fromHSV(double hue, double saturation, double value);
        static Color fromHex(int hexValue);

    private:
        int color = 0;
    };

    /* Utility functions to print / compare colors. */
    std::ostream& operator<< (std::ostream& out, const Color& color);
    bool operator== (const Color& lhs, const Color& rhs);
    bool operator!= (const Color& lhs, const Color& rhs);
    bool operator<  (const Color& lhs, const Color& rhs);
}
