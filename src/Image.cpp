#include <Image.h>
#include <cmath>

#define M_PI   3.14159265358979323846264338327950288

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
    Image->type = ImageType::RGB;
    return Image;
}

void CV::Utils::Image::RGB2GRAY()
{
    if (type == ImageType::GRAY) return;
    grayData.reserve(rgbData.size());
    for (auto channel : rgbData) {
        int sum = channel.r + channel.g + channel.b;
        grayData.push_back(sum / 3);
    }
    type = ImageType::GRAY;
}

/*
inline double gaussian(double x, double mu, double sigma) {
    const double a = (x - mu) / sigma;
    return std::exp(-0.5 * a * a);
}


inline std::vector<std::vector<double>> Produce2dGaussianKernel(int kernelRadius) {
    double sigma = kernelRadius / 2.;
    std::vector<std::vector<double>> kernel2d(2 * kernelRadius + 1, std::vector<double>(2 * kernelRadius + 1));
    double sum = 0;
    // compute values
    for (int row = 0; row < kernel2d.size(); row++)
        for (int col = 0; col < kernel2d[row].size(); col++) {
            double x = gaussian(row, kernelRadius, sigma)
                * gaussian(col, kernelRadius, sigma);
            kernel2d[row][col] = x;
            sum += x;
        }
    // normalize
    for (int row = 0; row < kernel2d.size(); row++)
        for (int col = 0; col < kernel2d[row].size(); col++)
            kernel2d[row][col] /= sum;
    return kernel2d;
}
*/

float gaussianKernel(int x, int y, float sigma) {
    float coefficient = 1.0f / (2.0f * M_PI * sigma * sigma);
    float exponent = -(x * x + y * y) / (2.0f * sigma * sigma);
    return coefficient * exp(exponent);
}

void CV::Utils::Image::ApplyGaussianBlur(int radius)
{
    if (type == ImageType::GRAY)
        ApplyGaussianBlurGray(radius);
    else
        ApplyGaussianBlurRGB(radius);
}

void CV::Utils::Image::ApplyGaussianBlurRGB(int radius)
{
    int kernelSize = static_cast<int>(6 * radius + 1);
    if (kernelSize % 2 == 0) {
        kernelSize += 1;
    }

    std::vector<RGB> blurredData(rgbData.size());

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
            float totalWeight = 0.0f;

            for (int j = -radius; j <= radius; j++) {
                for (int i = -radius; i <= radius; i++) {
                    int neighborX = x + i;
                    int neighborY = y + j;

                    if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                        RGB neighborPixel = rgbData[neighborY * width + neighborX];
                        float weight = gaussianKernel(i, j, radius);
                        sumR += weight * neighborPixel.r;
                        sumG += weight * neighborPixel.g;
                        sumB += weight * neighborPixel.b;
                        totalWeight += weight;
                    }
                }
            }

            sumR /= totalWeight;
            sumG /= totalWeight;
            sumB /= totalWeight;

            blurredData[y * width + x].r = static_cast<uint8_t>(sumR);
            blurredData[y * width + x].g = static_cast<uint8_t>(sumG);
            blurredData[y * width + x].b = static_cast<uint8_t>(sumB);
        }
    }

    rgbData = blurredData;
}

void CV::Utils::Image::ApplyGaussianBlurGray(int radius)
{
    std::vector<uint8_t> blurredData(width * height);

    // Apply Gaussian blur to each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sum = 0.0f;
            float totalWeight = 0.0f;

            // Apply blur to surrounding pixels within the kernel
            for (int j = -radius; j <= radius; j++) {
                for (int i = -radius; i <= radius; i++) {
                    int neighborX = x + i;
                    int neighborY = y + j;

                    // Check if neighbor pixel is within bounds
                    if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                        uint8_t neighborPixel = grayData[neighborY * width + neighborX];
                        float weight = gaussianKernel(i, j, radius);
                        sum += weight * neighborPixel;
                        totalWeight += weight;
                    }
                }
            }

            // Normalize the accumulated value
            sum /= totalWeight;

            blurredData[y * width + x] = static_cast<uint8_t>(sum);
        }
    }

    // Replace the original gray data with the blurred data
    grayData = blurredData;
}
