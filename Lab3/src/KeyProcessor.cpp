#include "KeyProcessor.hpp"

void KeyProcessor::process(int key) {
    if (key <= 0) return;
    switch(key) {
        case '1': currentMode = Mode::NORMAL; break;
        case '2': currentMode = Mode::CANNY; break;
        case '3': currentMode = Mode::BLUR; break;
        case '4': currentMode = Mode::INVERT; break;
        case '5': currentMode = Mode::GLITCH; break;
    }
}

Mode KeyProcessor::getMode() const {
    return currentMode;
}
