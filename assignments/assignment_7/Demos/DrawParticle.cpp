#include "DrawParticle.h"
#include "error.h"

namespace {
    static DrawFunction theDrawFunction;
}

void drawParticle(double x, double y, Color color) {
    if (!theDrawFunction) {
        error("drawParticle() was called in a context where it wasn't expected to be called.");
    }
    theDrawFunction(x, y, color);
}

void setDrawFunction(DrawFunction fn) {
    theDrawFunction = fn;
}
