#ifndef PHI_H
#define PHI_H

#include <vector>


struct Phi {
private:
    int ks;
    int width;
    int height;
    std::vector<float> phi;
    // float phi[32 * 480 * 252]{0};

public:
    Phi(int kSize, int width, int height);

    float& operator()(int k, int i, int j);

    ~Phi() = default;
};

#endif