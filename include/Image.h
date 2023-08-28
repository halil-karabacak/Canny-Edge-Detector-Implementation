#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
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
			std::vector<Eigen::Vector3i> rgbData;
			ImageType type = ImageType::DEFAULT;
			cv::Mat mat;
		private:
			std::vector<std::shared_ptr<Image>> DOGImages;
			void ApplyGaussianBlurRGB(int radius);
			void ApplyGaussianBlurGray(int radius);
			std::vector<std::shared_ptr<Image>> BuildScaleSpacePyramid(int levels, float sigma0, float k);
		};
	}
}
