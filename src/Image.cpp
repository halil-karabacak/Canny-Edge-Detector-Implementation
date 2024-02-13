#include <Image.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


std::shared_ptr<CV::Utils::Image> CV::Utils::Image::loadPNG(const char* file_path, ImageFormat format)
{    
    std::shared_ptr<CV::Utils::Image> out = std::make_shared<CV::Utils::Image>();;

    unsigned char* image = stbi_load(file_path, &out->width, &out->height, &out->channels, 0);

    if (image != nullptr) {
        out->data = image;
    } else {
        // Handle error loading the image
        stbi_image_free(image);
        std::cout << "error: image not read from file! \n";
        return std::shared_ptr<CV::Utils::Image>();   
    }

    out->type = format;
    return out;
}


bool CV::Utils::Image::saveImageToDisk(const char* file_path, const std::shared_ptr<CV::Utils::Image>& image)
{
    if (image == nullptr || image->data == nullptr) {
        std::cout << "error: invalid image data! \n";
        return false;
    }

    if (stbi_write_png(file_path, image->width, image->height, image->channels, image->data, image->width * image->channels) != 0) {
        std::cout << "Image saved successfully to " << file_path << "\n";
        return true;
    } else {
        std::cout << "error: failed to save the image to " << file_path << "\n";
        return false;
    }
}

void CV::Utils::Image::RGB2GRAY()
{
    if (type == ImageFormat::GRAY) return;
    // Convert RGB to grayscale
    for (int i = 0; i < width * height; ++i) {
        data[i] = static_cast<unsigned char>(
            0.299 * data[3 * i] +
            0.587 * data[3 * i + 1] +
            0.114 * data[3 * i + 2]
        );
    }
    channels = 1;

    type = ImageFormat::GRAY;
}


