#include "chainsWeight.h"

ChainsWeight::ChainsWeight(int width, int height, int ks) {
    this->width = width;
    this->height = height;
    this->ks = ks;
    // this->directions = 4;
    // this->costs = std::vector<float>(directions * width * height * ks, 0.0);
    this->costs = std::vector<float>(width * height * ks, 0.0);
}


// float& ChainsWeight::operator()(int i, int j, int k, int direction) {
float& ChainsWeight::operator()(int i, int j, int k) {
    // return this->costs[k + direction * ks + ks * directions * j + i * width * directions * ks];
    return this->costs[k + j * ks + i * ks * width];
}