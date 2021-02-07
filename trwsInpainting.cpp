#include <limits>
#include <math.h>
#include "trwsInpainting.h"
#include "timeMeasurement.h"

NodeCostTable::NodeCostTable(cv::Mat &image, std::vector<uint8_t> labels, uint8_t eps) {
    this->ks = static_cast<int>(labels.size());
    this->eps = eps;
    this->width = image.size().width;
    this->height = image.size().height;
    this->costs = std::vector<float>(width * height * ks);
    float temp1, temp2;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            temp1 = image.at<uint8_t>(i, j) != eps ? 1.0 : 0.0;
            for(int k = 0; k < ks; k++) {
                temp2 = static_cast<float>(abs((float)image.at<uint8_t>(i, j) - (float)labels[k]));
                this->costs[k + j * ks + i * width * ks] = -temp1 * temp2;
            }
        }
    }
}

float& NodeCostTable::operator()(int i, int j, int k) {
    return costs[k + j * ks + i * width * ks];
}


EdgeCostTable::EdgeCostTable(std::vector<uint8_t> labels, float alpha) {
    this->costs = std::vector<float>(labels.size() * labels.size());
    this->ks = static_cast<int>(labels.size());
    for(int k1 = 0; k1 < ks; k1++) {
        for(int k2 = 0; k2 < ks; k2++) {
            costs[k1 * ks + k2] = -alpha 
            // * static_cast<float>(cv::norm(cv::Vec<uint8_t, 1>(labels[k1]), cv::Vec<uint8_t, 1>(labels[k2]), cv::NORM_L1));
            * static_cast<float>(abs((float)labels[k1] - (float)labels[k2]));
        }
    }
}

float& EdgeCostTable::operator()(int k1, int k2) {
    return this->costs[k1 * ks + k2];
}


void inpainting(cv::Mat &image, int iters) {
    cv::Mat bgr[3];
    cv::split(image, bgr);
    std::vector<uint8_t> labels = linspace(0, 255, 32);
    cv::Mat rImage = singleChannelInpainting(bgr[2], iters, labels, 0.5, 0);
    cv::Mat gImage = singleChannelInpainting(bgr[1], iters, labels, 0.5, 0);
    cv::Mat bImage = singleChannelInpainting(bgr[0], iters, labels, 0.5, 0);
    cv::imshow("B", bImage);
    cv::imshow("G", gImage);
    cv::imshow("R", rImage);
    cv::Mat restoredImage;
    cv::Mat grayImages[] = {bImage, gImage, rImage};
    cv::merge(grayImages, 3, restoredImage);
    cv::imshow("Restored image", restoredImage);
    cv::waitKey(0);
}

cv::Mat singleChannelInpainting(
    cv::Mat &image, 
    int iters, 
    std::vector<uint8_t> &labels, 
    float alpha,
    uint8_t eps
    ) {
    TimeMeasurement time;
    cv::Size imageSize = image.size();
    int height = imageSize.height;
    int width = imageSize.width;
    int ks = static_cast<int>(labels.size());
    Phi phi(ks, width, height);
    time.updateTime();
    EdgeCostTable edges(labels, alpha);
    printf("Edges time %lu\n", time.getTime());
    time.updateTime();
    NodeCostTable nodes(image, labels, eps);
    printf("Nodes time %lu\n", time.getTime());
    ChainsWeight cwL(width, height, ks);
    ChainsWeight cwU(width, height, ks);
    ChainsWeight cwR(width, height, ks);
    ChainsWeight cwD(width, height, ks);
    printf("Iter %d\n", 0);
    for(int i = 1; i < height; ++i) {
            for(int j = 1; j < width; ++j) {
                for(int k = 0; k < ks; ++k) {
                    updateL(cwL, edges, nodes, phi, i, j, k, ks);
                    updateU(cwU, edges, nodes, phi, i, j, k, ks);
                } 
            }
        }
        for(int i = height - 2; i >= 0; --i) {
            for(int j = width - 2; j >= 0; --j) {
                for(int k = 0; k < ks; ++k) {
                    updateD(cwD, edges, nodes, phi, i, j, k, ks);
                    updateR(cwR, edges, nodes, phi, i, j, k, ks);
                }
            }
        }

    for(int iter = 1; iter < iters; ++iter) {
        time.updateTime();
        for(int i = 1; i < height; ++i) {
            for(int j = 1; j < width; ++j) {
                for(int k = 0; k < ks; ++k) {
                    updateL(cwL, edges, nodes, phi, i, j, k, ks);
                    updateU(cwU, edges, nodes, phi, i, j, k, ks);
                        phi(k, i, j) = (cwL(i, j, k) + cwR(i, j, k) - cwU(i, j, k) - cwD(i, j, k)) 
                        * 0.5f;
                } 
            }
        }
        for(int i = height - 2; i >= 0; --i) {
            for(int j = width - 2; j >= 0; --j) {
                for(int k = 0; k < ks; ++k) {
                    updateD(cwD, edges, nodes, phi, i, j, k, ks);
                    updateR(cwR, edges, nodes, phi, i, j, k, ks);
                    phi(k, i, j) = (cwL(i, j, k) + cwR(i, j, k) - cwU(i, j, k) - cwD(i, j, k)) 
                        * 0.5f;
                }
            }
        }
        printf("iter %d time %d\n", iter, time.getTime());
    }
    time.updateTime();
    cv::Mat restoredGSImage = cv::Mat::zeros(imageSize, CV_8UC1);
    float maxCost;
    float cost;
    int kBest = 0;
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            maxCost = std::numeric_limits<float>::lowest();
            for(int k = 0; k < ks; ++k) {
                cost = cwL(i, j, k) + cwR(i, j, k) + 0.5f * nodes(i, j, k) - phi(k, i, j);
                if(cost > maxCost) {
                    maxCost = cost;
                    kBest = k;
                }
            }
            restoredGSImage.at<uint8_t>(i, j) = labels[kBest];
        }
    }
    printf("GS time %lu\n", time.getTime());
    return restoredGSImage;
}

void updateR(ChainsWeight &cwR, 
    EdgeCostTable &edgeCostTable,
    NodeCostTable &nodeCostTable,
    Phi &phi,
    int i, 
    int j, 
    int k,
    int ks) {
        float maxCost = std::numeric_limits<float>::lowest();
        float cost = 0.0;
        for(int k2 = 0; k2 < ks; k2++) {
            cost = cwR(i, j + 1, k2) + 0.5f * nodeCostTable(i, j + 1, k2) 
            - phi(k2, i, j + 1) + edgeCostTable(k2, k); 
            if(cost > maxCost) {
                maxCost = cost;
            }
        }
        cwR(i, j, k) = maxCost;      
    }

void updateL(ChainsWeight &cwL, 
EdgeCostTable &edgeCostTable,
NodeCostTable &nodeCostTable,
Phi &phi,
int i, 
int j, 
int k,
int ks) {
    float maxCost = std::numeric_limits<float>::lowest();
    float cost; 
    for(int k2 = 0; k2 < ks; k2++) { 
        cost = cwL(i, j - 1, k) + 0.5f * nodeCostTable(i, j - 1, k2) 
        - phi(k2, i, j - 1) + edgeCostTable(k2, k);
        if(cost > maxCost) {
            maxCost = cost;
        }
    }
    cwL(i, j, k) = maxCost;
}

void updateU(
    ChainsWeight &cwU, 
    EdgeCostTable &edgeCostTable,
    NodeCostTable &nodeCostTable,
    Phi &phi,
    int i, 
    int j, 
    int k,
    int ks) {
    float maxCost = std::numeric_limits<float>::lowest();
    float cost;
    for(int k2 = 0; k2 < ks; k2++) {
        cost = cwU(i - 1, j, k2) + 0.5f * nodeCostTable(i - 1, j, k2) 
        + phi(k2, i - 1, j) + edgeCostTable(k2, k);
        if(cost > maxCost) {
            maxCost = cost;
        }
    }
    cwU(i, j, k) = maxCost;
}

void updateD(ChainsWeight &cwD, 
EdgeCostTable &edgeCostTable,
NodeCostTable &nodeCostTable,
Phi &phi,
int i, 
int j, 
int k,
int ks) {
    float maxCost = std::numeric_limits<float>::lowest();
    float cost;
    for(int k2 = 0; k2 < ks; k2++) {
        cost = cwD(i + 1, j, k2) + 0.5f * nodeCostTable(i + 1, j, k2) 
        + phi(k2, i + 1, j) + edgeCostTable(k2, k);
        if(cost > maxCost) {
            maxCost = cost;
        }
    }
    cwD(i, j, k) = maxCost;
}

std::vector<uint8_t> linspace(uint8_t a, uint8_t b, uint8_t intervals) {
    std::vector<uint8_t> linspace(intervals);
    float step = (b - a) / static_cast<float>(intervals - 1);
    linspace[0] = a;
    for(int i = 1; i < intervals; i++) {
        linspace[i] =  static_cast<uint8_t>(i * step);
    }
    return linspace;
}
