#include "Image.h"
#include <math.h>
#include <Canny.h>

using namespace CV::Detection;

void Canny::nonMaxSupression(const cv::Mat& gradientMagnitude, const cv::Mat& gradientAngle, cv::Mat& output) {
    output = gradientMagnitude.clone();

    for (int i = 1; i < gradientMagnitude.rows - 1; ++i) {
        for (int j = 1; j < gradientMagnitude.cols - 1; ++j) {
            float angle = gradientAngle.at<float>(i, j) * 180 / CV_PI;
            angle = (angle < 0) ? angle + 180 : angle;

            int q = 255;
            int r = 255;

            if ((0 <= angle && angle < 22.5) || (157.5 <= angle && angle <= 180))
                q = gradientMagnitude.at<float>(i, j + 1);
            else if (22.5 <= angle && angle < 67.5)
                q = gradientMagnitude.at<float>(i + 1, j + 1);
            else if (67.5 <= angle && angle < 112.5)
                q = gradientMagnitude.at<float>(i + 1, j);
            else if (112.5 <= angle && angle < 157.5)
                q = gradientMagnitude.at<float>(i + 1, j - 1);

            if (gradientMagnitude.at<float>(i, j) >= q && gradientMagnitude.at<float>(i, j) >= r)
                output.at<float>(i, j) = gradientMagnitude.at<float>(i, j);
            else
                output.at<float>(i, j) = 0;
        }
    }
}

void Canny::cannyEdgeDetector(const cv::Mat& input, cv::Mat& output)
{
    cv::Mat gradientMagnitude(input.size(), CV_32FC1);
    cv::Mat gradientAngle(input.size(), CV_32FC1);

    GaussianFilter::calculateGradient(input, gradientMagnitude, gradientAngle);
    Canny::nonMaxSupression(gradientMagnitude, gradientAngle, output);
}
