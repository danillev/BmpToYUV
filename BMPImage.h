#ifndef BMP_IMAGE_H
#define BMP_IMAGE_H

#include <string>
#include <vector>
#include "Structures.h"

struct BMPHeader;
struct RGBPixel;

class BMPImage {
public:
    BMPImage(const std::string& filePath);
    bool readHeader();

    std::string GetFilePath() const;
    int getWidth() const;
    int getHeight() const;

private:
    std::string filePath;
    BMPHeader header;
    int imgWidth;
    int imgHeight;
};

#endif // BMP_IMAGE_H
