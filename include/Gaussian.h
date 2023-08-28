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
        };
    }
}

