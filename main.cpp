#include "Image.h"
#include "Gaussian.h"
#include "Canny.h"

int main()
{
    auto image = CV::Utils::Image::loadPNG("../data/input/buran.jpg");
    image->RGB2GRAY();
    std::shared_ptr<CV::Utils::Image> out;
    CV::Detection::Canny::cannyEdgeDetector(image, out);
    CV::Utils::Image::saveImageToDisk("../data/export/buran_canny.png", out);
    
}
