#ifndef BIGINT_H
#define BIGINT_H

Class BigInt {

public:
	BigInt(const char * );
	BigInt(const BigInt & );
	BigInt (unsigned int );
	BigInt (int size, int type)
	BigInt(int );
	BigInt ();
	~BigInt();
	
	BigInt operator = (const BigInt & );
	
	/* Arithemtics */
	BigInt operator + (const BigInt );
	BigInt operator * (const BigInt & );
	BigInt operator - (const BigInt & );
	BigInt operator / (const BigInt & )
	BigInt operator % (const BigInt & ); 
	
	/* Comparation */
	bool BigInt operator == (const BigInt & ) const;
	bool BigInt operator < (const BigInt & ) const;
	bool BigInt operator <= (const BigInt & ) const;
	bool BigInt operator > (const BigInt & ) const;
	bool BigInt operator >= (const BigInt & ) const;
	bool BigInt operator != (const BigInt & ) const;
	

private:
    BASE * GetMem (int size);
    void FreeMem (void);
    int Size (void) const;
    int SizeZ (void) const;
    void Normalize (void);
    void PrintHex (const char * name = "");
    void PrintDec (const char * name = "");
    BASE add(BASE * , BASE * , BASE * , BASE * , BASE * );
    void mul(BASE * , BASE * , BASE * , BASE * , BASE * );
    int sub(BASE * , BASE * , BASE * , BASE * , BASE * );
    BigInt divb(const BASE , BASE & );
    BigInt div(BigInt u, BigInt v, BigInt & );
    BigInt divrm(BASE *, BASE *,BASE v,BASE & );
    

};

#endif
