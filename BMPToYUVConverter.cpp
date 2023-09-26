#include "BMPToYUVConverter.h"

yuvData BMPToYUVConverter::convertToYUV(std::shared_ptr<RGBFrame> frame) {
    yuvData yuvDataPtr;
    int imgHeight = frame->getHeight();
    int imgWidth = frame->getWidth();
    int ySize = imgWidth * imgHeight;
    int uvSize = imgWidth * imgHeight / 4;

    yuvDataPtr.yData.resize(ySize);
    yuvDataPtr.uData.resize(uvSize);
    yuvDataPtr.vData.resize(uvSize);

    yuvDataPtr.yData = BMPToYUVConverter::convertToY(frame);
    yuvDataPtr.uData = BMPToYUVConverter::convertToU(frame);
    yuvDataPtr.vData = BMPToYUVConverter::convertToV(frame);

    return yuvDataPtr;
}



std::vector<unsigned char> BMPToYUVConverter::convertToY(std::shared_ptr<RGBFrame> frameImage)
{
    int imgWidth = frameImage->getWidth();
    int imgHeight = frameImage->getHeight();
    std::vector<unsigned char> yData(imgWidth * imgHeight);

    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (int threadId = 0; threadId < numThreads; ++threadId) {
        threads.emplace_back([&, threadId]() {
            for (int i = threadId; i < imgHeight; i += numThreads) {
                int yIndex = i * imgWidth;
                for (int j = 0; j < imgWidth; ++j) {
                    RGBPixel rgb = frameImage->getPixel(j, i);
                    yData[yIndex + j] = BMPToYUVConverter::RGBtoY(rgb);
                }
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return yData;
}

std::vector<unsigned char> BMPToYUVConverter::convertToU(std::shared_ptr<RGBFrame> frameImage) {

    int imgWidth = frameImage->getWidth();
    int imgHeight = frameImage->getHeight();
    std::vector<unsigned char> uData(imgWidth * imgHeight / 4);

    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (int threadId = 0; threadId < numThreads; ++threadId) {
        threads.emplace_back([&, threadId]() {
            int iOffset = threadId * 2;
            for (int i = iOffset; i < imgHeight; i += numThreads * 2) {
                int uvIndex = (i / 2) * (imgWidth / 2);
                for (int j = 0; j < imgWidth; j += 2) {
                    RGBPixel rgb = frameImage->getPixel(j, i);
                    
                    uData[uvIndex + (j / 2)] = BMPToYUVConverter::RGBtoU(rgb);
                }
            }
            });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return uData;
}

std::vector<unsigned char> BMPToYUVConverter::convertToV(std::shared_ptr<RGBFrame> frameImage) {

    int imgWidth = frameImage->getWidth();
    int imgHeight = frameImage->getHeight();
    std::vector<unsigned char> vData(imgWidth * imgHeight / 4);

    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (int threadId = 0; threadId < numThreads; ++threadId) {
        threads.emplace_back([&, threadId]() {
            int iOffset = threadId * 2;
            for (int i = iOffset; i < imgHeight; i += numThreads * 2) {
                int uvIndex = (i / 2) * (imgWidth / 2);
                for (int j = 0; j < imgWidth; j += 2) {
                    RGBPixel rgb = frameImage->getPixel(j, i);
                    vData[uvIndex + (j / 2)] = BMPToYUVConverter::RGBtoV(rgb);
                }
            }
            });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return vData;
}

unsigned char BMPToYUVConverter::RGBtoY(const RGBPixel& rgb) {
    __m128i R_vector = _mm_set1_epi8(rgb.R);
    __m128i G_vector = _mm_set1_epi8(rgb.G);
    __m128i B_vector = _mm_set1_epi8(rgb.B);

    // Multiplication of each component by the corresponding coefficient and addition
    __m128i Y_vector = _mm_add_epi16(
        _mm_mullo_epi16(R_vector, _mm_set1_epi16(299)),
        _mm_add_epi16(
            _mm_mullo_epi16(G_vector, _mm_set1_epi16(587)),
            _mm_mullo_epi16(B_vector, _mm_set1_epi16(114))
        )
    );

    // Division on 1000
    Y_vector = _mm_div_epi16(Y_vector, _mm_set1_epi16(1000));

    unsigned char y = _mm_extract_epi8(Y_vector, 0);

    return y;
}
unsigned char BMPToYUVConverter::RGBtoU(const RGBPixel& rgb) {
   
    __m128i R_vector = _mm_set1_epi8(rgb.R);
    __m128i G_vector = _mm_set1_epi8(rgb.G);
    __m128i B_vector = _mm_set1_epi8(rgb.B);

    __m128i Y_vector = _mm_set1_epi8(BMPToYUVConverter::RGBtoY(rgb));

    __m128i U_vector = _mm_sub_epi16(B_vector, Y_vector);
    U_vector = _mm_mullo_epi16(U_vector, _mm_set1_epi16(564));
    U_vector = _mm_div_epi16(U_vector, _mm_set1_epi16(1000));
    U_vector = _mm_add_epi16(U_vector, _mm_set1_epi16(128));

    U_vector = _mm_min_epu8(_mm_max_epu8(U_vector, _mm_setzero_si128()), _mm_set1_epi8(255));

    unsigned char u = _mm_extract_epi8(U_vector, 0);

    return u;
}
unsigned char BMPToYUVConverter::RGBtoV(const RGBPixel& rgb) {
    // Loading R, G and B into separate 128-bit registers
    __m128i R_vector = _mm_set1_epi8(rgb.R);
    __m128i G_vector = _mm_set1_epi8(rgb.G);
    __m128i B_vector = _mm_set1_epi8(rgb.B);

    // Calculation of Y
    __m128i Y_vector = _mm_set1_epi8(RGBtoY(rgb));

    // Calculation of V
    __m128i V_vector = _mm_sub_epi16(R_vector, Y_vector);
    V_vector = _mm_mullo_epi16(V_vector, _mm_set1_epi16(713));
    V_vector = _mm_div_epi16(V_vector, _mm_set1_epi16(1000));
    V_vector = _mm_add_epi16(V_vector, _mm_set1_epi16(128));

    // Limiting results in the range from 0 to 255
    V_vector = _mm_min_epu8(_mm_max_epu8(V_vector, _mm_setzero_si128()), _mm_set1_epi8(255));

    unsigned char v = _mm_extract_epi8(V_vector, 0);

    return v;
}


