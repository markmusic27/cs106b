#include "Color.h"
#include "error.h"
#include "gcanvas.h"
#include "GUI/TextUtils.h"
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

/* Color constructor. */
MiniGUI::Color::Color(int red, int green, int blue) {
    if (red < 0 || green < 0 || blue < 0 || red >= 256 || green >= 256 || blue >= 256) {
        error("Color values out of range.");
    }
    color = GCanvas::createRgbPixel(red, green, blue);
}

/* From HSV. */
MiniGUI::Color MiniGUI::Color::fromHSV(double h, double s, double v) {
    if (h < 0 || h > 1 || s < 0 || s > 1 || v < 0 || v > 1) {
        error(format("Color::fromHSV(): Values (%s, %s, %s) are out of range.", h, s, v));
    }

    /* From https://en.wikipedia.org/wiki/HSL_and_HSV */
    auto f = [=](int n) {
        double k = fmod(n + h * 6, 6);
        return v - v * s * max(0.0, min({k, 4 - k, 1.0}));
    };

    return Color(255 * f(5), 255 * f(3), 255 * f(1));
}

MiniGUI::Color MiniGUI::Color::fromHex(int hexValue) {
    if (hexValue < 0 || hexValue > 0xFFFFFF) {
        ostringstream builder;
        builder << "Color::fromHex(): Hex value 0x" << hex << hexValue << " out of range.";
        error(builder.str());
    }

    Color result;
    result.color = hexValue;
    return result;
}

int MiniGUI::Color::toRGB() const {
    return color;
}

int MiniGUI::Color::red() const {
    return GCanvas::getRed(color);
}

int MiniGUI::Color::green() const {
    return GCanvas::getGreen(color);
}

int MiniGUI::Color::blue() const {
    return GCanvas::getBlue(color);
}

/* Color presets. */
MiniGUI::Color MiniGUI::Color::WHITE() {
    return MiniGUI::Color(255, 255, 255);
}
MiniGUI::Color MiniGUI::Color::BLACK() {
    return MiniGUI::Color(  0,   0,   0);
}
MiniGUI::Color MiniGUI::Color::RED() {
    return MiniGUI::Color(255,   0,   0);
}
MiniGUI::Color MiniGUI::Color::GREEN() {
    return MiniGUI::Color(  0, 255,   0);
}
MiniGUI::Color MiniGUI::Color::BLUE() {
    return MiniGUI::Color(  0,   0, 255);
}
MiniGUI::Color MiniGUI::Color::YELLOW() {
    return MiniGUI::Color(255, 255,   0);
}
MiniGUI::Color MiniGUI::Color::CYAN() {
    return MiniGUI::Color(  0, 255, 255);
}
MiniGUI::Color MiniGUI::Color::MAGENTA() {
    return MiniGUI::Color(255,   0, 25);
}
MiniGUI::Color MiniGUI::Color::GRAY() {
    return MiniGUI::Color(128, 128, 128);
}

/* Random color. */
MiniGUI::Color MiniGUI::Color::RANDOM() {
    return MiniGUI::Color(randomInteger(0, 255),
                          randomInteger(0, 255),
                          randomInteger(0, 255));
}

/* To HTML string. */
std::string MiniGUI::Color::toHTML() const {
    ostringstream builder;
    builder << setfill('0') << hex << '#'
            << setw(2) << GCanvas::getRed(color)
            << setw(2) << GCanvas::getGreen(color)
            << setw(2) << GCanvas::getBlue(color);
    return builder.str();
}

/* Stream insertion. */
ostream& MiniGUI::operator<< (ostream& out, const MiniGUI::Color& color) {
    /* Is it a named color? */
    if (color == MiniGUI::Color::BLACK()) {
        return out << "Color::BLACK()";
    } else if (color == MiniGUI::Color::BLUE()) {
        return out << "Color::BLUE()";
    } else if (color == MiniGUI::Color::CYAN()) {
        return out << "Color::CYAN()";
    } else if (color == MiniGUI::Color::GRAY()) {
        return out << "Color::GRAY()";
    } else if (color == MiniGUI::Color::GREEN()) {
        return out << "Color::GREEN()";
    } else if (color == MiniGUI::Color::MAGENTA()) {
        return out << "Color::MAGENTA()";
    } else if (color == MiniGUI::Color::RED()) {
        return out << "Color::RED()";
    } else if (color == MiniGUI::Color::WHITE()) {
        return out << "Color::WHITE()";
    } else if (color == MiniGUI::Color::YELLOW()) {
        return out << "Color::YELLOW()";
    }

    return out << color.toHTML();
}

bool MiniGUI::operator== (const MiniGUI::Color& lhs, const MiniGUI::Color& rhs) {
    return lhs.toRGB() == rhs.toRGB();
}
bool MiniGUI::operator!= (const MiniGUI::Color& lhs, const MiniGUI::Color& rhs) {
    return !(lhs == rhs);
}
bool MiniGUI::operator<  (const MiniGUI::Color& lhs, const MiniGUI::Color& rhs) {
    return lhs.toRGB() < rhs.toRGB();
}
