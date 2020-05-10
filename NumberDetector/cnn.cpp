#include "cnn.h"

//Performs an element-wise multiplication of matrices m1, m2, and sums the products.
//rejects if dim(m1) != dim(m2)
double dotProduct(matrix &a, matrix &b) {
	if (a.size() != b.size() || a.size() == 0 || b.size() == 0 || a[0].size() != b[0].size()) return 0;
	double sum = 0.0;
	for (int i = 0; i < a.size(); ++i) {
		for (int j = 0; j < a[i].size(); ++j) {
			sum += a[i][j] * b[i][j];
		}
	}
	return sum;
}

matrix matrixMultiplication(matrix &a, matrix &b) {
	matrix o;

	//a | m x n
	//b | n x p
	//o | m x p
	if (a.size() == 0 || b.size() == 0 || a[0].size() == 0 || b[0].size() == 0 || a[0].size() != b.size()) { //size check
		std::cout << "Matrix multiplication size misalignment\n";
		std::cout << a.size() << " " << a[0].size() << " " << b.size() << " " << b[0].size() << "\n";
		return o;
	}

	for (int m = 0; m < a.size(); ++m) {
		o.push_back(std::vector<double>());
		for (int p = 0; p < b[0].size(); ++p) {
			double sum = 0.0;
			for (int n = 0; n < a[0].size(); ++n) {
				sum += a[m][n] * b[n][p];
			}
			o[m].push_back(sum);
		}
	}

	return o;
}

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
	std::cout << "stop";
}

std::vector<double> SoftMax::forward(std::vector<matrix> &input) {
	inputCache_.clear();
	totalsCache_.clear();

	//Flatten input
	std::vector<double> flatInput;
	for (int v = 0; v < input.size(); ++v) {
		for (int i = 0; i < input[v].size(); ++i) {
			for (int j = 0; j < input[v][i].size(); ++j) {
				flatInput.push_back(input[v][i][j]);
			}
		}
	}

	inputCache_ = flatInput;

	std::vector<double> out;
	totalSumCache_ = 0.0;
	for (int i = 0; i < weights_[0].size(); ++i) {
		double total = 0.0;
		for (int j = 0; j < weights_.size(); ++j) {
			total += flatInput[j] * weights_[j][i]; //(1x1352 flatInput)x(1352x10 weights_)
		}
		total += biases_[i]; //add biases_
		total = std::exp(total);
		out.push_back(total);
		totalsCache_.push_back(total);
		totalSumCache_ += total;
	}
	//apply softmax formula: https://victorzhou.com/blog/softmax/
	for (int i = 0; i < out.size(); ++i) {
		out[i] = out[i] / totalSumCache_;
	}
	return out;
}

std::vector<double> SoftMax::backProp(std::vector<double>& dL_dOut, double learnRate) {
	for (int c = 0; c < dL_dOut.size(); ++c) {
		if (dL_dOut[c] == 0) //will result in no change if not on correct value
			continue;

		//Calculate intermediary gradients
		std::vector<double> dOut_dt;
		for (int k = 0; k < dL_dOut.size(); ++k) {
			if (k == c) {
				dOut_dt.push_back(
					(totalsCache_[c] * (totalSumCache_ - totalsCache_[c])) /
					(totalSumCache_*totalSumCache_)
				);
			} else {
				dOut_dt.push_back(
					-(totalsCache_[c]*totalsCache_[c]) / 
					 (totalSumCache_*totalSumCache_)
				);
			}
		}

		matrix dL_dt; //1x10
		dL_dt.push_back(std::vector<double>());
		for (int i = 0; i < dL_dOut.size(); ++i) {
			dL_dt[0].push_back(dL_dOut[i] * dOut_dt[i]);
		}

		matrix dt_dw; //1352x1
		for (int i = 0; i < inputCache_.size(); ++i)
			dt_dw.push_back(std::vector<double>({ inputCache_[i] }));

		//1352x10
		matrix dt_dInput = weights_;

		//Calculate final gradients
		matrix dL_dw = matrixMultiplication(dt_dw, dL_dt);
		std::vector<double> dL_db(dOut_dt); //dL_db = dOut_dt*dt_db = dOut_dt*1

		dL_dt.clear(); //10x1
		for (int i = 0; i < dL_dOut.size(); ++i) {
			dL_dt.push_back(std::vector<double>({dL_dOut[i] * dOut_dt[i]}));
		}
		std::vector<double> dL_dInput = matrixMultiplication(dt_dInput, dL_dt)[0];

		std::cout << "dL_dw: " << dL_dw.size() << "x" << dL_dw[0].size() << std::endl;
		std::cout << "dL_db: " << dL_db.size() << std::endl;
		std::cout << "dL_dInput: " << dL_dInput.size() << std::endl;

		//Update weights/biases, produce output

		//update weights
		for (int i = 0; i < weights_.size(); ++i) {
			for (int j = 0; j < weights_[i].size(); ++j) {
				weights_[i][j] -= learnRate * dL_dw[i][j];
			}
		}

		//update biases
		for (int i = 0; i < biases_.size(); ++i) {
			biases_[i] -= learnRate * dL_db[i];
		}

		//implement output
	}
	return std::vector<double>();
}


CNN::CNN(int numConvFilters, int numSMaxNodes) {
	conv_ = Convolution(numConvFilters);
	sMax_ = SoftMax(numSMaxNodes);
	//conv_.printFilters();
}

CNN::~CNN() {
}

std::vector<double> CNN::forward(matrix &canvas) {
	//Takes the canvas and performs convolution
	//28x28 -> 26x26x8
	std::vector<matrix> conv = conv_.forward(canvas);
	/*
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
	*/

	//Pools together the convolution's outputs via max value in a 2x2 area
	//26x26x8 -> 13x13x8
	std::vector<matrix> pool = poolMax(conv);
	/*
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
	*/

	//Performs softmax on the pooling layer's output
	//13x13x8 -> 10, each node representing the confidence the cnn has in what number the drawing is
	std::vector<double> out = sMax_.forward(pool);
	/*
	std::cout << "After softmax: ";
	for (int i = 0; i < out.size(); ++i)
		std::cout << out[i] << " ";
	std::cout << std::endl;
	*/
	return out;
}

void CNN::backProp(std::vector<double> &dL_dOut, double learnRate) {
	std::vector<double> dL_dPool = sMax_.backProp(dL_dOut, learnRate);
}
