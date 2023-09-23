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
    static bool convertToYUV(std::shared_ptr<RGBFrame> frame, std::shared_ptr<yuvData> yuvDataPtr);
    
private:
    static void subsampleUV(int i, int j, int imgWidth, const RGBPixel& rgb, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData);
    static unsigned char RGBtoY (const RGBPixel& rgb);
    static unsigned char RGBtoU(const RGBPixel& rgb);
    static unsigned char RGBtoV(const RGBPixel& rgb);
};
#endif //BMPTOYUVCONVERTER_H