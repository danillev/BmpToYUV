#ifndef BMPTOYUVCONVERTER_H
#define BMPTOYUVCONVERTER_H

#include "BMPImage.h"
#include "RGBFrame.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <intrin.h>
#include <mutex>

class BMPToYUVConverter {
public:
    static bool convertToYUV(const RGBFrame& frame, std::vector<unsigned char>& yData, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData);
    
private:
    static void subsampleUV(BMPToYUVConverter& converter, int i, int j, int imgWidth, const RGBPixel& rgb, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData);
    static unsigned char RGBtoY (const RGBPixel& rgb);
    static unsigned char RGBtoU(const RGBPixel& rgb);
    static unsigned char RGBtoV(const RGBPixel& rgb);
    std::mutex yDataMutex;
    std::mutex uDataMutex;
    std::mutex vDataMutex;
};
#endif //BMPTOYUVCONVERTER_H