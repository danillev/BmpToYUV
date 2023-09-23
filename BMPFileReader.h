#ifndef BMP_IMAGE_H
#define BMP_IMAGE_H

#include <string>
#include <vector>
#include "Structures.h"
#include "RGBFrame.h"
#include <memory>

struct BMPHeader;
struct RGBPixel;

class BMPFileReader {
public:
    BMPFileReader(const std::string& filePath);
    bool readHeader();
    bool readImage();
    std::shared_ptr<RGBFrame>& getFrame();
    std::string GetFilePath() const;
    int getWidth() const;
    int getHeight() const;

private:
    std::string filePath;
    BMPHeader header;
    int imgWidth;
    int imgHeight;
    std::shared_ptr<std::ifstream> bmpFileInput;
    std::shared_ptr<RGBFrame> frame;
};

#endif // BMP_IMAGE_H
