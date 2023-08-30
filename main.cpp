#include "Canny.h"
#include "Image.h"



int main()
{
    auto image = CV::Utils::Image::loadPNG("../data/buran.jpg");
    auto mat = image.mat;

    cv::Mat out;
    out = CV::Detection::GaussianFilter::ApplyGaussianBlur(mat, 5);
    

    cv::imshow("Gaus Blur", out);
    cv::imwrite("../data/gaus_blur.png", out);

    cv::waitKey(0);
}