#include "generalized_convolution.hpp"
#include "transform.hpp"

template <typename T>
vector<T> gcd_convolution(vector<T> a, vector<T> b) {
	return generalized_convolution<T, transform::mul_zeta<T>,
								   transform::mul_mobius<T>>(a, b);
}

template <typename T>
vector<T> lcm_convolution(vector<T> a, vector<T> b) {
	return generalized_convolution<T, transform::div_zeta<T>,
								   transform::div_mobius<T>>(a, b);
}