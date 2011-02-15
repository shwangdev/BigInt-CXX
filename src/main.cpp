#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "BigInt.h"

int main(){
  //  BigInt a("12345678912345678234567892345678234567899861236725378215427354328746284623846238746529782634897563294856328745638746487563284563287456328746523874569238475629837456298374562934587234578324587327532487563578736587365387563875635873653487563856378563758364578346583765387652438745632877657328465328456378456238653876532784564382563287657324653285673456787543456765456634564");
  //  BigInt b("2");
  //  BigInt b(321);
  //  BigInt c;
  //  Add(a, b, c);
  //  BigInt q(123), r(123);
  // std::cout<< a << "  " << b << std::endl;
  // a.div(b, q, r);
  // std::cout<< q << " " << r << std::endl;
  //  std::cout << a;
/*
  char buf1[1024], buf2[1024];
  while(scanf("%s %s", buf1, buf2)){
    BigInt a(buf1);
    BigInt b(buf2);
    std::cout<< a/b << " " << a%b << "\n";
  }
  */

  FILE *random = fopen("/dev/urandom", "rb");

  int T = 1000, M = 1000;
  BigInt A, B, Q, R;
  do {
    std::cout << "T = " << T << "\n";
    unsigned int randint;

    int r1,r2;
    do {
      fread(&randint, sizeof(unsigned int), 1, random);
      r1 = randint%M;
    }while(r1 == 0);
    do {
      fread(&randint, sizeof(unsigned int), 1, random);
      r2 = randint%M;
    }while(r2 ==0);
    char bufA[1024], bufB[1024];
    for(int i = 0; i < r1; i++){
      unsigned char byte;
      fread(&byte, 1, 1, random);
      bufA[i] = '0' + (byte%10);
    }
    bufA[r1] = '\0';
    for(int i = 0; i < r2; i++){
      unsigned char byte;
      fread(&byte, 1, 1, random);
      bufB[i] = '0' + (byte%10);
    }
    bufB[r2] = '\0';
    //std::cout << "bufA = '" << bufA << "', bufB = '"<<bufB<<"'\n";
    A = BigInt(bufA);
    B = BigInt(bufB);
    Q = BigInt(A/B);
    R = BigInt(A%B);
    //  std::cout << "A == " << A << "\n\nB == " << B << "\n\nQ == " << Q << "\n\nR == " << R << "\n\nB*Q+R == "<< B*Q+R << "\n\nA-R == "<< A-R << "\n--------------------------\n";
    //std::cout << "Test1: " << ((A==(B*Q+R)) ?"true":"false") << "\nTest2: " << ((R < B) ?"true":"false") << "\nTest3: " << (((A-R)==(B*Q))?"true":"false") << "\n\n\n";
										   }while(--T && A == (B*Q+R) && R < B && (A-R) == (B*Q));
  fclose(random);

  if(T == 0) { std::cout << "ALL TESTS OK\n"; }
  // for(int i = 0; i < 10000; i++){

  // char buf1[1024], buf2[1024];
  // for(int i = 0; i < 500; i++)
  //   buf1[i] = '0' + (rand()%10);
  // buf1[500] = 0;
  // for(int i = 0; i < 200; i++)
  //   buf2[i] = '0' + (rand()%10);
  // buf2[200] = 0;
  
  // BigInt a(buf1), b(buf1);

  // if((a / b) * b + (a % b) == a){
  //   printf("ok\n");
  // }

  // }

  return 0;
}
