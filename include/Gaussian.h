#pragma once


#include <matrix.h>


class GaussianFilter : public Matrix
{
public:
    GaussianFilter(double sigma);
    double scaleFactor;

};

