#ifndef YUV_VIDEO_H
#define YUV_VIDEO_H

#include <string>
#include <fstream>
#include "BMPFileReader.h"

class YUVVideo {
public:
    YUVVideo(const std::string& filePath);
    bool open();
    void close();
    void insertBMP(const BMPFileReader& bmpImage, std::shared_ptr<yuvData> yuvDataPtr);

private:
    std::string filePath;
    std::ifstream videoFile;
    std::ofstream outputFile;
};

#endif // YUV_VIDEO_H
