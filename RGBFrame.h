#ifndef RGBFRAME_H
#define RGBFRAME_H

#include <iostream>
#include <vector>
#include "Structures.h"

class RGBFrame {
public:
    std::vector<RGBPixel> frameData;
    RGBFrame() = default;
    RGBFrame(int width, int height);
    RGBPixel getPixel(int x, int y) const;
    int getWidth() const;
    int getHeight() const;


private:
    int _width;
    int _height;
};
#endif //RGBFRAME_H