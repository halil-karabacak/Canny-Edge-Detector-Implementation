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

std::vector<std::shared_ptr<CV::Utils::Image>> CV::Utils::Image::BuildScaleSpacePyramid(int levels, float sigma0, float k)
{
    std::vector<std::shared_ptr<Image>> pyramid;
    pyramid.push_back(std::enable_shared_from_this<Image>::shared_from_this());  // Base level is the original image
    float sigma = sigma0;

    for (int level = 1; level <= levels; level++) {
        sigma *= k;

        std::shared_ptr<Image> blurredImage = std::make_shared<Image>();
        blurredImage->width = width / 2;
        blurredImage->height = height / 2;
        blurredImage->grayData.resize(blurredImage->width * blurredImage->height);

        for (int y = 0; y < blurredImage->height; y++) {
            for (int x = 0; x < blurredImage->width; x++) {
                float sum = 0.0f;
                float totalWeight = 0.0f;
                int kernelRadius = 3 * sigma; // 

                for (int j = -kernelRadius; j <= kernelRadius; j++) {
                    for (int i = -kernelRadius; i <= kernelRadius; i++) {
                        int neighborX = 2 * x + i;
                        int neighborY = 2 * y + j;

                        if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                            uint8_t neighborPixel = grayData[neighborY * width + neighborX];
                            float weight = gaussianKernel(i, j, sigma);
                            sum += weight * neighborPixel;
                            totalWeight += weight;
                        }
                    }
                }

                sum /= totalWeight;
                blurredImage->grayData[y * blurredImage->width + x] = static_cast<uint8_t>(sum);
            }
        }

        pyramid.push_back(blurredImage);
    }

    return pyramid;
}

void CV::Utils::Image::GenerateDOGImages(int levels, int sigma0, int k)
{
    std::vector<std::shared_ptr<Image>> pyramid = this->BuildScaleSpacePyramid(levels, sigma0, k);
    DOGImages.clear(); 
    for (int level = 1; level < pyramid.size(); level++) {
        std::shared_ptr<Image> dogImage = std::make_shared<Image>();
        dogImage->width = pyramid[level]->width;
        dogImage->height = pyramid[level]->height;
        dogImage->grayData.resize(dogImage->width * dogImage->height);

        // Subtract adjacent scales to calculate the DoG image
        for (int i = 0; i < dogImage->grayData.size(); i++) {
            dogImage->grayData[i] = pyramid[level]->grayData[i] - pyramid[level - 1]->grayData[i];
        }

        DOGImages.push_back(dogImage);
    }
}

std::vector<std::shared_ptr<CV::Utils::Image>> CV::Utils::Image::GenerateExtremaImages()
{
    std::vector<std::shared_ptr<Image>> extremaImages;

    for (int level = 1; level < DOGImages.size() - 1; level++) {
        std::shared_ptr<Image> extremaImage = std::make_shared<Image>();
        extremaImage->width = DOGImages[level]->width;
        extremaImage->height = DOGImages[level]->height;
        extremaImage->grayData.resize(extremaImage->width * extremaImage->height);

        for (int y = 1; y < extremaImage->height - 1; y++) {
            for (int x = 1; x < extremaImage->width - 1; x++) {
                uint8_t pixelValue = DOGImages[level]->grayData[y * extremaImage->width + x];

                bool isExtrema = true;
                for (int j = -1; j <= 1; j++) {
                    for (int i = -1; i <= 1; i++) {
                        for (int k = -1; k <= 1; k++) {
                            if (DOGImages[level + j]->grayData[(y + k) * extremaImage->width + (x + i)] >= pixelValue) {
                                isExtrema = false;
                                break;
                            }
                        }
                        if (!isExtrema) break;
                    }
                    if (!isExtrema) break;
                }

                if (isExtrema) {
                    extremaImage->grayData[y * extremaImage->width + x] = 255;
                }
            }
        }
        extremaImages.push_back(extremaImage);
    }
    return extremaImages;
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

            for (int j = -radius; j <= radius; j++) {
                for (int i = -radius; i <= radius; i++) {
                    int neighborX = x + i;
                    int neighborY = y + j;

                    if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                        uint8_t neighborPixel = grayData[neighborY * width + neighborX];
                        float weight = gaussianKernel(i, j, radius);
                        sum += weight * neighborPixel;
                        totalWeight += weight;
                    }
                }
            }

            sum /= totalWeight;
            blurredData[y * width + x] = static_cast<uint8_t>(sum);
        }
    }
    grayData = blurredData;
}
