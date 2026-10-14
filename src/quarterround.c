#include <stdio.h>
#include <stdint.h>
// Ham xoay trai mot so nguyen 32 bit n vi tri
uint32_t rotl32(uint32_t x,unsigned int n){
    n=n%32;// Dua so vi tri xoay ve khoang 0 den 31
    if(n==0){
        return x;
    }else{
        return(x<<n|x>>(32-n));// Dich trai va lay lai cac bit bi day ra
    }
}
//Ham bien doi truc tiep 4 phan tu cua mang x
void quarterround(uint32_t x[4]){
    // Buoc 1: Cong x[0] va x[3], xoay trai 7 bit,
    // sau do XOR voi x[1]
    x[1]=x[1]^rotl32(x[0]+x[3],7);
    //Buoc 2: Su dung gia tri x[1] moi de cap nhat x[2]
    x[2]=x[2]^rotl32(x[1]+x[0],9);
    // Buoc 3: Su dung x[2] va x[1] de cap nhat x[3]
    x[3]=x[3]^rotl32(x[2]+x[1],13);
    // Buoc 4: Su dung x[3] moi va x[2] de cap nhat x[0]
    x[0]=x[0]^rotl32(x[3]+x[2],18);
}