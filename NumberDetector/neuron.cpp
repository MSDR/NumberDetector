#include "neuron.h"


double Neuron::feedForward(std::vector<double> inputs) {
	//sigmoid(inputs•weights_ + bias)
	//The dot product weights the inputs.
	//Bias, well, biases that result.
	//Sigmoid is the activation function, clamping results on (-1,1)
	return sigmoid(std::inner_product(weights_.begin(), weights_.end(), inputs.begin(), 0.0) + bias_);
}
