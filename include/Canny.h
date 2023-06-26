#pragma once


#include <matrix.h>

namespace CV {
	namespace Canny {
		class Canny {
		public:
			static void convolution(CV::Utils::Matrix& output, CV::Utils::Matrix& image, CV::Utils::Matrix& filter, int padding, double scaleFactor);
			static double convSum(CV::Utils::Matrix& image, CV::Utils::Matrix& filter, int padding, int centerRow, int centerCol, int offset);
			static void calculateIntensity(CV::Utils::Matrix& output, CV::Utils::Matrix& edgeX, CV::Utils::Matrix& edgeY);
			static void calculateEdgeDir(CV::Utils::Matrix& output, CV::Utils::Matrix& edgeX, CV::Utils::Matrix& edgeY, double threshold);
			static void nonMaxSupression(CV::Utils::Matrix& output, CV::Utils::Matrix& edgeIntensity, CV::Utils::Matrix& edgeDir);
			static int maxTrio(int before, int center, int after);
		};
	}
}

