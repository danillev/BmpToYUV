#ifndef YUV_VIDEO_H
#define YUV_VIDEO_H

#include <string>
#include <fstream>
#include "BMPImage.h"

class YUVVideo {
public:
    YUVVideo(const std::string& filePath);
    bool open();
    void close();
    void insertBMP(const BMPImage& bmpImage, std::vector<unsigned char>& yData, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData);

private:
    std::string filePath;
    std::ifstream videoFile;
    std::ofstream outputFile;
};

#endif // YUV_VIDEO_H
