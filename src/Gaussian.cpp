#include "Gaussian.h"


using namespace CV::Detection;

void GaussianFilter::calculateGradient(const cv::Mat& image, cv::Mat& gradientMagnitude, cv::Mat& gradientAngle) {
    cv::Mat sobelX = (cv::Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::Mat sobelY = (cv::Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

    for (int i = 1; i < image.rows - 1; ++i) {
        for (int j = 1; j < image.cols - 1; ++j) {
            int gx = 0, gy = 0;
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    gx += image.at<uchar>(i + k, j + l) * sobelX.at<int>(k + 1, l + 1);
                    gy += image.at<uchar>(i + k, j + l) * sobelY.at<int>(k + 1, l + 1);
                }
            }
            gradientMagnitude.at<float>(i, j) = sqrt(gx * gx + gy * gy);
            gradientAngle.at<float>(i, j) = atan2(gy, gx);
        }
    }
}