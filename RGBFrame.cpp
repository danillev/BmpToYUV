#include "RGBFrame.h"
#include "Structures.h"

RGBFrame::RGBFrame(int width, int height): _width(width), _height(height)
{
	dataSize = _width * _height * sizeof(RGBPixel);
    frameData = std::make_shared<std::vector<RGBPixel>>();
	this->frameResize(dataSize);
}

RGBPixel RGBFrame::getPixel(int x, int y) const
{
    int index = y * _width + x;
    return (*frameData)[index];
}

int RGBFrame::getWidth() const
{
    return _width;
}

int RGBFrame::getHeight() const
{
    return _height;
}

unsigned int RGBFrame::getDataSize()
{
    return dataSize;
}

void RGBFrame::frameResize(const size_t newSize)
{
    frameData->resize(newSize);
}


std::istream& operator>>(std::istream& stream, RGBFrame& rgbFrame)
{
    return stream.read(reinterpret_cast<char*>(rgbFrame.frameData->data()), rgbFrame.getDataSize());
}
