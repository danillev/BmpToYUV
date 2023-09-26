#ifndef BMPTOYUVCONVERTER_H
#define BMPTOYUVCONVERTER_H

#include "BMPFileReader.h"
#include "RGBFrame.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <intrin.h>
#include <mutex>

class BMPToYUVConverter {
public:
    static yuvData convertToYUV(std::shared_ptr<RGBFrame> frame);
    
private:
    static std::vector<unsigned char> convertToY(std::shared_ptr<RGBFrame> frameImage);
    static std::vector<unsigned char> convertToU(std::shared_ptr<RGBFrame> frameImage);
    static std::vector<unsigned char> convertToV(std::shared_ptr<RGBFrame> frameImage);
    //static void subsampleUV(int i, int j, int imgWidth, const RGBPixel& rgb, yuvData& yuvData);
    static unsigned char RGBtoY (const RGBPixel& rgb);
    static unsigned char RGBtoU(const RGBPixel& rgb);
    static unsigned char RGBtoV(const RGBPixel& rgb);
};
#endif //BMPTOYUVCONVERTER_H