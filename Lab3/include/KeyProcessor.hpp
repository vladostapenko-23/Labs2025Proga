#pragma once

enum class Mode { NORMAL, CANNY, BLUR, INVERT, GLITCH };

class KeyProcessor {
private:
    Mode currentMode = Mode::NORMAL;
public:
    void process(int key);
    Mode getMode() const;
};
