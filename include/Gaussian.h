#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace CV {
    namespace Detection {
        class GaussianFilter
        {
        public:
            double scaleFactor;
            void static calculateGradient(const cv::Mat& image, cv::Mat& gradientMagnitude, cv::Mat& gradientAngle);

            static std::vector<std::vector<double>> Produce2dGaussianKernel(double sigma);
            static cv::Mat ApplyGaussianBlur(cv::Mat input, double sigma);
        };
    }
}

