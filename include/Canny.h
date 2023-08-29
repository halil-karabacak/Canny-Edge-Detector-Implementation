#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "Gaussian.h"

namespace CV {
	namespace Detection {
		class Canny {
		public:
			static void nonMaxSupression(const cv::Mat& gradientMagnitude, const cv::Mat& gradientAngle, cv::Mat& output);
			static void cannyEdgeDetector(const cv::Mat& input, cv::Mat& output);
		};
	}
}

