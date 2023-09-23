#include "BMPFileReader.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <RGBFrame.h>

using namespace std;

BMPFileReader::BMPFileReader(const std::string& filePath) : filePath(filePath) {}

bool BMPFileReader::readHeader() {

    ifstream bmpFileInput(filePath, ios::binary);
    if (!bmpFileInput.is_open()) {
        cerr << "Failed to open BMP image file" << endl;
        return false;
    }

    bmpFileInput.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

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



    bmpFileInput.close();
    return true;
}

bool BMPFileReader::readImage() {

    bmpFileInput = std::make_shared<std::ifstream>(std::ifstream(filePath, std::fstream::in | std::fstream::binary));
    frame = std::make_shared<RGBFrame>(RGBFrame(imgWidth, imgHeight));
    if (!bmpFileInput->is_open()) {
        cerr << "Failed to open BMP image file" << endl;
        return false;
    }

    bmpFileInput->seekg(sizeof(BMPHeader), ios::beg);

    int dataSize = imgWidth * imgHeight * 3 ; 
    frame->frameResize(dataSize);
    *bmpFileInput >> *frame;

    bmpFileInput->close();
    return true;
}


std::shared_ptr<RGBFrame>& BMPFileReader::getFrame()
{
    return frame;
}

std::string BMPFileReader::GetFilePath() const {
    return filePath;
}

int BMPFileReader::getWidth() const {
    return imgWidth;
}

int BMPFileReader::getHeight() const {
    return imgHeight;
}



