#include "BigInt.h"
#include "general.h"
#include <cmath>
#include <string.h>
#include <stdio.h>


BigInt::BigInt() {
	SizeMax = Size = 0;
	Coef = NULL;
}


BigInt::BigInt(ulong MaxLen) {
	Coef = new ushort[MaxLen];
	SizeMax = MaxLen;
}


BigInt::BigInt(const BigInt &A) {
	SizeMax = A.SizeMax;
	Size = A.Size;
	Coef = new ushort[SizeMax];
	for(ulong i=0; i<SizeMax; i++) Coef[i] = A.Coef[i];
}

BigInt::BigInt(const char *str){
  //printf("000\n");
  // printf("str = '%s'\n", str);
  int len = strlen(str);
  Coef = new ushort[len];
  SizeMax = len;

  zero();
  //printf("111 str = '%s'\n", str);
  for(int i = 0; i < len; i++){
    selfmulbase(10);
    //  std::cout << "after mul : " << *this << "\n";
    int carry = str[i] - '0';
    int j = 0, temp;
    while(carry){
      temp = Coef[j] + str[i] - '0';
      if (temp >= BASE) {
	Coef[j++] = temp - BASE;
	carry = 1;
      } else {
	Coef[j++] = temp;
	carry = 0;
      }
    }
    if(j > Size) Size++;
    // std::cout<< "d : " << *this << "\n";
  }
  //printf("222\n");
}

BigInt::~BigInt() {
	delete Coef;
}

void BigInt::zero() {
	for(ulong i=0; i<SizeMax; i++) Coef[i]=0;
	Size=1;
}    


BigInt::operator long() {
  long tmp=0;
  // warning: overflow possible !
  for(ushort i=0; i<Size; i++) tmp += Coef[i]*(long)pow( BASE, (real)i);
  return tmp;
}

bool BigInt::operator==(const BigInt &B) const {
  if(this->Size != B.Size) return false;
  for(int i = 0; i < Size; i++)
    if(Coef[i] != B.Coef[i])
      return false;
  return true;
}

bool BigInt::operator<(const BigInt &B) const {
  //printf("Size == %d, B.Size == %d\n", Size, B.Size);
  if(Size < B.Size)
    return true;
  else if(B.Size < Size) return false;
  
  for(int i = Size-1; i >= 0; i--) {
    //printf("Coef[i] == %d, B.Coef[i] == %d\n", Coef[i], B.Coef[i]);
    if(Coef[i] > B.Coef[i]) return false;
    else if(Coef[i] < B.Coef[i]) return true;
  }
  return false;
}
	

inline BigInt& BigInt::operator=(const BigInt &A) {
  const ushort *a = A.Coef;
  if (this == &A) return *this;
  if ( SizeMax < A.Size ) {
    delete Coef;
    Coef = new ushort[A.Size];
    SizeMax = Size = A.Size;
  } else Size = A.Size;
  for(ulong l=0; l<Size; l++)
    Coef[l] = a[l];
  return *this;
}

BigInt BigInt::operator *(const BigInt &B) const {
  if((Size == 1 && this->Coef[0] == 0) || (B.Size == 1 && B.Coef[0] == 0)) return BigInt("0"); // workaround :)
  BigInt result(this->Size + B.Size);
  ulong i, j;
  const ushort *a = this->Coef, *b = B.Coef;
  ushort *c = result.Coef;
  ulong temp, carry;
  
  for ( i=0; i < this->Size+B.Size; i++ ) c[i]=0;
  for ( i = 0; i < this->Size; i++) {
    carry = 0;
    for (j = 0; j < B.Size; j++) {
      temp = a[i] * b[j] + c[i+j] + carry;
      carry = temp/BASE;
      c[i+j] = temp - carry*BASE;
    }
    c[i+j] = carry;
  }
  i = this->Size + B.Size - 1;
  if ( c[i] == 0 ) i--;
  result.Size = i+1;
  return result;
}

BigInt BigInt::operator +(const BigInt &B) const {
  ulong i;
  long temp;
  const ushort *a=this->Coef, *b=B.Coef;
  ushort *c, carry = 0;
  
  if ( this->Size < B.Size )
    return B + *this;

  BigInt result(this->Size+1);
  c = result.Coef;
  
  for (i=0; i<B.Size; i++) {
    temp = a[i] + b[i] + carry;
    if (temp >= BASE) {
      c[i] = temp - BASE;
      carry = 1;
    } else {
      c[i] = temp;
      carry = 0;
    }
  }
  
  for (; i < this->Size; i++) {
    temp = a[i] + carry;
    if (temp >= BASE) {
      c[i] = temp - BASE;
      carry = 1;
    } else {
      c[i] = temp;
      carry = 0;
    }
  }
  if (carry) {
    c[i] = carry;
    result.Size = this->Size+1;
  } 
  else result.Size = this->Size;
  return result;
}

BigInt BigInt::operator -(const BigInt &B) const {

  const ushort *a = this->Coef, *b = B.Coef;
  ushort *c;
  ulong i;
  long temp;
  short carry=0;

  if ( this->Size < B.Size ) error ("BigSub: size error");

  BigInt result(MAX(this->Size, B.Size));
  c = result.Coef;	

  for (i=0; i<B.Size; i++) {
    temp = a[i] - b[i] + carry;
    if (temp < 0) {
      c[i] = temp + BASE;
      carry = -1;
    } else {
      c[i] = temp;
      carry = 0;
    }
  }

  for (; i< this->Size; i++) {
    temp = a[i] + carry;
    if (temp < 0) {
      c[i] = temp + BASE;
      carry = -1;
    } else {
      c[i] = temp;
      carry = 0;
    }
  }
  
  i = this->Size-1;
  while ( (i>0) && (c[i]==0) ) i--;
  result.Size = i+1;
  
  return result;
}



BigInt BigInt::mulbase(const ushort B) const {
  BigInt result(this->Size + 1);
  ulong i, temp;
  const ushort *a = this->Coef;
  ushort *c = result.Coef, carry = 0;

  for (i=0; i<this->Size;i++) {
    temp = a[i]*B + carry;
    carry = temp / BASE;
    c[i] = temp - carry*BASE;
  }
  
  if (carry) {
    c[i] = carry;
    result.Size = this->Size + 1;
  } else result.Size = this->Size;
  return result;
}

void BigInt::selfmulbase(const ushort B) {
  ulong i, temp;
  const ushort *a = this->Coef;
  ushort *c = this->Coef, carry = 0;

  for (i=0; i< Size; i++) {
    temp = a[i]*B + carry;
    carry = temp / BASE;
    c[i] = temp - carry * BASE;
  }
  
  if (carry) {
    c[i] = carry;
    Size++;
  }
}


BigInt BigInt::operator %(BigInt &B) const {
  BigInt result(B.Size);
  BigInt Q(this->Size);
  div(B, Q, result);
  return result;
}

void BigInt::divbase(const ushort B, BigInt &Q, ushort &R) const {
  short r = 0;
  ushort *q=Q.Coef;
  const ushort *a = this->Coef;
  long i, temp;

  for ( i=this->Size-1; i>=0; i--) {
    temp = r*BASE + a[i]; //  r=0
    q[i] = temp / B;
    r = temp - q[i]*B;
  }
  
  R = r;
  
  i = this->Size-1;
  while ( (i>0) && (q[i]==0) ) i--;
  Q.Size = i+1;
}

BigInt BigInt::operator /(BigInt &B) const {
  BigInt result(this->Size);
  BigInt R(B.Size);
  div(B, result, R);
  return result;
}

void BigInt::div(BigInt &B, BigInt &Q, BigInt &R) const {
  if ( this->Size < B.Size ) {
    Q.zero();
    R = *this;
    return;
  }
  
  if ( B.Size == 1) {		
    divbase(B.Coef[0], Q, R.Coef[0]);
    R.Size = 1;
    return;
  }
  
  BigInt U(this->Size+1); U = *this; U.Coef[this->Size]=0; 

  ushort *b = B.Coef, *u = U.Coef, *q = Q.Coef;
  long n = B.Size, m = U.Size - B.Size;
  
  long uJ, vJ, i;		
  long temp1, temp2, temp;
  
  ushort scale;	
  ushort qGuess, r;
  short borrow, carry;
  
  scale = BASE / ( b[n-1] + 1 );
  
  if (scale > 1) {		
    U = U.mulbase(scale);
    B = B.mulbase(scale);
  }

  
  for (vJ = m, uJ=n+vJ; vJ>=0; --vJ, --uJ) {

    qGuess = (u[uJ]*BASE + u[uJ-1]) / b[n-1];
    r = (u[uJ]*BASE + u[uJ-1]) % b[n-1];

    while ( r < BASE) {	
      temp2 = b[n-2]*qGuess;
      temp1 = r*BASE + u[uJ-2];

      if ( (temp2 > temp1) || (qGuess==BASE) ) {
	--qGuess;
	r += b[n-1];
      } else break;
    }

    carry = 0; borrow = 0; 
    ushort *uShift = u + vJ;

    for (i=0; i<n;i++) {
      temp1 = b[i]*qGuess + carry;  
      carry = temp1 / BASE;
      temp1 -= carry*BASE;       

      temp2 = uShift[i] - temp1 + borrow;	
      if (temp2 < 0) {
	uShift[i] = temp2 + BASE;
	borrow = -1;
      } else {
	uShift[i] = temp2;
	borrow = 0;
      }
    }

    temp2 = uShift[i] - carry + borrow;
    if (temp2 < 0) {
      uShift[i] = temp2 + BASE;
      borrow = -1;
    } else {
      uShift[i] = temp2;
      borrow = 0;
    }
		

    if (borrow == 0) {
      q[vJ] = qGuess;
    } else {
      q[vJ] = qGuess-1;

      carry = 0;
      for (i=0; i<n; i++) {
	temp = uShift[i] + b[i] + carry;
	if (temp >= BASE) {
	  uShift[i] = temp - BASE;
	  carry = 1;
	} else {
	  uShift[i] = temp;
	  carry = 0;
	}
      }
      uShift[i] = uShift[i] + carry - BASE;
    }
    i = U.Size-1;
    while ( (i>0) && (u[i]==0) ) i--;
    U.Size = i+1;

  }
  while ( (m>0) && (q[m]==0) ) m--;
  Q.Size = m+1;
	
  if (scale > 1) {
    ushort junk;
    B.divbase(scale, B, junk);
    U.divbase(scale, R, junk);
  } else R=U;
}


ostream& operator<<(ostream& os, const BigInt& A) {
	long j, Digit=0;
	short Pow, Dec, Coef;

	os << A.Coef[A.Size-1];

	for (long i=A.Size-2; i>=0; i--) {
		Pow = BASE/10;
		Coef = A.Coef[i];
		for (j=0; j<BASE_DIG; j++) {
			Dec = Coef/Pow;
			Coef -= Dec*Pow;
			Pow /= 10;
			os << Dec;
			Digit++;
			//if (Digit%1000==0) os << "\n\n";
			//else if (Digit%50==0) os << "\t: " << Digit << "\n";
		}
	}
	return os;
}





