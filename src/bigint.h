//      Redistribution and use in source and binary forms, with or without
//      modification, are permitted provided that the following conditions are
//      met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following disclaimer
//        in the documentation and/or other materials provided with the
//        distribution.
//      * Neither the name of the  nor the names of its
//        contributors may be used to endorse or promote products derived from
//        this software without specific prior written permission.
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//		for more info see License

#ifndef BIGINT_H
#define BIGINT_H



#define BASE unsigned char
#define BASE2 unsigned short

const int yet=sizeof(BASE)*8;
const BASE2 pz=1<<yet;

class BigInt
{
private:
	BASE *pLow, *pHigh, *pAll;

	BASE* get_memory(int );
	void normalize();

public:
	BigInt();
	int Size() const;
	int SizeZ() const;
	BigInt (int , int );
	BigInt (const char* );
	BigInt (unsigned int );
	BigInt(const BigInt &);

	void imul(BASE* , BASE* , BASE* , BASE* ,BASE*);
	BASE add(BASE* , BASE* , BASE* , BASE* ,BASE*);
	int sub(BASE * , BASE * , BASE * , BASE * , BASE * );
	BigInt div_BASE(BASE * ,BASE * ,BASE  ,BASE & );
	BigInt div(BigInt ,BigInt ,BigInt &);

	void PrintHex();
	void PrintDec();


	BigInt operator = (const BigInt &);
	~BigInt();


	/* Arithmetics */

	BigInt operator + (const BigInt &);
	BigInt operator - (const BigInt & );
	BigInt operator *(const BigInt & b);
	BigInt operator / (BigInt &);
	BigInt operator % ( BigInt &);

	/* Comparsion */

	bool operator == (const BigInt &) const;
	bool operator < (const BigInt & ) const;
	bool operator <= (const BigInt & ) const;
	bool operator > (const BigInt & ) const;
	bool operator >= (const BigInt & ) const;
	bool operator != (const BigInt & ) const;

};

#endif


