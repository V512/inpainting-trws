#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "timeMeasurement.h"
#include "neighborStruct.h"
#include "trwsInpainting.h"

int main() {
    TimeMeasurement time;
    cv::Mat image = cv::imread("C:\\vlad\\Development\\TRWS\\inpainting\\mona-lisa-damaged.png", 1);
    // cv::Mat image = cv::Mat(cv::Size(2, 2), CV_8UC1);
    // image.at<uint8_t>(0, 0) = 0;
    // image.at<uint8_t>(0, 1) = 128;
    // image.at<uint8_t>(1, 0) = 64;
    // image.at<uint8_t>(1, 1) = 255;
    // cv::resize(image, image, cv::Size(20, 20));
    // cv::imshow("image", image);
    // cv::waitKey(0);
    inpainting(image, 5);
    return 0;
}


