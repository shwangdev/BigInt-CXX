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

#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "bigint.h"

using namespace std;

#ifdef DEBUG
#define WHERESTR  "[file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__)
#define DEBUGPRINT(_fmt, ...)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#else
#define DEBUGPRINT(...)
#endif

const int SZ=sizeof(BASE)*8;
char msg [1024];

BASE* BigInt::get_memory(int size){ //size - кол-во баз
		BASE *p;
		try{
			//DEBUGPRINT("Allocating %d bases\n", size);
			p = new BASE [size];
		}
		catch (std::bad_alloc) {
			sprintf(msg, "Memory can not be allocated. You tried to allocate %d bytes", size*sizeof(BASE));
		throw (msg);
		}
		return p;
	}

int BigInt::Size() const {
	return (pAll-pLow + 1);
}

int BigInt::SizeZ() const {
	return (pHigh-pLow + 1);
}

void BigInt::normalize() {
	pHigh = pAll;
	while ( (pHigh > pLow)&&(*pHigh==0) )
		pHigh--;
}

BigInt::BigInt() {
	pLow = NULL;
	pHigh = NULL;
	pAll = NULL;
}

BigInt::BigInt (int size, int type) {
	pLow = get_memory(size);
	pAll = pLow + size - 1;
	switch (type){
		case 0:
			for (BASE *p=pLow; p<=pAll; p++) *p=0;
			pHigh = pLow;
			break;
		default:
			for (BASE *p=pLow; p<=pAll; p++)
				*p=rand()%0x7ffffff;
			normalize();
			break;
	}
}

BigInt::BigInt(const BigInt &a) {
	int n=a.pHigh-a.pLow+1;
	pLow=get_memory(n);
	pAll=pLow+n-1;
	for(BASE *p=a.pLow,*q=this->pLow;p<=a.pHigh;p++, q++)
	 *q=*p;
	this->normalize();

}

BigInt::BigInt (const char* a) {
    BigInt k(1,0),ten(10);
	for(int i=0;a[i]!=0;i++){
	    BigInt ai(a[i]-'0');
		k=k*ten+ai;
    }
	k.normalize();
	int n=k.pHigh-k.pLow+1;
	pLow=get_memory(n);
	pAll=pLow+n-1;
	for(BASE *p=k.pLow,*q=this->pLow;p<=k.pAll;p++, q++)
	 *q=*p;
	this->normalize();
}

BigInt::BigInt (unsigned int a)	{
		int sb = sizeof(BASE), si = sizeof(unsigned int);
		if (sb > si)
		{
			pLow = get_memory(1);
			pAll = pLow;
			pHigh = pAll;
			*pLow = a;
		}
		else
		{
			int s = (si+sb-1)/sb;
			pLow = get_memory(s);
			pAll = pLow + s - 1;
			for (BASE *p=pLow; p<=pAll; p++) *p=0;
			for (BASE *p=pLow;a;p++,a>>=SZ) *p=(BASE)a;
			this->normalize();
		}
}

BigInt::~BigInt() {
	//DEBUGPRINT("Deallocating %d bases\n", pAll-pLow+1);
	if(pLow!=NULL)
		delete[] pLow;
}

BASE BigInt::add(BASE* u0, BASE* un, BASE* v0, BASE* vm,BASE* w) {
    BASE2 result;
	BASE pereNos = 0;
	BASE *pp = u0 , *qp = v0;
	for(; pp<= un && qp <= vm; ++pp, ++qp, ++w )
	{result = *pp+ *qp + pereNos;
	*w = (BASE)result;
	pereNos = result >> SZ;
	}
	for(; pp<= un ; ++pp, ++w )
	{result = *pp + pereNos;
	*w = (BASE)result;
	pereNos = result >> SZ;}

	for(; qp<= vm ; ++qp, ++w )
	{result = *qp + pereNos;
	*w = (BASE)result;
	pereNos = result >> SZ;}

	return pereNos;
}

void BigInt::imul(BASE* u0, BASE* un, BASE* v0, BASE* vm,BASE* w) {
	BASE2 result;
	BASE pereNos;
	BASE *qp, *wp;
	for(BASE *pp = v0; pp<= vm ; ++pp, ++w )
		{if (*pp!= 0){
		pereNos = 0;
		for(qp = u0, wp = w; qp <= un; ++qp,++wp)
			{
			result = (BASE2)*pp * (BASE2)*qp + (BASE2)*wp + (BASE2)pereNos;
			*wp = (BASE)result;
			pereNos = result >> SZ;
			}
		*wp = pereNos;
		}
	}
}

void BigInt::PrintHex () {
  int k = SZ / 4;
  for (BASE *ptr = pAll; ptr >= pLow; ptr--) {
    for (int i = 0, sh = SZ - 4; i < k; i++, sh -= 4) printf("%x" , (*ptr>>sh)&0xF);
    printf(" ");
  }
}

void BigInt::PrintDec() {
    BigInt tmp(*this);
    int i = 0;
    char ret[500], chh;

    while(tmp.pHigh >= tmp.pLow) {
        //BASE r;
        //chh = (tmp.divb(10, r) + '0');
        //ret[i] = chh;
        i++;
    }

    for(i--; i >= 0; i--)
        printf("%c", ret[i]);
}

int BigInt::sub(BASE * u0, BASE * un, BASE * v0, BASE * vm, BASE * w0) {
	BASE2 result;
	BASE carry = 0;

	BASE *p1=u0, *p2=v0, *p3=w0;

	for (;p1<=un && p2<=vm; p1++, p2++, p3++)
		{ result=((BASE2)1<<yet) + *p1;
		result = result - *p2 - carry;
        	*p3=(BASE)result;
		if(result>>yet)   carry=0;else carry=1;
		}

        for (;p1<=un;p1++, p3++)
		{
		result=((BASE2)1<<yet) + *p1;
		result = result -carry;
		*p3=(BASE)result;
		if(result>>yet)   carry=0;else carry=1;
		}

	for( ;p2<=vm;p2++,p3++)
		{result=((BASE2)1<<yet);
		result = result -carry -*p2;
        	*p3=(BASE)result;
		if(result>>yet)   carry=0;else carry=1;
		}

	return carry;
}
/*
// OLD
BigInt BigInt::operator = (const BigInt &a) {
//использовать проверку на размер баз
	if(this!=&a) {
		if(pLow!=NULL){
			delete [] pLow;
		}
		int n=a.pAll-a.pLow+1;
		pLow=get_memory(n);
		pAll=pLow+n-1;
		//копировать до pHigh
		for(BASE *p=a.pLow,*q=this->pLow;p<=a.pAll;p++, q++)
		 *q=*p;
		this->normalize();
		}
	return *this;
}*/

BigInt BigInt::operator = (const BigInt &a) {
	if(this!=&a) {

		if (pLow!=NULL)
			delete [] pLow;

		int n=a.pAll-a.pLow+1;
		pLow=get_memory(n);
		pAll=pLow+n-1;

		for(BASE *p=a.pLow,*q=this->pLow;p<=a.pAll;p++, q++)
			*q=*p;
		this->normalize();
	}

	return *this;
}


BigInt BigInt::operator + (const BigInt & b) {
	int n = this->SizeZ();
	if(b.SizeZ()> n){n = b.SizeZ();}
	BigInt w(n+1,0);
	*w.pAll = add(this->pLow, this->pHigh, b.pLow, b.pHigh, w.pLow);
	w.normalize();
	return w;
}

BigInt BigInt::operator - (const BigInt & arg) {
	if(*this<arg) throw "otritsateln rezult";
	int n = this->SizeZ();

	BigInt result(n,0);

	sub(this->pLow, this->pHigh, arg.pLow, arg.pHigh, result.pLow);
	result.normalize();

	return result;
}

BigInt BigInt::operator * (const BigInt & b) {
	int n = this->SizeZ()+b.SizeZ();
	BigInt w(n,0);
	imul(this->pLow, this->pHigh, b.pLow, b.pHigh, w.pLow);

	w.normalize();

	return w;
}

BigInt BigInt::div_BASE(BASE *u0,BASE *u1,BASE v,BASE &ost) {
	BigInt ch(u0-u1+1,0);
	BASE *q=ch.pAll,q1;
	q1=0;
	for (q=ch.pAll ;u0>=u1; u0--,q--)
	*(q)=((BASE2)(q1*pz)+(*u0))/v,
	q1=((BASE2)(q1*pz)+(*u0))%v;
	ost=q1;
	ch.normalize();
	return ch;
}

//FIXME: REWRITE THIS NIGHTMARE
BigInt BigInt::div(BigInt u,BigInt v,BigInt &r) {
	int nu=u.SizeZ(), nv=v.SizeZ();
	int m=nu-nv;
	BigInt w,g(1,0); int t;
	BigInt ch(m+1,0);
	BASE d=pz/(*v.pHigh+1);

	BASE q1;
	u=u*d;
	v=v*d;
	v.normalize();
	BASE *u0=u.pAll,*v0=v.pHigh,*u1=u.pAll-nv,*v1=v.pLow,*q=ch.pAll,*s=u.pLow;
	//r=div_BASE(u0-m+1,s,d);
	for(int j=0 ;u1>=u.pLow && j<=m;u0--,u1--,q--,j++)
		{
		if(*u0==*v0) q1=pz-1;
		else q1=( ((BASE2)*u0)*pz+(*(u0-1)) )/(*v0);
		while( (((BASE2)*(v0-1))*(q1))>((((BASE2)*u0)*pz + ((BASE2)*(u0-1))-q1*((BASE2)*v0))*pz+(BASE2)*(u0-2)) )
			q1--;
	 	w=v*q1;t=sub(u1,u0,w.pLow,w.pAll,u1);
		if(t) {
			q1--;
			add(u1, u0, v1, v0,u1); // D6
		}
		*(ch.pAll-j)=q1;
		}
	BASE k;
	u.normalize();
	r=div_BASE(u.pAll,u.pLow,d,k);
	return ch;
}


BigInt BigInt::operator / (BigInt &t) {
	BigInt q,r(1,0);
	BASE ost;
	BigInt tmp0(1,0),tmp1(1);
	if (t==tmp0) throw "Error: devision by 0 in operator /";
	if(*this<t) return tmp0;
	if(*this==t)return tmp1;
	if(t.pHigh==t.pLow) return (this->div_BASE(this->pHigh,this->pLow,*t.pLow,ost));

	q=div(*this,t,r);
	q.normalize();
	return q;
}

BigInt BigInt::operator % (BigInt &t) {
	BigInt tmp0(1,0),tmp1(1,1);
	BASE ost;
	if (t==tmp0) throw "Error: devision by 0 in operator %";
	if(*this<t) return *this;
	if(*this==t)return tmp0;
	if(t.pHigh==t.pLow) {this->div_BASE(this->pHigh,this->pLow,*t.pLow,ost);
	return ost;
	}
	BigInt q,r(1,0),s;
	q=div(*this,t,r);
	r.normalize();
	return r;
}

bool BigInt::operator == (const BigInt &arg) const {
    int n=this->SizeZ(),v=arg.SizeZ();
	if (n=!v) {
		return false;
	}

	BASE *q=this->pHigh,*w=arg.pHigh;
	for ( ;q>=this->pLow && *w==*q;q--,w--);
	if (q<this->pLow) return true;
	return false;
}

bool BigInt::operator < (const BigInt & arg) const {
     if (arg.pLow==this->pLow) return false;
     if (this->SizeZ() < arg.SizeZ()) return true;
     if (this->SizeZ() > arg.SizeZ()) return false;
     BASE *pn=arg.pHigh, *pm=this->pHigh;
     while (pn>=arg.pLow) {
             if (*pm<*pn) return true;
             if (*pm>*pn) return false;
             if (*pm==*pn) pm--,pn--;
              }
      return false;
}

bool BigInt::operator <= (const BigInt & arg) const {
     if (arg.pLow==this->pLow) return true;
     if (this->SizeZ() < arg.SizeZ()) return true;
     if (this->SizeZ() > arg.SizeZ()) return false;
     BASE *pn=arg.pHigh, *pm=this->pHigh;
     while (pn>=arg.pLow)
             {
             if (*pm<*pn) return true;
             if (*pm>*pn) return false;
             if (*pm==*pn) pm--,pn--;
             }
     return true;
}

bool BigInt::operator > (const BigInt & arg) const {
    if (arg.pLow==this->pLow) return false;
    if (this->SizeZ() > arg.SizeZ()) return true;
    if (this->SizeZ() < arg.SizeZ()) return false;
    BASE *pn=arg.pHigh, *pm=this->pHigh;
    while (pn>=arg.pLow)
             {
             if (*pm>*pn) return true;
             if (*pm<*pn) return false;
             if (*pm==*pn) pm--,pn--;
             }
      return false;
}

bool BigInt::operator >= (const BigInt & arg) const {
    if (arg.pLow==this->pLow) return true;
    if (this->SizeZ() > arg.SizeZ()) return true;
    if (this->SizeZ() < arg.SizeZ()) return false;
    BASE *pn=arg.pHigh, *pm=this->pHigh;
    while (pn>=arg.pLow)
         {
         if (*pm>*pn) return true;
         if (*pm<*pn) return false;
         if (*pm==*pn) pm--,pn--;
         }
    return true;
}

bool BigInt::operator != (const BigInt & arg) const {
    if (arg.pLow==this->pLow) return false;
    if (this->SizeZ() != arg.SizeZ()) return true;
    BASE *pn=arg.pLow, *pm=this->pLow;
    while (pn<=arg.pHigh && *pn==*pm) pn++, pm++;
    if (pn<=arg.pHigh) return true;
    return false;
}


int main (void) {
	srand(time(NULL));
	BigInt Q, R;
	BigInt A, B;
	BigInt zero(1,0);
	int T = 1000, M = 1000;
	int r1, r2;
	int i = 0;
	puts("Starting cycle...");
	do {
		i++;
		r1 = rand()%M+1;
		r2 = rand()%M+1;
		BigInt AA(r1,2);
		BigInt BB(r2,2);
		if (BB == zero) continue;
		A = AA;
		B = BB;
		Q = A/B;
		R = A%B;
	} while (--T && A == B*Q+R && R < B && A-R==B*Q);
	DEBUGPRINT("quited at %d\n", i);
	if (T == 0) {
		puts("OKAY");
		return 0;
	}
}
