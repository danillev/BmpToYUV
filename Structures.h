#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t dataSize;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    uint32_t colors;
    uint32_t importantColors;
};

struct RGBPixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};


#pragma pack(pop)
#endif // STRUCTURES_H
