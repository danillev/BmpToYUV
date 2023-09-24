#include "BMPFileReader.h"
#include "YUVVideo.h"
#include "BMPToYUVConverter.h"
#include <iostream>
#include <locale>
#include <RGBFrame.h>


int main() {
    auto startTime = std::chrono::high_resolution_clock::now();

    

    setlocale(LC_ALL, "Russian");
    BMPFileReader bmpImage("231000002.bmp");
    if (!bmpImage.readHeader()) {
        return 1;
    }

    std::shared_ptr<RGBFrame> frame = std::make_shared<RGBFrame>(RGBFrame(bmpImage.getWidth(), bmpImage.getHeight()));
    if (!bmpImage.readImage()) {
        return 1;
    }
    frame = bmpImage.getFrame();

    std::shared_ptr<yuvData> yuvDataPtr = std::make_shared<yuvData>(BMPToYUVConverter::convertToYUV(frame));
    
    YUVVideo yuvVideo("bus_cif.yuv");
    if (!yuvVideo.open()) {
        return 1;
    }

    yuvVideo.insertBMP(bmpImage, yuvDataPtr);

    yuvVideo.close();
    std::cout << "Inserting BMP into YUV is complete." << std::endl;
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "The program is completed in " << duration.count() << " milliseconds." << std::endl;

    return 0;
}
