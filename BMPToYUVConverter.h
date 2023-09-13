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
    //static bool subsampling(std::vector<unsigned char>& uData, std::vector<unsigned char>& vData, int i, int j)
private:
   // static void RGBtoY(const int threadId, const int numThreads, const int imgHeight, const int imgWidth);
    static void RGBtoYUV (const RGBPixel& rgb, unsigned char& Y, unsigned char& U, unsigned char& V);
};
#endif //BMPTOYUVCONVERTER_H