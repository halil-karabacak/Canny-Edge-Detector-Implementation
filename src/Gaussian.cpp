#include "Gaussian.h"
#include <cmath>

using namespace CV::Detection;
using namespace CV::Utils;

inline float gaussianKernel(int x, int y, float sigma) {
    float coefficient = 1.0f / (2.0f * M_PI * sigma * sigma);
    float exponent = -(x * x + y * y) / (2.0f * sigma * sigma);
    return coefficient * exp(exponent);
}


std::vector<std::vector<double>> GaussianFilter::Produce2dGaussianKernel(double sigma) {
    int kernelSize = std::ceil(2 * sigma + 1);
    if (kernelSize % 2 == 0) {
        kernelSize++;
    }

    std::vector<std::vector<double>> kernel2d(kernelSize, std::vector<double>(kernelSize));
    double sum = 0;

    for (int row = 0; row < kernel2d.size(); row++) {
        for (int col = 0; col < kernel2d[row].size(); col++) {
            int x = row - kernelSize / 2;
            int y = col - kernelSize / 2;
            double val = gaussianKernel(x, y, sigma);
            kernel2d[row][col] = val;
            sum += val;
        }
    }

    // normalization
    for (int row = 0; row < kernel2d.size(); row++) {
        for (int col = 0; col < kernel2d[row].size(); col++) {
            kernel2d[row][col] /= sum;
        }
    }

    return kernel2d;
}

void GaussianFilter::calculateGradient(const CV::Utils::Image* Image,float* gradientMagnitude, float* gradientAngle) {
    int sobelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int sobelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
    int width = Image->width;
    int height = Image->height;
    auto image = Image->data;
    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
            int gx = 0, gy = 0;
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    gx += image[(i + k) * width + (j + l)] * sobelX[k + 1][l + 1];
                    gy += image[(i + k) * width + (j + l)] * sobelY[k + 1][l + 1];
                }
            }
            gradientMagnitude[i * width + j] = sqrt(static_cast<float>(gx * gx + gy * gy));
            gradientAngle[i * width + j] = atan2(static_cast<float>(gy), static_cast<float>(gx));
        }
    }
}


std::shared_ptr<Image> GaussianFilter::ApplyGaussianBlur(const Image& input, std::shared_ptr<Image>& out, double sigma) {
    out = std::make_shared<Image>();
    out->width = input.width; out->height = input.height; out->channels = input.channels;

    unsigned char* blurredData = new unsigned char[input.width * input.height];
    
    auto kernel = Produce2dGaussianKernel(sigma);
    int kernelSize = kernel.size();

    for (int y = kernelSize / 2; y < input.height - kernelSize / 2; ++y) {
        for (int x = kernelSize / 2; x < input.width - kernelSize / 2; ++x) {
            double newValue = 0.0;
            for (int i = 0; i < kernelSize; ++i) {
                for (int j = 0; j < kernelSize; ++j) {
                    newValue += kernel[i][j] * input.data[(y + i - kernelSize / 2) * input.width + (x + j - kernelSize / 2)];
                }
            }
            blurredData[y * input.width + x] = static_cast<unsigned char>(newValue);
        }
    }
    out->data = blurredData;
    
    return out;
}