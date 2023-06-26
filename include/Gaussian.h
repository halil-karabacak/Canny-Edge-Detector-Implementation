#pragma once


#include <matrix.h>

namespace CV {
    namespace Canny {
        class GaussianFilter : public CV::Utils::Matrix
        {
        public:
            GaussianFilter(double sigma);
            double scaleFactor;

        };
    }
}

