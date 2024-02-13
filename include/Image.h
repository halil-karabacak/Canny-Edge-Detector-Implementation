#pragma once

#include <memory>
#include <iostream>

namespace CV {
	namespace Utils {
		enum class ImageFormat {
			RGB,
			GRAY,
			DEFAULT // RGB
		};

		class Image : std::enable_shared_from_this<Image> {
		public:
			/**
			* Reads PNG File from disk, reads it as a RGB.
			*/
			static std::shared_ptr<CV::Utils::Image> loadPNG(const char* file_path, ImageFormat format =  ImageFormat::DEFAULT);
			/**
			* Converts RGB image to Gray-scale. Loads new data to grayData.
			*/
			void RGB2GRAY();

			/**
			 * Saves the image to the disk
			*/
			static bool saveImageToDisk(const char* path, const std::shared_ptr<CV::Utils::Image>& image);

			ImageFormat type;
			int width;
			int height;
			int channels;
			unsigned char* data;
		};
	}
}
