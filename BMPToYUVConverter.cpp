#include "BMPToYUVConverter.h"


void BMPToYUVConverter::RGBtoYUV(const RGBPixel& rgb, unsigned char& Y, unsigned char& U, unsigned char& V) {
    Y = static_cast<unsigned char>((rgb.R * 299 + rgb.G * 587 + rgb.B * 114) / 1000);
    U = static_cast<unsigned char>((rgb.B - Y) * 564 / 1000 + 128);
    V = static_cast<unsigned char>((rgb.R - Y) * 713 / 1000 + 128);

}


void BMPToYUVConverter::subsampleUV(int i, int j, int imgWidth, unsigned char& U, unsigned char& V, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData) {
    if (i % 2 == 0 && j % 2 == 0) {
        int uvIndex = (i / 2) * (imgWidth / 2) + (j / 2);
        uData[uvIndex] = U;
        vData[uvIndex] = V;
    }
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

    
    const int numThreads = std::thread::hardware_concurrency();
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

                    subsampleUV(i, j, imgWidth, U, V, uData, vData);

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