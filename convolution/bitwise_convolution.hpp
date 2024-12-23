#include "generalized_convolution.hpp"
#include "transform.hpp"

template <typename T>
vector<T> bitwise_and_convolution(vector<T> a, vector<T> b) {
	return generalized_convolution<T, transform::fzt_sup<T>,
								   transform::ifzt_sup<T>>(a, b);
}

template <typename T>
vector<T> bitwise_or_convolution(vector<T> a, vector<T> b) {
	return generalized_convolution<T, transform::fzt_sub<T>,
								   transform::ifzt_sub<T>>(a, b);
}

template <typename T>
vector<T> bitwise_xor_convolution(vector<T> a, vector<T> b) {
	return generalized_convolution<T, transform::fwt<T>, transform::ifwt<T>>(a,
																			 b);
}