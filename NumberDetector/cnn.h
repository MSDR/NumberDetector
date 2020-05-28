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
	void backProp(std::vector<matrix> dL_dPool, double learnRate);

	void printFilters();
private:
	int numFilters_;
	std::vector<matrix> filters_; //Each filter is 3x3
	matrix inputCache_;
};

class Pool {
public: 
	Pool() {};
	std::vector<matrix> forward(std::vector<matrix> &input);
	std::vector<matrix> backProp(std::vector<matrix> dL_dSMax);
private:
	std::vector<matrix> inputCache_;
};

class SoftMax {
public:
	SoftMax(int numOut = 10);

	std::vector<double> forward(std::vector<matrix> &input);
	std::vector<matrix> backProp(std::vector<double> &dL_dOut, double learnRate);
private:
	matrix weights_;
	std::vector<double> biases_;

	std::vector<double> inputCache_;
	std::vector<double> totalsCache_;
	double totalSumCache_;
};

class CNN {
public:
	CNN(int numConvFilters = 8, int numSMaxNodes = 10);

	std::vector<double> forward(matrix &canvas);
	void backProp(std::vector<double> &dl_dOut, double learnRate = 0.005, bool stop = false); 

	bool maxTrained_; //Guards against filters becoming nan
private:
	void centerDrawing(matrix &canvas, matrix* out);
	Convolution conv_;
	Pool pool_;
	SoftMax sMax_;

};

#endif