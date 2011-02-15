#include "bigint.h"

const int SZ=sizeof(BASE)*8;
char msg [250];

BASE* bigint::get_memory(int size){ //size - кол-во баз
		BASE *p;
		try{
			p = new BASE [size];
		}
		catch (std::bad_alloc){
		sprintf(msg, "Memory can not be allocated. You tried to allocate %d bytes", size*sizeof(BASE));
		throw (msg);
		}
		return p;
	}

int bigint::Size() const {
	return (pAll-pLow + 1);
	}

int bigint::SizeZ() const {
	return (pHigh-pLow + 1);
	}

void bigint::normalize(){
		pHigh = pAll;
		while ((pHigh > pLow)&&(*pHigh==0)) pHigh--;
	}

bigint::bigint(){
		pLow = NULL;
		pHigh = NULL;
		pAll = NULL;
	}

bigint::bigint (int size, int type){
		pLow = get_memory(size);
		pAll = pLow + size - 1;
		switch (type){
			case 0:	{
			    for (BASE *p=pLow; p<=pAll; p++) *p=0;
				pHigh = pLow; break;}
			default:{
			    for (BASE *p=pLow; p<=pAll; p++) *p=rand()%0x7ffffff;
				normalize();
				break;}
		}
}

bigint::bigint (const char* a){
    bigint k(1,0),ten(10);
	for(int i=0;a[i]!=0;i++){
	    bigint ai(a[i]-'0');
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

BASE bigint::add(BASE* u0, BASE* un, BASE* v0, BASE* vm,BASE* w){
    DOUBLEBASE result;
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

void bigint::imul(BASE* u0, BASE* un, BASE* v0, BASE* vm,BASE* w) {
	DOUBLEBASE result;
	BASE pereNos;
	BASE *qp, *wp;
	for(BASE *pp = v0; pp<= vm ; ++pp, ++w )
		{if (*pp!= 0){
		pereNos = 0;
		for(qp = u0, wp = w; qp <= un; ++qp,++wp)
			{
			result = (DOUBLEBASE)*pp * (DOUBLEBASE)*qp + (DOUBLEBASE)*wp + (DOUBLEBASE)pereNos;
			*wp = (BASE)result;
			pereNos = result >> SZ;
			}
		*wp = pereNos;
		}
	}
	}

bigint::bigint (unsigned int a)
	{
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

void bigint::PrintHex()
	{int k=SZ/4;
		for (BASE *p=pHigh; p>=pLow; p--)
		{for(int i=0,sh=SZ-4;i<k;i++,sh-=4)
			printf("%x", (*p>>sh)&0xf);
		printf(" ");
		}
	printf("\n");	//сделать так, чтобы число выводилось со всеми левыми нулями!!
	}
	//дома конструктор копии, оператор присвоения, деструктор.

bigint::bigint(const bigint &a)
	{int n=a.pAll-a.pLow+1;
	pLow=get_memory(n);
	pAll=pLow+n-1;
	for(BASE *p=a.pLow,*q=this->pLow;p<=a.pAll;p++, q++)
	 *q=*p;
	this->normalize();

	}

int bigint::sub(BASE * u0, BASE * un, BASE * v0, BASE * vm, BASE * w0)
{
	DOUBLEBASE result;
	BASE carry = 0;

	BASE *p1=u0, *p2=v0, *p3=w0;

	for (;p1<=un && p2<=vm; p1++, p2++, p3++)
		{ result=((DOUBLEBASE)1<<yet) + *p1;
		result = result - *p2 - carry;
        	*p3=(BASE)result;
		if(result>>yet)   carry=0;else carry=1;
		}

        for (;p1<=un;p1++, p3++)
		{
		result=((DOUBLEBASE)1<<yet) + *p1;
		result = result -carry;
		*p3=(BASE)result;
		if(result>>yet)   carry=0;else carry=1;
		}

	for( ;p2<=vm;p2++,p3++)
		{result=((DOUBLEBASE)1<<yet);
		result = result -carry -*p2;
        	*p3=(BASE)result;
		if(result>>yet)   carry=0;else carry=1;
		}

	return carry;
}
// OLD
bigint bigint::operator = (const bigint &a) {
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
}
/*
bigint bigint::operator = (const bigint &a) {
//использовать проверку на размер баз
printf("%d and %d\n", this->Size(), a.SizeZ());
	if (this->Size() >= a.SizeZ()) {
		
		for(BASE *p=a.pLow,*q=this->pLow;p<=a.pHigh;p++, q++) 
			*q=*p;
		for(BASE *q=this->pHigh;q<=this->pAll;*q=0);
		this->normalize();
		return *this;
	}
	if(this!=&a) {
		if (pLow!=NULL) {
			delete [] pLow;
		}
		int n=a.pAll-a.pLow+1;
		pLow=get_memory(n);
		pAll=pLow+n-1;

		for(BASE *p=a.pLow,*q=this->pLow;p<=a.pAll;p++, q++) 
			*q=*p;
		this->normalize();
		}
	return *this;
}*/

bigint::~bigint()
	{
     	if(pLow!=NULL) delete[] pLow;
	}

bigint bigint::operator + (const bigint & b) {
	int n = this->SizeZ();
	if(b.SizeZ()> n){n = b.SizeZ();}
	bigint w(n+1,0);
	*w.pAll = add(this->pLow, this->pHigh, b.pLow, b.pHigh, w.pLow);
	w.normalize();
	return w;
	}

bigint bigint::operator - (const bigint & arg) {
	if(*this<arg) throw "otritsateln rezult";  
	int n = this->SizeZ();

	bigint result(n,0);

	sub(this->pLow, this->pHigh, arg.pLow, arg.pHigh, result.pLow);
	result.normalize();

	return result;
	}

bigint bigint::operator * (const bigint & b) {
	int n = this->SizeZ()+b.SizeZ();
	bigint w(n,0);
	imul(this->pLow, this->pHigh, b.pLow, b.pHigh, w.pLow);
	
	w.normalize();
	
	return w;
	}

bigint bigint::div_BASE(BASE *u0,BASE *u1,BASE v,BASE &ost) //u0-pAll, u1-pHigh;
	{bigint ch(u0-u1+1,0);
	BASE *q=ch.pAll,q1;
	//DOUBLEBASE
	q1=0;
	for (q=ch.pAll ;u0>=u1; u0--,q--)
	*(q)=((DOUBLEBASE)(q1*pz)+(*u0))/v,
	q1=((DOUBLEBASE)(q1*pz)+(*u0))%v;
	ost=q1;
	ch.normalize();
	return ch;
	}

bigint bigint::div(bigint u,bigint v,bigint &r)  // функция возвращает частное, в r остаток
	{int nu=u.SizeZ(), nv=v.SizeZ();
	int m=nu-nv;
	bigint w,g(1,0); int t;
	bigint ch(m+1,0);
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
		else q1=( ((DOUBLEBASE)*u0)*pz+(*(u0-1)) )/(*v0);
		while( (((DOUBLEBASE)*(v0-1))*(q1))>(  (((DOUBLEBASE)*u0)*pz + ((DOUBLEBASE)*(u0-1))-q1*((DOUBLEBASE)*v0))*pz+(DOUBLEBASE)*(u0-2)) )
			q1--  ;
	 	w=v*q1;t=sub(u1,u0,w.pLow,w.pAll,u1);
   //  D4

		if(t)
			{q1--;
			add(u1, u0, v1, v0,u1); // D6
			}
		*(ch.pAll-j)=q1;
		}
	BASE k;
	u.normalize();


		r=div_BASE(u.pAll,u.pLow,d,k);




	// нужно возвратить остаток в r
	return ch;
	}

bigint bigint::operator / ( bigint &t) {
	bigint q,r(1,0);
	BASE ost;
	bigint tmp0(1,0),tmp1(1);
	if (t==tmp0) throw "Error: devision by 0 in operator /";
	if(*this<t) return tmp0;
	if(*this==t)return tmp1;
	if(t.pHigh==t.pLow) return (this->div_BASE(this->pHigh,this->pLow,*t.pLow,ost));

	q=div(*this,t,r);
	q.normalize();
	return q;
	}

bigint bigint::operator % ( bigint &t) {
	bigint tmp0(1,0),tmp1(1,1);
	BASE ost;
	if (t==tmp0) throw "Error: devision by 0 in operator %";
	if(*this<t) return *this;
	if(*this==t)return tmp0;
	if(t.pHigh==t.pLow) {this->div_BASE(this->pHigh,this->pLow,*t.pLow,ost);
	return ost;
	}
	bigint q,r(1,0),s;
	q=div(*this,t,r);
	r.normalize();
	return r;
	}


bool bigint::operator == (const bigint &arg) const {
    int n=this->SizeZ(),v=arg.SizeZ();
	if (n=!v) {
		return false;
	}

	BASE *q=this->pHigh,*w=arg.pHigh;
	for ( ;q>=this->pLow && *w==*q;q--,w--);
	if (q<this->pLow) return true;
	return false;
	}

bool bigint::operator < (const bigint & arg) const {
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

bool bigint::operator <= (const bigint & arg) const {
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

bool bigint::operator > (const bigint & arg) const {
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

bool bigint::operator >= (const bigint & arg) const {
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

bool bigint::operator != (const bigint & arg) const {
    if (arg.pLow==this->pLow) return false;
    if (this->SizeZ() != arg.SizeZ()) return true;
    BASE *pn=arg.pLow, *pm=this->pLow;
    while (pn<=arg.pHigh && *pn==*pm) pn++, pm++;
    if (pn<=arg.pHigh) return true;
    return false;
}


int main (void) {
	srand(time(NULL));
	try {
		bigint Q, R;
		bigint A, B;
		bigint zero(1,0);
		int T = 1000, M = 1000;
		int r1, r2;
		int i = 0;
		puts("Starting cycle...");
		do {
			i++;
			r1 = rand()%M+1;
			r2 = rand()%M+1;
			bigint AA(r1,2);
			bigint BB(r2,2);
			if (BB == zero) continue;
			A = AA;
			B = BB;
			Q = A/B;
			R = A%B;
		} while (--T && A == B*Q+R && R < B && A-R==B*Q);
		if (T == 0) {
			puts("OKAY");
			return 0;
		}
		
	} catch (char * msg) {
		puts(msg);
		puts("ERROR");
		return -1;
	}
}
