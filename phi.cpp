#include "phi.h"

Phi::Phi(int ks, int width, int height) {
    this->ks = ks;
    this->width = width;
    this->height = height;
    this->phi = std::vector<float>(ks * width * height, 0.0);
    // for(int i = 0; i < phi.size(); i++) {
    //     this->phi[i] = 0.0;
    // }
}

float& Phi::operator()(int k, int i, int j) {
    return this->phi[k + j * ks + i * width * ks];
}