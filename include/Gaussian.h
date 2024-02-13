#pragma once
#include "Image.h"
#include <vector>

namespace CV {
    namespace Detection {
        class GaussianFilter
        {
        public:
            double scaleFactor;
            void static calculateGradient(const CV::Utils::Image* Image, float* gradientMagnitude, float* gradientAngle);

            static std::vector<std::vector<double>> Produce2dGaussianKernel(double sigma);
            static std::shared_ptr<CV::Utils::Image> ApplyGaussianBlur(const CV::Utils::Image& input, std::shared_ptr<CV::Utils::Image>& out, double sigma = 5);
        };
    }
}
