#ifndef TRWSINPAITING_H
#define  TRWSINPAITING_H

#include <opencv2/opencv.hpp>
#include "neighborStruct.h"
#include "phi.h"
#include "chainsWeight.h"


struct NodeCostTable {
public:
    NodeCostTable(cv::Mat &image, std::vector<uint8_t> labels, uint8_t eps);

    float& operator()(int i, int j, int k);

    ~NodeCostTable() = default;
   
private:
    int width, height, ks;
    uint8_t eps;
    std::vector<float> costs;
};

struct EdgeCostTable {
public:
    EdgeCostTable(std::vector<uint8_t> labels, float alpha);

    float& operator()(int k1, int k2);

private:
    std::vector<float> costs;
    int ks;
};


void inpainting(cv::Mat &image, int iters);

// void subchainMaxCostUpdate(
//     ChainsWeight &cw, 
//     EdgeCostTable &edgeCostTable,
//     NodeCostTable &nodeCostTable,
//     Phi &phi,
//     int i, 
//     int j, 
//     int k,
//     int chain, 
//     int ks,
//     cv::Size &imageSize
//     );

std::vector<uint8_t> linspace(uint8_t a, uint8_t b, uint8_t intervals);

cv::Mat singleChannelInpainting(
    cv::Mat &image, 
    int iters, 
    std::vector<uint8_t> &labels, 
    float alpha,
    uint8_t eps
    );

void updateL(ChainsWeight &cw, 
    EdgeCostTable &edgeCostTable,
    NodeCostTable &nodeCostTable,
    Phi &phi,
    int i, 
    int j, 
    int k,
    int ks);

void updateU(ChainsWeight &cw, 
    EdgeCostTable &edgeCostTable,
    NodeCostTable &nodeCostTable,
    Phi &phi,
    int i, 
    int j, 
    int k,
    int ks);

void updateR(ChainsWeight &cw, 
    EdgeCostTable &edgeCostTable,
    NodeCostTable &nodeCostTable,
    Phi &phi,
    int i, 
    int j, 
    int k,
    int ks);

void updateD(ChainsWeight &cw, 
    EdgeCostTable &edgeCostTable,
    NodeCostTable &nodeCostTable,
    Phi &phi,
    int i, 
    int j, 
    int k,
    int ks);

#endif