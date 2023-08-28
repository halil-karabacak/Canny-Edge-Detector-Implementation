#include <Image.h>
#include <cmath>



#define M_PI   3.14159265358979323846264338327950288

std::shared_ptr<CV::Utils::Image> CV::Utils::Image::loadPNG(std::string file_path)
{
    std::shared_ptr<CV::Utils::Image> out = std::make_shared<CV::Utils::Image>();
    
    auto img = cv::imread(file_path);
    if (img.empty()) {
        std::cout << "error: image not read from file! \n";         
        return(0);
    }

    out->width = img.cols;
    out->height = img.rows;
    
    // Access pixel values as uint8_t
    for (int row = 0; row < img.rows; ++row) {
        uint8_t* pixelRow = img.ptr<uint8_t>(row);  // Get pointer to the current row

        for (int col = 0; col < img.cols; ++col) {
            uint8_t blue = pixelRow[3 * col];         // Blue channel
            uint8_t green = pixelRow[3 * col + 1];    // Green channel
            uint8_t red = pixelRow[3 * col + 2];      // Red channel

            out->rgbData.push_back(Eigen::Vector3i(blue, green, red));   
        }
    }
    out->type = ImageType::BGR;
    out->mat = img;
    return out;
}

void CV::Utils::Image::RGB2GRAY()
{
    cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
    type = ImageType::GRAY;
}

float gaussianKernel(int x, int y, float sigma) {
    float coefficient = 1.0f / (2.0f * M_PI * sigma * sigma);
    float exponent = -(x * x + y * y) / (2.0f * sigma * sigma);
    return coefficient * exp(exponent);
}

void CV::Utils::Image::ApplyGaussianBlur(int radius)
{
    //if (type == ImageType::GRAY)
        //ApplyGaussianBlurGray(radius);
    //else
        //ApplyGaussianBlurRGB(radius);
}
