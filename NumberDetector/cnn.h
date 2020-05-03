#ifndef CNN_H_
#define CNN_H_

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "globals.h"
using globals::matrix;

class Convolution {
public:
	Convolution(int numFilters = 1);

	//Canvas is a 28x28 input
	//Returns a numFilters_x26x26 output
	std::vector<matrix> forward(matrix canvas);

	void printFilters();
private:
	int numFilters_;
	std::vector<matrix> filters_; //Each filter is 3x3
};

class CNN {
public:
	CNN(int numConvFilters = 1);
	~CNN();

	double* forward(matrix canvas);


private:
	Convolution conv_;
};

#endif