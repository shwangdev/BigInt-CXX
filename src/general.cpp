#include "general.h"
#include <iostream>
#include <cstdlib>

clock_t	time_global;

void error(const char *error_text) {
	std::cout << "Run-time error...\n" << error_text << "\n";
	exit(0);
}

template<class T>
inline void swap(T& a, T& b) {
	T temp = a;
	b = a;
	a = temp;
}

void start() {
	time_global = clock();
}

clock_t finish() {
	return (clock() - time_global);
}

