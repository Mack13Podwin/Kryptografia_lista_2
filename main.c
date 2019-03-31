#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#include "TestU01.h"

#define N 256
#define D 3
#define KEY_LEN 128

unsigned char S[N];
unsigned int I=0U, J=0U;
int tempc=0;
unsigned char tempv[2];

void swap(int i, int j){
  unsigned char temp = S[i];
  S[i]=S[j];
  S[j]=temp;
}

unsigned char prga(void){
  I=(I+1)%N;
  J=(J+S[I])%N;
  swap(I,J);
  return S[(S[I]+S[J])%N];
}

void ksa(void){
  int t = N;
  unsigned char k[KEY_LEN];
  for (int i=0; i<KEY_LEN; i++){
    k[i] = rand()%256;
  }
  for(int i=0; i<N; i++){
    S[i] = (unsigned char)i;
  }
  int j=0;
  for (int i= 0; i<t; i++){
    j = (j+S[i%N]+k[i%KEY_LEN])%N;
    swap(i%N, j%N);
  }
}

void ksa_rs(void){
  int t = (int)ceil(2*N*log(N));
  printf("%i\n",t);
  unsigned char k[KEY_LEN];
  for (int i=0; i<KEY_LEN; i++){
    k[i] = rand()%2;
  }
  for(int i=0; i<N; i++){
    S[i] = (unsigned char)i;
  }
  for(int r=0; r<t; r++){
    unsigned char top[N], bottom[N];
    int to=0, bo=0;
    for(int i=0; i<N; i++){
      if(k[(r*N+i)%KEY_LEN]==0){
        top[to]=S[i];
        to++;
      }else{
        bottom[bo]=S[i];
        bo++;
      }
    }
    for(int j=to; j<N; j++){
      top[j]=bottom[j-to];
    }
    for(int j=0; j<N; j++){
      S[j]=top[j];
    }
  }
}

void ksa_sst(void){
  unsigned char k[KEY_LEN], pi[N];
  int marked = 0, ceiling=(int)ceil((N-1)/2);
  for (int i=0; i<KEY_LEN; i++){
    k[i] = rand()%256;
  }
  for(int i=0; i<N; i++){
    S[i] = (unsigned char)i;
    pi[i] = 0;
  }
  int i=0, j=0;
  while(marked<N){
    j = (j+S[i%N]+k[i%KEY_LEN])%N;
    swap(i%N, j%N);
    unsigned char temp = pi[i];
    pi[i]=pi[j];
    pi[j]=temp;
    if(marked<ceiling){
      if(pi[i]==pi[j]||pi[i]==0){
        pi[i]=1;
        marked++;
      }
    }else if((pi[i]!=pi[j]&&pi[i]==0)||(pi[i]==0&&i==j)){
      pi[i]=1;
      marked++;
    }
    i=(i+1)%N;
  }

}

void drop_zeros(unsigned char* result){
  if(N==16){
    unsigned char val1= prga(), val2=prga();
    result[0]=val1*16+val2;
  }else if(N==64){
    unsigned char val1=prga(), val2=prga(), val3=prga(), val4=prga();
    unsigned char tmp=val2/16;
    result[0]=val1*4+tmp;
    val2=val2%16; tmp=val3/4;
    result[1]=val2*16+tmp;
    val3=val3%4;
    result[2]=val3*64+val4;
  }
}

unsigned int mdrop(void){
  unsigned int val = 0U;
  for(int j=0; j<4; j++){
    val*=256;
    if(N==16){
      unsigned char res[1];
      for(int i=0; i<D; i++){
        drop_zeros(res);
      }
      drop_zeros(res);
      val+=res[0];
    }else if(N==256){
      for(int i=0; i<D; i++){
        prga();
      }
      val+=prga();
    }else if(N==64){
      unsigned char res[3];
      int ptr=0, more=0;
      if(tempc<=D){
        more = D-tempc;
        tempc=0;
      }else{
        ptr = D;
      }
      switch(more){
        case 0:
          if(ptr==0&&tempc==0){
            drop_zeros(res);
            val+=res[0];
            tempv[0]=res[1];
            tempv[1]=res[2];
            tempc = 2;
          }else{
            val+=tempv[ptr];
            tempc--;
            if(tempc==1){
              tempv[0]=tempv[1];
            }
          }
          break;
        case 1:
          drop_zeros(res);
          val+=res[1];
          tempv[1]=res[2];
          tempc=1;
          break;
        case 2:
          drop_zeros(res);
          val+=res[2];
          break;
        case 3:
          drop_zeros(res);
          drop_zeros(res);
          val+=res[0];
          tempv[0]=res[1];
          tempv[1]=res[2];
          tempc = 2;
      }
    }
  }
  return val;
}

int main(){
  srand(time(0));
  ksa();
  unif01_Gen* gen = unif01_CreateExternGenBits("RC4", mdrop);
  bbattery_SmallCrush(gen);
  fflush(stdout);
  unif01_DeleteExternGenBits(gen);
  ksa_rs();
  unif01_Gen* genrs = unif01_CreateExternGenBits("RC4rs", mdrop);
  bbattery_SmallCrush(genrs);
  fflush(stdout);
  unif01_DeleteExternGenBits(genrs);
  ksa_sst();
  unif01_Gen* gensst = unif01_CreateExternGenBits("RC4sst", mdrop);
  bbattery_SmallCrush(gensst);
  fflush(stdout);
  unif01_DeleteExternGenBits(gensst);
  return 0;
}
