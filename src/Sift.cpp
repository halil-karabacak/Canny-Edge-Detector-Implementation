#include "Sift.h"
#include "Gaussian.h"

cv::Mat CV::Detection::Sift::generateBaseImage(const cv::Mat& image, double sigma, double assumed_blur) {
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(), 2, 2, cv::INTER_LINEAR);

    double sigma_diff = std::sqrt(std::max((sigma * sigma) - (4 * assumed_blur * assumed_blur), 0.01));
    cv::Mat blurredImage;

    blurredImage = CV::Detection::GaussianFilter::ApplyGaussianBlur(resizedImage, sigma_diff);
    return blurredImage;
}


int CV::Detection::Sift::computeNumberOfOctaves(const cv::Size& image_shape) {
    return static_cast<int>(std::round(std::log(std::min(image_shape.width, image_shape.height)) / std::log(2) - 1));
}

std::vector<double> CV::Detection::Sift::generateGaussianKernels(double sigma, int num_intervals) {
    std::vector<double> gaussian_kernels(num_intervals + 3);

    double k = std::pow(2.0, 1.0 / num_intervals);
    gaussian_kernels[0] = sigma;

    for (int image_index = 1; image_index < gaussian_kernels.size(); ++image_index) {
        double sigma_previous = std::pow(k, image_index - 1) * sigma;
        double sigma_total = k * sigma_previous;
        gaussian_kernels[image_index] = std::sqrt(sigma_total * sigma_total - sigma_previous * sigma_previous);
    }
    return gaussian_kernels;
}

std::vector<std::vector<cv::Mat>> CV::Detection::Sift::generateGaussianImages(cv::Mat image, int num_octaves, const std::vector<double>& gaussian_kernels) {
    std::vector<std::vector<cv::Mat>> gaussian_images;

    for (int octave_index = 0; octave_index < num_octaves; ++octave_index) {
        std::vector<cv::Mat> gaussian_images_in_octave;
        gaussian_images_in_octave.push_back(image);

        for (size_t i = 1; i < gaussian_kernels.size(); ++i) {
            auto blurred_img = CV::Detection::GaussianFilter::ApplyGaussianBlur(image, gaussian_kernels[i]);
            gaussian_images_in_octave.push_back(image);
        }

        gaussian_images.push_back(gaussian_images_in_octave);
        cv::Mat octave_base = gaussian_images_in_octave[gaussian_images_in_octave.size() - 3];
        cv::resize(octave_base, image, cv::Size(octave_base.cols / 2, octave_base.rows / 2), 0, 0, cv::INTER_NEAREST);
    }
    return gaussian_images;
}

std::vector<std::vector<cv::Mat>> CV::Detection::Sift::generateDoGImages(const std::vector<std::vector<cv::Mat>>& gaussian_images) {
    std::vector<std::vector<cv::Mat>> dog_images;

    for (const auto& gaussian_images_in_octave : gaussian_images) {
        std::vector<cv::Mat> dog_images_in_octave;

        for (size_t i = 1; i < gaussian_images_in_octave.size(); ++i) {
            cv::Mat dog_image;
            cv::subtract(gaussian_images_in_octave[i], gaussian_images_in_octave[i - 1], dog_image);
            dog_images_in_octave.push_back(dog_image);
        }

        dog_images.push_back(dog_images_in_octave);
    }
    return dog_images;
}
