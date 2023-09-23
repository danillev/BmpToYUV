#include "YUVVideo.h"
#include "BMPFileReader.h"
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

void YUVVideo::insertBMP(const BMPFileReader& bmpImage, std::shared_ptr<yuvData> yuvDataPtr) {
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
                int videoUVIndex = videoWidth * videoHeight + i / 2 * (videoWidth / 2) +  j / 2;

                unsigned char Y = yuvDataPtr->yData[i * imgWidth + j];
                unsigned char U = yuvDataPtr->uData[(i / 2) * (imgWidth / 2) + (j / 2)];
                unsigned char V = yuvDataPtr->vData[(i / 2) * (imgWidth / 2) + (j / 2)];

                frameBuffer[videoYIndex] = Y;
                if (i % 2 == 0 && j % 2 == 0) {
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
