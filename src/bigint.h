#ifndef BIGINT_H
#define BIGINT_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#define BASE unsigned char
#define DOUBLEBASE unsigned short

const int yet=sizeof(BASE)*8;
const DOUBLEBASE pz=1<<yet;

class bigint
{
	private:
	BASE *pLow, *pHigh, *pAll;//*pLow - младший  разряд, *pHigh - старший значимый разряд, *pAll - самый последний разряд

	BASE* get_memory(int );
	void normalize();

	public:
	bigint();
	int Size() const;
	int SizeZ() const;
	bigint (int , int );
	bigint (const char* );
	bigint (unsigned int );
	void imul(BASE* , BASE* , BASE* , BASE* ,BASE*);
	BASE add(BASE* , BASE* , BASE* , BASE* ,BASE*);
	int sub(BASE * , BASE * , BASE * , BASE * , BASE * );
	void PrintHex();
	bigint(const bigint &);
	bigint operator = (const bigint &);
	~bigint();
	bigint operator + (const bigint &);
	bigint operator - (const bigint & );
	bigint operator *(const bigint & b);
	bigint div_BASE(BASE * ,BASE * ,BASE  ,BASE & );
	bigint div(bigint ,bigint ,bigint &);
	bigint operator / (bigint &);
	bool operator == (const bigint &) const;
	bigint operator % ( bigint &);
	bool operator < (const bigint & ) const;
	bool operator <= (const bigint & ) const;
	bool operator > (const bigint & ) const;
	bool operator >= (const bigint & ) const;
	bool operator != (const bigint & ) const;

};

#endif


