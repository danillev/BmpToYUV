#ifndef RGBFRAME_H
#define RGBFRAME_H

#include <iostream>
#include <vector>
#include "Structures.h"
#include <memory>

class RGBFrame {
public:
    RGBFrame() = default;
    RGBFrame(int width, int height);
    RGBPixel getPixel(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    unsigned int getDataSize();
    void frameResize(const size_t newSize);
    friend std::istream& operator>>(std::istream& stream, RGBFrame& rgbFrame);

private:
    int _width;
    int _height;
    unsigned int dataSize;
    std::shared_ptr<std::vector<RGBPixel>> frameData;
};
#endif //RGBFRAME_H