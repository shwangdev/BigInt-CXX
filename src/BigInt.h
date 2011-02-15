#ifndef BIGINT_H
#define BIGINT_H


#define BASE 10000
#define MAX_DIG 9999
#define BASE_DIG 4



#include <iostream>
#include "general.h"

class BigInt {
public:
	ulong Size, SizeMax;
	ushort *Coef;

	BigInt();
	BigInt(ulong);
	BigInt(const BigInt&);
	BigInt(const char *str);
	virtual ~BigInt();

	BigInt operator *(const BigInt &B) const;
	BigInt mulbase(const ushort B) const;
	void selfmulbase(const ushort B);
	BigInt operator +(const BigInt &B) const;
       	BigInt operator -(const BigInt &B) const;
	BigInt operator /(BigInt &B) const;
	BigInt operator %(BigInt &B) const;
	bool operator==(const BigInt &B) const;
	bool operator<(const BigInt &B) const;
	void divbase(const ushort B, BigInt &Q, ushort &R) const;
	void div(BigInt &B, BigInt &Q, BigInt &R) const;
	//	BigInt &operator /(const ushort B) const;
	void zero();

	BigInt& operator=(const BigInt&);
	operator long();
};

void Add(const BigInt&, const BigInt&, BigInt&);
int  Sub(const BigInt&, const BigInt&, BigInt&);
void Div(const BigInt&, BigInt&, BigInt&, BigInt&);
void SMul(const BigInt&, const ushort B, BigInt&);
void Mul(const BigInt&, const BigInt&, BigInt&);
void SDiv(const BigInt &A, const ushort B, BigInt &Q, ushort &R);
void Div(const BigInt &A, const BigInt &V, BigInt &Q, BigInt &R);
ostream& operator<<(ostream& os, const BigInt& A);

#endif

