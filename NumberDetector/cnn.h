#ifndef CNN_H_
#define CNN_H_

#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "globals.h"
using globals::matrix;

class Convolution {
public:
	Convolution(int numFilters = 8);

	//Canvas is a 28x28 input
	//Returns a numFilters_x26x26 output
	std::vector<matrix> forward(matrix &canvas);

	void printFilters();
private:
	int numFilters_;
	std::vector<matrix> filters_; //Each filter is 3x3
};

class SoftMax {
public:
	SoftMax(int numOut = 10);

	std::vector<double> forward(std::vector<matrix> input);

private:
	matrix weights_;
	std::vector<double> biases_;
};

class CNN {
public:
	CNN(int numConvFilters = 1);
	~CNN();

	std::vector<double> forward(matrix &canvas);


private:
	Convolution conv_;
	SoftMax sMax_;
	std::vector<matrix> poolMax(std::vector<matrix> &input);
};

#endif