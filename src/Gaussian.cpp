#include "Gaussian.h"
#include <math.h>
#define M_PI   3.14159265358979323846264338327950288
#include <math.h>
#define M_PI   3.14159265358979323846264338327950288

using namespace CV::Detection;


inline float gaussianKernel(int x, int y, float sigma) {
    float coefficient = 1.0f / (2.0f * M_PI * sigma * sigma);
    float exponent = -(x * x + y * y) / (2.0f * sigma * sigma);
    return coefficient * exp(exponent);
}


std::vector<std::vector<double>> GaussianFilter::Produce2dGaussianKernel(double sigma) {
    int kernelRadius = std::ceil(6 * sigma);
    if (kernelRadius % 2 == 0) {
        kernelRadius++;
    }

    std::vector<std::vector<double>> kernel2d(kernelRadius, std::vector<double>(kernelRadius));
    double sum = 0;

    for (int row = 0; row < kernel2d.size(); row++) {
        for (int col = 0; col < kernel2d[row].size(); col++) {
            int x = row - kernelRadius / 2;
            int y = col - kernelRadius / 2;
            double val = gaussianKernel(x, y, sigma);
            kernel2d[row][col] = val;
            sum += val;
        }
    }

    for (int row = 0; row < kernel2d.size(); row++) {
        for (int col = 0; col < kernel2d[row].size(); col++) {
            kernel2d[row][col] /= sum;
        }
    }

    return kernel2d;
}




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



void GaussianFilter::calculateGradient(const cv::Mat& image, cv::Mat& gradientMagnitude, cv::Mat& gradientAngle) {
    cv::Mat sobelX = (cv::Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::Mat sobelY = (cv::Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

    for (int i = 1; i < image.rows - 1; ++i) {
        for (int j = 1; j < image.cols - 1; ++j) {
            int gx = 0, gy = 0;
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    gx += image.at<uchar>(i + k, j + l) * sobelX.at<int>(k + 1, l + 1);
                    gy += image.at<uchar>(i + k, j + l) * sobelY.at<int>(k + 1, l + 1);
                }
            }
            gradientMagnitude.at<float>(i, j) = sqrt(gx * gx + gy * gy);
            gradientAngle.at<float>(i, j) = atan2(gy, gx);
        }
    }
}

cv::Mat CV::Detection::GaussianFilter::ApplyGaussianBlur(cv::Mat input, double sigma)
{
    cv::Mat blurredImage = input.clone();
    auto kernel = Produce2dGaussianKernel(sigma);
    auto kernelSize = kernel.size();

    for (int y = kernelSize / 2; y < input.rows - kernelSize / 2; ++y) {
        for (int x = kernelSize / 2; x < input.cols - kernelSize / 2; ++x) {
            double newValue = 0.0;
            for (int i = 0; i < kernelSize; ++i) {
                for (int j = 0; j < kernelSize; ++j) {
                    newValue += kernel[i][j] * input.at<uchar>(y + i - kernelSize / 2, x + j - kernelSize / 2);
                }
            }
            blurredImage.at<uchar>(y, x) = static_cast<uchar>(newValue);
        }
    }
    return blurredImage;
}


cv::Mat CV::Detection::GaussianFilter::ApplyGaussianBlur(cv::Mat input, double sigma)
{
    cv::Mat blurredImage = input.clone();
    auto kernel = Produce2dGaussianKernel(sigma);
    auto kernelSize = kernel.size();

    for (int y = kernelSize / 2; y < input.rows - kernelSize / 2; ++y) {
        for (int x = kernelSize / 2; x < input.cols - kernelSize / 2; ++x) {
            double newValue = 0.0;
            for (int i = 0; i < kernelSize; ++i) {
                for (int j = 0; j < kernelSize; ++j) {
                    newValue += kernel[i][j] * input.at<uchar>(y + i - kernelSize / 2, x + j - kernelSize / 2);
                }
            }
            blurredImage.at<uchar>(y, x) = static_cast<uchar>(newValue);
        }
    }
    return blurredImage;
}

cv::Mat CV::Detection::GaussianFilter::ApplyGaussianBlur(cv::Mat input, std::vector<std::vector<double>> vec)
{
    cv::Mat blurredImage = input.clone();
    auto kernel = vec;
    auto kernelSize = kernel.size();

    for (int y = kernelSize / 2; y < input.rows - kernelSize / 2; ++y) {
        for (int x = kernelSize / 2; x < input.cols - kernelSize / 2; ++x) {
            double newValue = 0.0;
            for (int i = 0; i < kernelSize; ++i) {
                for (int j = 0; j < kernelSize; ++j) {
                    newValue += kernel[i][j] * input.at<uchar>(y + i - kernelSize / 2, x + j - kernelSize / 2);
                }
            }
            blurredImage.at<uchar>(y, x) = static_cast<uchar>(newValue);
        }
    }
    return blurredImage;
}
