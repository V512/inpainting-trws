#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "timeMeasurement.h"
#include "neighborStruct.h"
#include "trwsInpainting.h"

int main(int argc, char* argv[]) {
    std::string imagePath = argv[1];
    printf("Image path %s\n", imagePath.c_str());
    cv::Mat image = cv::imread(imagePath, 1);

    if(image.empty()) {
        printf("Image is null\n");
        return -1;
    }

    inpainting(image, 5);
    return 0;
}


