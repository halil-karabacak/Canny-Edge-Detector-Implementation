#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstddef>


namespace CV {
	namespace Utils {
		struct RGB {
			unsigned char r, g, b;
		};

		enum class ImageType {
			RGB,
			BGR,
			GRAY,
			DEFAULT
		};

		class Image {
		public:
			static std::shared_ptr<Image> loadPNG(std::string file_path);
			void RGB2GRAY();
			std::vector<double> GenerateGaussianKernel(double sigma, int size);

			void ApplyGaussianBlur(int radius);

			int width, height;
			std::vector<RGB> rgbData;
			std::vector<uint8_t> grayData;
			ImageType type = ImageType::DEFAULT;
		private:
			void ApplyGaussianBlurRGB(int radius);
			void ApplyGaussianBlurGray(int radius);
		};
	}
}
