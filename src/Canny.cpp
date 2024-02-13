#include "Image.h"
#include <math.h>
#include <Canny.h>


using namespace CV::Detection;

void Canny::calculateGradient(const unsigned char* image, int width, int height, std::vector<float>& gradientMagnitude, std::vector<float>& gradientAngle) {
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
        
            float dx = (image[(y + 1) * width + x] - image[(y - 1) * width + x]) / 2.0f;
            float dy = (image[y * width + x + 1] - image[y * width + x - 1]) / 2.0f;

            // Calculate magnitude and angle
            gradientMagnitude[y * width + x] = sqrt(dx * dx + dy * dy);
            gradientAngle[y * width + x] = atan2(dy, dx);
        }
    }
}

void Canny::nonMaxSuppression(const std::vector<float>& gradientMagnitude, const std::vector<float>& gradientAngle, int width, int height, unsigned char* output) {
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float angle = gradientAngle[y * width + x];
            int q, r;

            // Quantize the angle
            if (angle >= -M_PI_4 && angle < M_PI_4) {
                q = gradientMagnitude[y * width + x + 1];
                r = gradientMagnitude[y * width + x - 1];
            } else if (angle >= M_PI_4 && angle < 3 * M_PI_4) {
                q = gradientMagnitude[(y + 1) * width + x - 1];
                r = gradientMagnitude[(y - 1) * width + x + 1];
            } else if (angle >= -3 * M_PI_4 && angle < -M_PI_4) {
                q = gradientMagnitude[(y - 1) * width + x - 1];
                r = gradientMagnitude[(y + 1) * width + x + 1];
            } else {
                q = gradientMagnitude[(y + 1) * width + x];
                r = gradientMagnitude[(y - 1) * width + x];
            }

            // Perform non-maximum suppression
            if (gradientMagnitude[y * width + x] >= q && gradientMagnitude[y * width + x] >= r)
                output[y * width + x] = static_cast<unsigned char>(gradientMagnitude[y * width + x]);
            else
                output[y * width + x] = 0;
        }
    }
}


void Canny::cannyEdgeDetector(const std::shared_ptr<CV::Utils::Image> input, std::shared_ptr<CV::Utils::Image>& output)
{
    output = std::make_shared<CV::Utils::Image>();
    output->width = input->width;
    output->height = input->height;
    output->channels = 1;
    output->type = CV::Utils::ImageFormat::GRAY;
    output->data = new unsigned char[input->width * input->height];

    std::vector<float> gradientMagnitude(input->width * input->height);
    std::vector<float> gradientAngle(input->width * input->height);

    calculateGradient(input->data, input->width, input->height, gradientMagnitude, gradientAngle);

    nonMaxSuppression(gradientMagnitude, gradientAngle, input->width, input->height, output->data);
}
