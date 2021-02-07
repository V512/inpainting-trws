#ifndef CHAINSWEIGHT_H
#define CHAINSWEIGHT_H

#include <vector>
enum {
    L, U, R, D
};

struct ChainsWeight {
public:
    ChainsWeight(int width, int height, int ks);

    // float& operator()(int i, int j, int k, int direction);
    float& operator()(int i, int j, int k);

    ~ChainsWeight() = default;

private:
    std::vector<float> costs;
    // int ks, height, width, directions;
    int ks, height, width;
};

#endif 