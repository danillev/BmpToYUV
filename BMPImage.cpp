#include "BMPImage.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

BMPImage::BMPImage(const std::string& filePath) : filePath(filePath) {}

bool BMPImage::readHeader() {
    ifstream bmpFile(filePath, ios::binary);
    if (!bmpFile.is_open()) {
        cerr << "Failed to open BMP image file" << endl;
        return false;
    }

    bmpFile.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    if (header.signature[0] != 'B' || header.signature[1] != 'M') {
        cerr << "The file is not a BMP image" << endl;
        return false;
    }

    if (header.compression != 0 || header.bitsPerPixel != 24) {
        cerr << "The image does not meet the requirements (24 bit/pixel)" << endl;
        return false;
    }

    imgWidth = header.width;
    imgHeight = header.height;

    bmpFile.close();
    return true;
}

std::string BMPImage::GetFilePath() const {
    return filePath;
}

int BMPImage::getWidth() const {
    return imgWidth;
}

int BMPImage::getHeight() const {
    return imgHeight;
}
