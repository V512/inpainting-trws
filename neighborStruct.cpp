#include "neighborStruct.h"

void NeighborStruct::print(int i, int j) {
    int nI, nJ;
    for(int n = 0; n < mN; n++) {
        getNeighborIndexes(i, j, n, nI, nJ);
        printf("(%d, %d) ", nI, nJ);
        printf("is allow %d ", isAllowedNeighbor(nI, nJ));
        printf("n index %d \n", getIndex(i, j, nI, nJ));
    }
    printf("Nt = %d\n", getNT(i, j));
}

int NeighborStruct::getN() const {
    return mN;
}

void NeighborStruct::getNeighborIndexes(const int i, const int j, const int n, int &nI, int &nJ) const {
    if(n == 0) {
        nI = i;
        nJ = j + 1;
        return;
    }
    if(n == 1) {
        nI = i + 1;
        nJ = j;
        return;
    }
    if(n == 2) {
        nI = i;
        nJ = j - 1;
        return;
    }
    if(n == 3) {
        nI = i - 1;
        nJ = j;
        return;
    }
}

int NeighborStruct::getIndex(int i, int j, int nI, int nJ) {
    if ((i == nI) && (j - 1 == nJ)) {
        return 0;
    }
    if ((i - 1 == nI) && (j == nJ)) {
        return 1;
    }
    if ((i == nI) && (j + 1 == nJ)) {
        return 2;
    }
    if((i + 1 == nI) && (j == nJ)) {
        return 3;
    }
    return -1;
}

bool NeighborStruct::isAllowedNeighbor(int i, int j) const {
    if ((i >= 0) && (i < mImageSize.height) && (j >= 0) && (j < mImageSize.width)) {
        return true;
    }
    return false; 
} 

int NeighborStruct::getNT(int i, int j) const {
    if(    ((i == 0) && (j == 0)) 
        || ((i == mImageSize.height) && (j == 0)) 
        || ((i == 0) && (j == mImageSize.width)) 
        || ((i == mImageSize.height) && (j == mImageSize.width))) {
        return 2;
    }
    if((i == 0) || (j == 0) || (i == mImageSize.height) || (j == mImageSize.width)) {
        return 3;
    } 
    return 4;
}

NeighborStruct::NeighborStruct(cv::Size imageSize) {
    mImageSize = imageSize;
}