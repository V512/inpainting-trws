#ifndef NEIGHBORSTRUCT_H
#define NEIGHBORSTRUCT_H

#include <opencv2/opencv.hpp>

struct NeighborStruct {
private:
    const int mN = 4;
    cv::Size mImageSize;

public:
    NeighborStruct(cv::Size imageSize);

    void print(int i, int j);

    int getN() const;

    void getNeighborIndexes(const int i, const int j, const int n, int &nI, int &nJ) const;

    int static getIndex(int i, int j, int nI, int nJ);

    bool isAllowedNeighbor(int i, int j) const;

    int getNT(int i, int j) const;

};

#endif