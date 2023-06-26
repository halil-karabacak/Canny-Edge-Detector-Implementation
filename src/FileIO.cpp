#include <FileIO.h>

inline bool IsPNG(const char* data) {
    return (data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47 &&
        data[4] == 0x0D && data[5] == 0x0A && data[6] == 0x1A && data[7] == 0x0A);
}

std::shared_ptr<CV::Utils::Image> CV::Utils::Image::loadPNG(std::string file_path)
{
    std::shared_ptr<CV::Utils::Image> Image;
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* data = new char[size];
    file.read(data, size);
    file.close();

    if (!IsPNG(data)) {
        std::cerr << "Not a valid PNG file: " << file_path << std::endl;
        delete[] data;
        return nullptr;
    }

    size_t offset = 8;


    while (offset < size) {
        uint32_t chunk_length = (data[offset] << 24) | (data[offset + 1] << 16) | (data[offset + 2] << 8) | data[offset + 3];
        offset += 4;

        std::string chunk_type(data + offset, data + offset + 4);
        offset += 4;

        if (chunk_type == "IHDR") {
            Image->width = (data[offset] << 24) | (data[offset + 1] << 16) | (data[offset + 2] << 8) | data[offset + 3];
            Image->height = (data[offset + 4] << 24) | (data[offset + 5] << 16) | (data[offset + 6] << 8) | data[offset + 7];

            offset += chunk_length;
            break;
        }
        offset += chunk_length + 4;
    }

    while (offset < size) {
        uint32_t chunk_length = (data[offset] << 24) | (data[offset + 1] << 16) | (data[offset + 2] << 8) | data[offset + 3];
        offset += 4;


        std::string chunk_type(data + offset, data + offset + 4);
        offset += 4;

        if (chunk_type == "IDAT") {
            size_t pixel_size = 3;
            size_t num_pixels = (size - offset) / pixel_size;

            Image->rgbData.reserve(num_pixels);

            // Extract RGB values for each pixel
            for (size_t i = 0; i < num_pixels; i++) {
                uint8_t r = static_cast<uint8_t>(data[offset + i * pixel_size]);
                uint8_t g = static_cast<uint8_t>(data[offset + i * pixel_size + 1]);
                uint8_t b = static_cast<uint8_t>(data[offset + i * pixel_size + 2]);

                RGB rgb;
                rgb.r = r;
                rgb.g = g;
                rgb.b = b;
                Image->rgbData.push_back(rgb);
            }
            break;
        }
        offset += chunk_length + 4;
    }

    delete[] data;
}
