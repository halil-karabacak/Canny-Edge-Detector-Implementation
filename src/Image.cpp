#include <Image.h>

#define M_PI   3.14159265358979323846264338327950288

CV::Utils::Image CV::Utils::Image::loadPNG(std::string file_path)
{    
    CV::Utils::Image out;
    auto img = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cout << "error: image not read from file! \n";         
        return CV::Utils::Image();
    }

    out.mat = img;
    out.type = ImageType::GRAY;
    return out;
}

void CV::Utils::Image::RGB2GRAY()
{
    cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
    type = ImageType::GRAY;
}


