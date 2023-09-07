#ifndef BMPTOYUVCONVERTER_H
#define BMPTOYUVCONVERTER_H

#include "BMPImage.h"
#include <fstream>
#include <iostream>

class BMPToYUVConverter {
public:
    static bool convertToYUV(const BMPImage& bmpImage, std::vector<unsigned char>& yData, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData);

private:
    static void RGBtoYUV(const RGBPixel& rgb, unsigned char& Y, unsigned char& U, unsigned char& V);
};
#endif //BMPTOYUVCONVERTER_H