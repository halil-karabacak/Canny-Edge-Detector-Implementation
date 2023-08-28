#include "Canny.h"
#include "Image.h"



int main()
{
    auto image = CV::Utils::Image::loadPNG("../data/buran.jpg");
    auto mat = image.mat;

    cv::Mat out;
    CV::Detection::Canny::cannyEdgeDetector(mat, out);
    

    cv::imshow("Canny Edge Detection", out);
    cv::imwrite("../data/canny_out.png", out);

    cv::waitKey(0);
}