#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>


namespace CV {
	namespace Detection {
		class Sift {
		public:
			static cv::Mat generateBaseImage(const cv::Mat& image, double sigma, double assumed_blur);
		private:
			static int computeNumberOfOctaves(const cv::Size& image_shape);
			static std::vector<double> generateGaussianKernels(double sigma, int num_intervals);
			static std::vector<std::vector<cv::Mat>> generateGaussianImages(cv::Mat image, int num_octaves, const std::vector<double>& gaussian_kernels);
			std::vector<std::vector<cv::Mat>> generateDoGImages(const std::vector<std::vector<cv::Mat>>& gaussian_images);
		};
	}
}
