#ifndef BMPTOYUVCONVERTER_H
#define BMPTOYUVCONVERTER_H

#include "BMPImage.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <intrin.h>

class BMPToYUVConverter {
public:
    static bool convertToYUV(const BMPImage& bmpImage, std::vector<unsigned char>& yData, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData);
private:
    static void subsampleUV(int i, int j, int imgWidth, unsigned char& U, unsigned char& V, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData);
    static void RGBtoYUV (const RGBPixel& rgb, unsigned char& Y, unsigned char& U, unsigned char& V);
};
#endif //BMPTOYUVCONVERTER_H