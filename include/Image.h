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

		class Image : std::enable_shared_from_this<Image> {
		public:
			/**
			* Reads PNG File from disk, reads it as a RGB.
			*/
			static std::shared_ptr<Image> loadPNG(std::string file_path);
			/**
			* Converts RGB image to Gray-scale. Loads new data to grayData.
			*/
			void RGB2GRAY();
			/**
			* Applies Gaussian Blurs for the given radius.
			*/
			void ApplyGaussianBlur(int radius);
			/**
			* Calculates Difference of Gaussians. Puts the results in DOGImages.
			*/
			void GenerateDOGImages(int levels, int sigma0, int k);
			/**
			* Generating Extrema Images from an already Generated set of DOG Images.
			*/
			std::vector<std::shared_ptr<Image>> GenerateExtremaImages();

			int width, height;
			std::vector<RGB> rgbData;
			std::vector<uint8_t> grayData;
			ImageType type = ImageType::DEFAULT;
		private:
			std::vector<std::shared_ptr<Image>> DOGImages;
			void ApplyGaussianBlurRGB(int radius);
			void ApplyGaussianBlurGray(int radius);
			std::vector<std::shared_ptr<Image>> BuildScaleSpacePyramid(int levels, float sigma0, float k);
		};
	}
}
