#include "matrix.h"

#include "Gaussian.h"
#include "sobelFilter.h"
#include "Image.h"


int main()
{
    auto img = CV::Utils::Image::loadPNG("buran.png");
    return 0;
}