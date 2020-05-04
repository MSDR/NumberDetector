#include "cnn.h"

Convolution::Convolution(int numFilters)  : numFilters_(numFilters) {
	std::default_random_engine gen;
	std::normal_distribution<double> dist(0, 1);
	
	//Generate numFilters_ 3x3 filters from a Gaussian Distribution
	for (int i = 0; i < numFilters_; ++i) {
		matrix filter;
		for (int i = 0; i < 3; ++i) {
			filter.push_back(std::vector<double>());
			for (int j = 0; j < 3; ++j) {
				filter[i].push_back(dist(gen)/9);
			}
		}
		filters_.push_back(filter);
	}
}

void Convolution::printFilters() {
	std::cout << "Convolution filters:";
	for (int v = 0; v < numFilters_; ++v) {
		std::cout << "\n_____________________________";
		for (int i = 0; i < 3; ++i) {
			std::cout << std::endl;
			for (int j = 0; j < 3; ++j) {
				std::cout << std::to_string(filters_[v][i][j]) << (filters_[v][i][j] > 0 ? "  " : " ");
			}
		}
	}
	std::cout << "\n_____________________________\n";
}

//Performs an element-wise multiplication of matrices m1, m2, and sums the products.
//rejects if dim(m1) != dim(m2)
double dotProduct(matrix &m1, matrix &m2) {
	if (m1.size() != m2.size() || m1.size() == 0 || m2.size() == 0 || m1[0].size() != m2[0].size()) return 0;
	double sum = 0.0;
	for (int i = 0; i < m1.size(); ++i) {
		for (int j = 0; j < m1[i].size(); ++j) {
			sum += m1[i][j] * m2[i][j];
		}
	}
	return sum;
}

std::vector<matrix> Convolution::forward(matrix &canvas) {
	std::vector<matrix> convases;
	for(int f = 0; f < numFilters_; ++f) {
		matrix convas;
		for (int i = 0; i < 26; ++i) {
			convas.push_back(std::vector<double>());
			for (int j = 0; j < 26; ++j) {
				double sum = 0.0;
				for (int im = 0; im < 3; ++im) {
					for (int jm = 0; jm < 3; ++jm) {
						sum += canvas[i+im][j+jm] * filters_[f][im][jm];
					}
				}
				convas[i].push_back(sum);
			}
		}
		convases.push_back(convas);
	}
	return convases;
}

std::vector<matrix> CNN::poolMax(std::vector<matrix> &input) {
	std::vector<matrix> pools;
	for(int f = 0; f < input.size(); ++f) {
		matrix pool;
		for (int i = 0; i < 13; ++i) {
			pool.push_back(std::vector<double>());
			for (int j = 0; j < 13; ++j) {
				double maxInPool = -1.0;
				for (int im = 0; im < 2; ++im) {
					//std::cout << std::endl;
					for (int jm = 0; jm < 2; ++jm) {
						//std::cout << input[f][i * 2 + im][j * 2 + jm] << " ";
						maxInPool = maxInPool > input[f][i*2+im][j*2+jm] ? maxInPool : input[f][i*2+im][j*2+jm] ;
					}
				}
				pool[i].push_back(maxInPool);
			}
		}
		pools.push_back(pool);
	}
	return pools;
}


CNN::CNN(int numConvFilters) {
	conv_ = Convolution(numConvFilters);
	sMax_ = SoftMax(10);
	//conv_.printFilters();
}


CNN::~CNN() {
}

double* CNN::forward(matrix &canvas) {
	std::vector<matrix> conv = conv_.forward(canvas);
	
	std::cout << "After convolution:";
	for (int v = 0; v < conv.size(); ++v) {
		std::cout << "\n_____________________________";
		for (int i = 0; i < 26; ++i) {
			std::cout << std::endl;
			for (int j = 0; j < 26; ++j) {
				std::cout << (conv[v][i][j] > 0 ? "  " : " ") << std::to_string(conv[v][i][j]);
			}
		}
	}
	std::cout << "\n_____________________________\n";
	
	std::vector<matrix> pool = poolMax(conv);
	
	std::cout << "After pooling:";
	for (int v = 0; v < pool.size(); ++v) {
		std::cout << "\n_____________________________";
		for (int i = 0; i < 13; ++i) {
			std::cout << std::endl;
			for (int j = 0; j < 13; ++j) {
				std::cout << (pool[v][i][j] > 0 ? "  " : " ") << std::to_string(pool[v][i][j]);
			}
		}
	}
	std::cout << "\n_____________________________\n";
	

	std::vector<double> out = sMax_.forward(pool);

	std::cout << "After softmax: ";
	for (int i = 0; i < out.size(); ++i)
		std::cout << out[i] << " ";
	std::cout << std::endl;
	return nullptr;
}

SoftMax::SoftMax(int numOut) {
	std::default_random_engine gen;
	std::normal_distribution<double> dist(0, 1);

	//Initialize 1352x10 weights_ with random doubles along a standard gaussian distribution
	for (int i = 0; i < (13*13*8); ++i) {
		weights_.push_back(std::vector<double>());
		for (int j = 0; j < numOut; ++j) {
			weights_[i].push_back(dist(gen)/(13*13*8));
		}
	}

	//Initialize 1x10 biases_ with 0's
	for (int i = 0; i < numOut; ++i) {
		biases_.push_back(0);
	}
}

std::vector<double> SoftMax::forward(std::vector<matrix> input) {
	//Flatten input
	std::vector<double> flatInput;
	for (int v = 0; v < input.size(); ++v) {
		for (int i = 0; i < input[v].size(); ++i) {
			for (int j = 0; j < input[v][i].size(); ++j) {
				flatInput.push_back(input[v][i][j]);
			}
		}
	}

	std::vector<double> out;
	double outSum = 0.0;
	for (int i = 0; i < weights_[0].size(); ++i) {
		double sum = 0.0;
		for (int j = 0; j < weights_.size(); ++j) {
			sum += flatInput[j] * weights_[j][i]; //(1x1352 flatInput)x(1352x10 weights_)
		}
		sum += biases_[i]; //add biases_
		sum = std::exp(sum);
		out.push_back(sum);
		outSum += sum;
	}
	//apply softmax formula: https://victorzhou.com/blog/softmax/
	for (int i = 0; i < out.size(); ++i) {
		out[i] = out[i] / outSum;
	}
	return out;
}
