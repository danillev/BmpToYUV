#include "BMPImage.h"
#include "YUVVideo.h"
#include "BMPToYUVConverter.h"
#include <iostream>
#include <locale>


int main() {
    auto startTime = std::chrono::high_resolution_clock::now();


    std::vector<unsigned char> yData;
    std::vector<unsigned char> uData;
    std::vector<unsigned char> vData;


    setlocale(LC_ALL, "Russian");
    BMPImage bmpImage("231000002.bmp");
    if (!bmpImage.readHeader()) {
        return 1;
    }

    if (!BMPToYUVConverter::convertToYUV(bmpImage, yData, uData, vData)) {
        return 1;
    }


    YUVVideo yuvVideo("bus_cif.yuv");
    if (!yuvVideo.open()) {
        return 1;
    }

    yuvVideo.insertBMP(bmpImage, yData, uData, vData);

    yuvVideo.close();
    std::cout << "Вставка BMP в YUV завершена." << std::endl;
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Программа завершена за " << duration.count() << " миллисекунд." << std::endl;

    return 0;
}
