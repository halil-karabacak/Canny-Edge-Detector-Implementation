#pragma once

#include <matrix.h>

namespace CV {
    namespace Canny {
        enum class FilterType {
            X,
            Y
        };
        class SobelFilter : public CV::Utils::Matrix
        {
        public:
            SobelFilter(CV::Canny::FilterType Type);

        };
    }
}

