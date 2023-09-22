#include "RGBFrame.h"
#include "Structures.h"

RGBFrame::RGBFrame(int width, int height): _width(width), _height(height)
{
	unsigned int dataSize = _width * _height * sizeof(RGBPixel);
	frameData.resize(dataSize);
}

RGBPixel RGBFrame::getPixel(int x, int y) const
{
    int index = y * _width + x;
    return frameData[index];   
}

int RGBFrame::getWidth() const
{
    return _width;
}

int RGBFrame::getHeight() const
{
    return _height;
}
