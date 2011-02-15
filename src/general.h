#if 0

	Функции и объявления общего назначения.

#endif



#ifndef GENERAL_H_INCLUDED_
#define GENERAL_H_INCLUDED_

#include <ctime>

using namespace std;

#define CACHE_HALF 65536

#define CONST_PI      3.1415926535897932384626433832
#define CONST_SQRT_2  0.7071067811865475244008443621
#define CONST_SQRT2   1.4142135623730950488016887242

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

typedef double real;
typedef unsigned long ulong;
typedef unsigned short ushort;

void error(const char error_text[]);

template<class T>
inline void swap(T&, T&);

clock_t finish();
void start();

#endif
