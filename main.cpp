#include<iostream>
#include "Tracer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "./stb/stb_image_write.h"
#include "./stb/stb_image.h"
using namespace std;

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples = 30;
    const int max_depth = 100;

int main(){
    Tracer rayTracer;
    rayTracer.initialize(image_width, image_height, 4);

    rayTracer.setMaxDepth(50);
    
    unsigned char* image;
    image = rayTracer.render(samples);
    stbi_flip_vertically_on_write(true);
    stbi_write_png("out_50depth_30samples.png", image_width, image_height, 4, image, 0);
    return 0;
}