#pragma once

#include "Image.h"
#include <vector>

namespace CV {
	namespace Detection {
		class Canny {
		public:
			static void cannyEdgeDetector(const std::shared_ptr<CV::Utils::Image> input, std::shared_ptr<CV::Utils::Image>& output);
		private:
			static void calculateGradient(const unsigned char* image, int width, int height, std::vector<float>& gradientMagnitude, std::vector<float>& gradientAngle);
			static void nonMaxSuppression(const std::vector<float>& gradientMagnitude, const std::vector<float>& gradientAngle, int width, int height, unsigned char* output);
		};
	}
}

