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
double dotProduct(matrix m1, matrix m2) {
	if (m1.size() != m2.size() || m1.size() == 0 || m2.size() == 0 || m1[0].size() != m2[0].size()) return 0;
	double sum = 0.0;
	for (int i = 0; i < m1.size(); ++i) {
		for (int j = 0; j < m1[i].size(); ++j) {
			sum += m1[i][j] * m2[i][j];
		}
	}
	return sum;
}

std::vector<matrix> Convolution::forward(matrix canvas) {
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

CNN::CNN(int numConvFilters) {
	conv_ = Convolution(numConvFilters);
	conv_.printFilters();
}


CNN::~CNN() {
}

double* CNN::forward(matrix canvas) {
	std::vector<matrix> out = conv_.forward(canvas);

	std::cout << "After convolution:";
	for (int v = 0; v < out.size(); ++v) {
		std::cout << "\n_____________________________";
		for (int i = 0; i < 26; ++i) {
			std::cout << std::endl;
			for (int j = 0; j < 26; ++j) {
				std::cout << std::to_string(out[v][i][j]) << (out[v][i][j] > 0 ? "  " : " ");
			}
		}
	}
	std::cout << "\n_____________________________\n";
	return nullptr;
}
