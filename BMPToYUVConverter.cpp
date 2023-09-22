#include "BMPToYUVConverter.h"
#include "RGBFrame.h"

unsigned char BMPToYUVConverter::RGBtoY(const RGBPixel& rgb) {
    return static_cast<unsigned char>((rgb.R * 299 + rgb.G * 587 + rgb.B * 114) / 1000);
}
unsigned char BMPToYUVConverter::RGBtoU(const RGBPixel& rgb) {
    return static_cast<unsigned char>((rgb.B - BMPToYUVConverter::RGBtoY(rgb)) * 564 / 1000 + 128);
}
unsigned char BMPToYUVConverter::RGBtoV(const RGBPixel& rgb) {
    return static_cast<unsigned char>((rgb.R - BMPToYUVConverter::RGBtoY(rgb)) * 713 / 1000 + 128);
}


void BMPToYUVConverter::subsampleUV(BMPToYUVConverter& converter, int i, int j, int imgWidth, const RGBPixel& rgb, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData) {
    if (i % 2 == 0 && j % 2 == 0) {
        int uvIndex = (i / 2) * (imgWidth / 2) + (j / 2);
        {
            std::lock_guard<std::mutex> lock(converter.uDataMutex);
            uData[uvIndex] = BMPToYUVConverter::RGBtoU(rgb);
        }
        {
            std::lock_guard<std::mutex> lock(converter.vDataMutex);
            vData[uvIndex] = BMPToYUVConverter::RGBtoV(rgb);
        }
        
    }
}

bool BMPToYUVConverter::convertToYUV(const RGBFrame& frame, std::vector<unsigned char>& yData, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData) {
    BMPToYUVConverter converter;
    int imgHeight = frame.getHeight();
    int imgWidth = frame.getWidth();
    yData.resize(imgWidth * imgHeight);
    uData.resize(imgWidth * imgHeight / 4);
    vData.resize(imgWidth * imgHeight / 4);

    
    const int numThreads =  std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    
    
    for (int threadId = 0; threadId < numThreads; ++threadId) {
        threads.emplace_back([&, threadId]() {
            for (int i = threadId; i < imgHeight; i += numThreads) {
                for (int j = 0; j < imgWidth; ++j) {
                    RGBPixel rgb = frame.getPixel(j, i);

                    int yIndex = i * imgWidth + j;
                    {
                        std::lock_guard<std::mutex> lock(converter.yDataMutex);
                        yData[yIndex] = BMPToYUVConverter::RGBtoY(rgb);
                    }
                    
                    subsampleUV(converter, i, j, imgWidth, rgb, uData, vData);
                   

                }
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    
    int videoWidth = 352;
    int videoHeight = 288;
    int imageHeight = frame.getHeight();
    int imageWidth = frame.getWidth();
    int frameSize = imageWidth * imageHeight * 3 / 2; // YUV420 format

    std::vector<unsigned char> frameBuffer(frameSize);
    std::ofstream outputFile;
    outputFile.open("output_123123.yuv", std::ios::binary);
    // Записываем компоненту Y
    outputFile.write(reinterpret_cast<const char*>(yData.data()), yData.size());

    outputFile.close();

    return true;
}