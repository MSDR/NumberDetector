#ifndef NEURON_H_
#define NEURON_H_

#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

class Neuron
{
public:
	Neuron() : bias_(0), weights_({0}) {};
	Neuron(std::vector<double> weights, double bias) : bias_(bias), weights_(weights) {};

	double feedForward(std::vector<double> inputs);

private:
	double bias_;
	std::vector<double> weights_;

	double sigmoid(double x) { return 1 / (1 + exp(-x)); }
};

#endif // !NEURON_H_
