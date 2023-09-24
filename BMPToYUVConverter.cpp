#include "BMPToYUVConverter.h"

unsigned char BMPToYUVConverter::RGBtoY(const RGBPixel& rgbPixel) {
    return static_cast<unsigned char>((rgbPixel.R * 299 + rgbPixel.G * 587 + rgbPixel.B * 114) / 1000);
}
unsigned char BMPToYUVConverter::RGBtoU(const RGBPixel& rgb) {
    return static_cast<unsigned char>((rgb.B - BMPToYUVConverter::RGBtoY(rgb)) * 564 / 1000 + 128);
}
unsigned char BMPToYUVConverter::RGBtoV(const RGBPixel& rgb) {
    return static_cast<unsigned char>((rgb.R - BMPToYUVConverter::RGBtoY(rgb)) * 713 / 1000 + 128);
}


void BMPToYUVConverter::subsampleUV(int indexY, int indexX, int imgWidth, const RGBPixel& rgbPixel, yuvData& yuvData) {
    if (indexY % 2 == 0 && indexX % 2 == 0) {
        int uvIndex = (indexY / 2) * (imgWidth / 2) + (indexX / 2);
        yuvData.uData[uvIndex] = BMPToYUVConverter::RGBtoU(rgbPixel);
        yuvData.vData[uvIndex] = BMPToYUVConverter::RGBtoV(rgbPixel);
    }
}

yuvData BMPToYUVConverter::convertToYUV(std::shared_ptr<RGBFrame> frame) {
    yuvData yuvDataPtr;
    int imgHeight = frame->getHeight();
    int imgWidth = frame->getWidth();
    yuvDataPtr.yData.resize(imgWidth * imgHeight);
    yuvDataPtr.uData.resize(imgWidth * imgHeight / 4);
    yuvDataPtr.vData.resize(imgWidth * imgHeight / 4);

    const int numThreads =  std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);  
    
    for (int threadId = 0; threadId < numThreads; ++threadId) {
        threads.emplace_back([&, threadId]() {
            for (int i = threadId; i < imgHeight; i += numThreads) {
                for (int j = 0; j < imgWidth; ++j) {
                    RGBPixel rgb = frame->getPixel(j, i);
                    int yIndex = i * imgWidth + j;
                    yuvDataPtr.yData[yIndex] = BMPToYUVConverter::RGBtoY(rgb);
                    subsampleUV(i, j, imgWidth, rgb, yuvDataPtr);
                }
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    return yuvDataPtr;
}