#include "YUVVideo.h"
#include "BMPImage.h"
#include <iostream>

using namespace std;

YUVVideo::YUVVideo(const std::string& filePath) : filePath(filePath) {}

bool YUVVideo::open() {
    videoFile.open(filePath, ios::binary);
    if (!videoFile.is_open()) {
        cerr << "Failed to open YUV420 video file" << endl;
        return false;
    }
    return true;
}

void YUVVideo::close() {
    if (videoFile.is_open()) {
        videoFile.close();
    }
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

void YUVVideo::insertBMP(const BMPImage& bmpImage, std::vector<unsigned char>& yData, std::vector<unsigned char>& uData, std::vector<unsigned char>& vData) {
    outputFile.open("output_video.yuv", ios::binary);
    int frameCounter = 0;
    int imgWidth = bmpImage.getWidth();
    int imgHeight = bmpImage.getHeight();
    int videoWidth = 352;
    int videoHeight = 288;

    int frameSize = videoWidth * videoHeight * 3 / 2; // YUV420 format

    vector<unsigned char> frameBuffer(frameSize);



    while (!videoFile.eof()) {
        videoFile.read(reinterpret_cast<char*>(frameBuffer.data()), frameSize);


        for (int i = 0; i < imgHeight; ++i) {
            for (int j = 0; j < imgWidth; ++j) {
                int videoYIndex = i * videoWidth + j;
                int videoUVIndex = videoWidth * videoHeight + (i / 2) * (videoWidth / 2) + (j / 2);

                unsigned char Y = yData[i * imgWidth + j];

                frameBuffer[videoYIndex] = Y;
                
                if (i % 2 == 0 && j % 2 == 0) {
                    unsigned char U = uData[(i / 2) * (imgWidth / 2) + (j / 2)];
                    unsigned char V = vData[(i / 2) * (imgWidth / 2) + (j / 2)];

                    frameBuffer[videoUVIndex] = U;
                    frameBuffer[videoUVIndex + videoWidth * videoHeight / 4] = V;
                }
            }
        }

        outputFile.write(reinterpret_cast<char*>(frameBuffer.data()), frameSize);
        frameCounter++;
    }

    outputFile.close();
}
