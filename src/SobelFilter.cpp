#include "sobelFilter.h"

CV::Canny::SobelFilter::SobelFilter(CV::Canny::FilterType FilterType) : Matrix(3, 3)
{
    if (FilterType == CV::Canny::FilterType::Y)
    {
        mat[0][0] = -1;
        mat[0][1] = -2;
        mat[0][2] = -1;
        mat[1][0] = 0;
        mat[1][1] = 0;
        mat[1][2] = 0;
        mat[2][0] = 1;
        mat[2][1] = 2;
        mat[2][2] = 1;
    }
    else if (FilterType == CV::Canny::FilterType::X)
    {
        mat[0][0] = -1;
        mat[0][1] = 0;
        mat[0][2] = 1;
        mat[1][0] = -2;
        mat[1][1] = 0;
        mat[1][2] = 2;
        mat[2][0] = -1;
        mat[2][1] = 0;
        mat[2][2] = 1;
    }
}