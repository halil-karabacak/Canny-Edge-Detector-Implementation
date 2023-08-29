#pragma once

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace CV {
	namespace Utils {
		enum class ImageType {
			RGB,
			BGR,
			GRAY,
			DEFAULT
		};

		class Image : std::enable_shared_from_this<Image> {
		public:
			/**
			* Reads PNG File from disk, reads it as a BGR.
			*/
			static CV::Utils::Image loadPNG(std::string file_path);
			/**
			* Converts RGB image to Gray-scale. Loads new data to grayData.
			*/
			void RGB2GRAY();

			ImageType type = ImageType::DEFAULT;
			cv::Mat mat;
		};
	}
}
