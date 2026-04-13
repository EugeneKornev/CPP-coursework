#include <type_traits>
#include <iostream>

using true_type = std::integral_constant<bool, true>;
using false_type = std::integral_constant<bool, false>;


template <int N, int M> 
struct NModM : std::integral_constant<bool, std::conditional_t<(N % M == 0), false_type, NModM<N, M - 1>>{}> {};

template<int N> struct NModM<N, 1> : true_type {};

template <int N>
struct isPrime : NModM<N, N - 1> {};

template <>
struct isPrime<1> : true_type {};

template <int N>
struct nextPrime : std::integral_constant<int, std::conditional_t<isPrime<N + 1>{}, std::integral_constant<int, N + 1>, nextPrime<N + 1>>{}> {};

template <int N>
struct Prime : std::integral_constant<int, nextPrime<Prime<N - 1>{}>{}> {};

template <>
struct Prime<0> : std::integral_constant<int, 1> {};


int main() {
	std::cout << Prime<12>{} << std::endl;
};