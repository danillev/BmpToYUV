cmake_minimum_required(VERSION 3.10)

project(TestTaskElecard)

# Установите стандарт языка C++ на C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавьте исполняемый файл и исходные файлы
add_executable(TestTaskElecard 
    TestTaskElecard.cpp 
    BMPImage.cpp 
    BMPToYUVConverter.cpp 
    YUVVideo.cpp)

# Укажите путь к заголовочным файлам вашего проекта
include_directories(${CMAKE_SOURCE_DIR})

# Укажите ресурсы файлы
configure_file(231000001.bmp ${CMAKE_BINARY_DIR}/231000001.bmp COPYONLY)
configure_file(231000002.bmp ${CMAKE_BINARY_DIR}/231000002.bmp COPYONLY)
configure_file(bus_cif.yuv ${CMAKE_BINARY_DIR}/bus_cif.yuv COPYONLY)

# Свяжите заголовочные файлы с исходными файлами
target_include_directories(TestTaskElecard PUBLIC ${CMAKE_SOURCE_DIR})

# Добавьте файл Structures.h к вашему проекту
target_sources(TestTaskElecard PRIVATE Structures.h)
