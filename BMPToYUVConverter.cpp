#include "BMPToYUVConverter.h"


void BMPToYUVConverter::RGBtoYUV(const RGBPixel& rgb, unsigned char& Y, unsigned char& U, unsigned char& V) {
    Y = static_cast<unsigned char>(0.299 * rgb.R + 0.587 * rgb.G + 0.114 * rgb.B);
    U = static_cast<unsigned char>((rgb.B - Y) * 0.565 + 128);
    V = static_cast<unsigned char>((rgb.R - Y) * 0.713 + 128);
}



bool BMPToYUVConverter::convertToYUV(const BMPImage& bmpImage, std::vector<unsigned char>& yData, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData) {
    std::ifstream bmpFile(bmpImage.GetFilePath(), std::ios::binary);
    if (!bmpFile.is_open()) {
        std::cerr << "Failed to open BMP image file" << std::endl;
        return false;
    }

    int imgHeight = bmpImage.getHeight();
    int imgWidth = bmpImage.getWidth();
    yData.resize(imgWidth * imgHeight);
    uData.resize(imgWidth * imgHeight / 4);
    vData.resize(imgWidth * imgHeight / 4);

    
    const int numThreads = 1;// std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    
    int tmpTest = imgHeight / numThreads;
    
    for (int threadId = 0; threadId < numThreads; ++threadId) {
        threads.emplace_back([&, threadId]() {
            for (int i = threadId; i < imgHeight; i += numThreads) {
                for (int j = 0; j < imgWidth; ++j) {
                    RGBPixel rgb;
                    bmpFile.read(reinterpret_cast<char*>(&rgb), sizeof(RGBPixel));

                    unsigned char Y, U, V;
                    RGBtoYUV(rgb, Y, U, V);

                    int yIndex = i * imgWidth + j;
                    yData[yIndex] = Y;

                    // Subsampling U and V for YUV420

                    int uvIndex = (i / 2) * (imgWidth / 2) + (j / 2);
                    uData[uvIndex] = U;
                    vData[uvIndex] = V;

                }
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }


    bmpFile.close();
    return true;
}