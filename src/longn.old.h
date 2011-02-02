/* Long numbers implementation
* TODO: refactor, make sexy
* Depends: longn.h 		*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <limits.h>

#include "longn.h"

using namespace std;
int mem = 0;
/* Private functions zone */

BASE * Longn::GetMem (int size) {
  BASE *p;
  try {
    p = new BASE[size];
  } catch (bad_alloc) {
    sprintf(myexception, "Bad allocation: %d", size);
    throw(myexception);
  }
  return p;
}

void Longn::FreeMem (void) {
  if (pLow) {
	delete [] pLow;
	}
}

int Longn::Size (void) const {
  return (pAll - pLow + 1);
}

int Longn::SizeZ (void) const {
  return (pHigh - pLow + 1);
}

void Longn::Normalize (void) {
    pHigh = pAll;
    while ((pHigh > pLow)&&(*pHigh==0)) pHigh--;
}

void Longn::PrintHex (const char * name = "") {
  printf("\n%s=", name);
  int k = SZ / 4;
  for (BASE *ptr = pAll; ptr >= pLow; ptr--) {
    for (int i = 0, sh = SZ - 4; i < k; i++, sh -= 4) printf("%x" , (*ptr>>sh)&0xF);
    printf(" ");
  }
}
void Longn::PrintDec(const char * name = "") {
    cout << name << ": ";
    Longn tmp(*this);
    int i = 0;
    char ret[500], chh;

    while(tmp.pHigh >= tmp.pLow) {
        BASE r;
        //chh = (tmp.divb(10, r) + '0');
        ret[i] = chh;
        i++;
    }

    for(i--; i >= 0; i--)
        printf("%c", ret[i]);
    cout << ";\n";
    cout << "total=" << (BASE) (pAll - pLow) + 1 << ", use=" << (BASE) (pHigh - pLow) + 1 << " ;\n";
}

/* MAIN FUNCTIONS */

BASE Longn::add(BASE* u0, BASE* un, BASE* v0, BASE* vm, BASE* w){
    BASE2 result;

    BASE pereNos = 0;
    BASE *pp = u0 , *qp = v0;
    for(; pp<= un && qp <= vm; ++pp, ++qp, ++w ) {
        result = *pp+ *qp + pereNos;
        *w = (BASE)result;
        pereNos = result >> SZ;
    }
    for(; pp<= un ; ++pp, ++w ){
        result = *pp + pereNos;
        *w = (BASE)result;
        pereNos = result >> SZ;
    }

    for(; qp<= vm ; ++qp, ++w ) {
        result = *qp + pereNos;
        *w = (BASE)result;
        pereNos = result >> SZ;}

    return pereNos;
}

void Longn::mul(BASE* u0, BASE* un, BASE* v0, BASE* vm,BASE* w) {
    BASE2 result;
        BASE pereNos;
        BASE *qp, *wp;
        for(BASE *pp = v0; pp<= vm ; ++pp, ++w) {
            if (*pp!= 0){
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

int Longn::sub(BASE * u0, BASE * un, BASE * v0, BASE * vm, BASE * w0) {
    BASE2 result;
    BASE carry = 0;

    BASE *p1=u0, *p2=v0, *p3=w0;

    for (;p1<=un && p2<=vm; p1++, p2++, p3++){
        result=((BASE2)1<<SZ) + *p1;
        result = result - *p2 - carry;
        *p3=(BASE)result;
        if(result>>SZ) carry=0;
        else carry=1;
    }

    for (;p1<=un;p1++, p3++) {
        result=((BASE2)1<<SZ) + *p1;
        result = result -carry;
        *p3=(BASE)result;
        if(result>>SZ) carry=0;
        else carry=1;
        }

    for( ;p2<=vm;p2++,p3++)
        {result=((BASE2)1<<SZ);
        result = result - carry -*p2;
        *p3=(BASE2)result;
        if(result>>SZ) carry=0;
        else carry=1;
    }

    return carry;
}
Longn Longn::divb(const BASE t, BASE &r) {
  int n=pHigh-pLow;
  Longn ch(n+1,0);
  BASE2 tmp;
  BASE p=0;

  for(BASE *iPtr=pHigh, *chPtr=ch.pAll; iPtr>=pLow; iPtr--, chPtr--) {
    tmp=0;
    tmp|=p<<SZ;
    tmp+=*iPtr;
    *chPtr=tmp/t;
    p=tmp%t;
  }

  r=p;
  ch.pAll=ch.pLow+n;
  ch.Normalize();
  return ch;
}

Longn Longn::div(Longn u, Longn v, Longn &r)  // функция возвращает частное, в r остаток
	{
	    int nu=u.SizeZ(), nv=v.SizeZ();
	int m=nu-nv;
	Longn w,g(1,0); int t;
	Longn ch(m+1,0);
	BASE d=pow/(*v.pHigh+1);

    //D2
	BASE q1;
	u=u*d;
	v=v*d;

	v.Normalize();
	BASE *u0=u.pAll,*v0=v.pHigh,*u1=u.pAll-nv,*v1=v.pLow,*q=ch.pAll,*s=u.pLow;
	for(int j=0 ;u1>=u.pLow && j<=m;u0--,u1--,q--,j++) {
		if(*u0==*v0) q1=pow-1;
		else q1=( ((BASE2)*u0)*pow+(*(u0-1)) )/(*v0);
		while( (((BASE2)*(v0-1))*(q1))>(  (((BASE2)*u0)*pow + ((BASE2)*(u0-1))-q1*((BASE2)*v0))*pow+(BASE2)*(u0-2)) )
			q1--;
	 	w=v*q1;t=sub(u1,u0,w.pLow,w.pAll,u1);
   //  D4
		if(t)
			{q1--;
			add(u1, u0, v1, v0,u1); //D6
			}
		*(ch.pAll-j)=q1;
		}
	BASE k;
	u.Normalize();
	r=divrm(u.pAll,u.pLow,d,k);
	return ch;
}


Longn Longn::divrm(BASE *u0,BASE *u1,BASE v,BASE &ost) {
    Longn ch(u0-u1+1,0);
    BASE *q=ch.pAll,q1;
    //BASE2
    q1=0;
    for (q=ch.pAll ;u0>=u1; u0--,q--) {
    *(q)=((BASE2)(q1*pow)+(*u0))/v;
    q1=((BASE2)(q1*pow)+(*u0))%v; }
    ost=q1;
    ch.Normalize();
    return ch;
}

/* END MAIN FUNCTIONS */

/* Constructors & Destructors */

Longn::Longn () {
  pLow = NULL;
  pHigh = NULL;
  pAll = NULL;
}

Longn::Longn (int size, int type) {
        pLow = GetMem(size);
        pAll = pLow + size - 1;
        switch (type)
        {
                case 0:	{for (BASE *p=pLow; p<=pAll; p++) *p=0;
                                pHigh = pLow; break;}
                default:{for (BASE *p=pLow; p<=pAll; p++) *p=rand()%0xff+1;
                                Normalize();
                                break;}
        }
}

Longn::Longn (unsigned int arg) {
  int sizeb = sizeof(BASE);
  int sizeui = sizeof(unsigned int);

  if (sizeb > sizeui) {
    pLow = GetMem(1);
    pAll = pLow;
    *pLow = arg;
  } else {
    int sizea = (sizeui + sizeb - 1) / sizeb; //количество баз в ui
    pLow = GetMem(sizea);
    pAll = pLow + sizea - 1;

    for (BASE *ptr = pLow; ptr <= pAll; ptr++) *ptr = 0;
    for (BASE *ptr = pLow; arg != 0; arg = arg >> SZ, ptr++) *ptr = (BASE) arg;
  }
  Normalize();
}

Longn::Longn (const char * string) {
    BASE a=10;
    Longn ten(a);
    Longn k(1,0);
    for(int i=0;string[i]!='\0';i++){
      k=k*ten;
      Longn l((BASE)(string[i]-'0'));
      k=k+l;
    }
    k.Normalize();
    int n=k.pHigh-k.pLow;
    pLow=GetMem(n+1);
    pAll=pLow+n;
    for(BASE *tmp=k.pLow,*b=pLow;tmp<=k.pHigh;tmp++,b++) *b=*tmp;
    this->Normalize();
}

Longn::Longn(const Longn &a)
        {int n=a.pAll-a.pLow+1;
        pLow=GetMem(n);
        pAll=pLow+n-1;
        for(BASE *p=a.pLow,*q=this->pLow;p<=a.pAll;p++, q++)
         *q=*p;
        this->Normalize();

        }

Longn::~Longn() {
  FreeMem();
  pAll = pLow = pHigh = NULL;
}
/* END Constructors & Destructors */

/* ACTION OPERATORS */

Longn Longn::operator = (const Longn &a) {
    if (this!=&a) {
        if(pLow)delete [] pLow;
        int n=a.pAll-a.pLow+1;
        pLow=GetMem(n);
        pAll=pLow+n-1;
        for(BASE *p=a.pLow,*q=this->pLow;p<=a.pAll;p++, q++)
         *q=*p;
        this->Normalize();
     }
    return *this;
}

Longn Longn::operator + (const Longn arg) {
  int n = SizeZ();
  if (arg.SizeZ() > n) n = arg.SizeZ();
  Longn result(n+1, 0);
  *result.pAll = add(pLow, pHigh, arg.pLow, arg.pHigh, result.pLow);
  result.Normalize();

  return result;
}

Longn Longn::operator * (const Longn & b) {
    int n = this->SizeZ()+b.SizeZ();
    Longn w(n,0);
    mul(this->pLow, this->pHigh, b.pLow, b.pHigh, w.pLow);
    w.Normalize();
    return w;
}

Longn Longn::operator - (const Longn & arg) {
    int n = this->SizeZ();
    Longn result(n,0);
    sub(this->pLow, this->pHigh, arg.pLow, arg.pHigh, result.pLow);
    result.Normalize();
    return result;
}
/* END ACTION OPERATORS */

/* CMP OPERATORS */

bool Longn::operator == (const Longn &arg) const {
    int n=this->SizeZ(),v=arg.SizeZ();
	if (n=!v)return false;

	BASE *q=this->pHigh,*w=arg.pHigh;
	for ( ;q>=this->pLow && *w==*q;q--,w--);
	if (q<this->pLow) return true;
	return false;
	}

bool Longn::operator < (const Longn & arg) const {
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

bool Longn::operator <= (const Longn & arg) const {
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

bool Longn::operator > (const Longn & arg) const {
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

bool Longn::operator >= (const Longn & arg) const {
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

bool Longn::operator != (const Longn & arg) const {
    if (arg.pLow==this->pLow) return false;
    if (this->SizeZ() != arg.SizeZ()) return true;
    BASE *pn=arg.pLow, *pm=this->pLow;
    while (pn<=arg.pHigh && *pn==*pm) pn++, pm++;
    if (pn<=arg.pHigh) return true;
    return false;
}


/* END CMP OPERATORS */



Longn Longn::operator / (const Longn &arg) {
        Longn q,r(1,0);
        BASE ost;
        Longn tmp0(1,0),tmp1(1);
        if (arg==tmp0) throw "/ Error: devision by zero";
        if(*this<arg) return tmp0;
        if(*this==arg)return tmp1;
        if(arg.pHigh==arg.pLow) return (this->divrm(this->pHigh,this->pLow,*arg.pLow,ost));

        q=div(*this,arg,r);
        q.Normalize();
        return q;
}

Longn Longn::operator % (const Longn &t) {
        Longn tmp0(1,0);
	BASE ost;

        if (t==tmp0) throw "% Error: division by zero";
        if(*this<t) return *this;
        if(*this==t)return tmp0;
        if(t.pHigh==t.pLow) {
		divrm(pHigh,pLow,*t.pLow,ost);
		return ost;
	}
        Longn q,r(1,0);
        q=div(*this,t,r);
        r.Normalize();
        return r;
}

int main () {
try {
  srand(time(NULL));
  Longn A, B, C;

  int a, n, b;

   for (int i =0; i< 1000000; i++) {
        a = rand()%200 +1;
        n = rand()%150 +1;

        Longn *A1;
	A1 = new Longn(a, 2);
        Longn *A2;
	A2 = new Longn(n, 2);

        A = *A1 / *A2;
        B = *A1 % *A2;
        if(*A1 != *A2 * A + B || B >= *A2) {
            cout << "Error in div at iter.: " << i << endl;
        }
	delete A1;
	delete A2;
   }

return 0;
 } catch (const char * msg) {
    cout << msg;
  }

}
