#include <stdio.h>
#include <stdint.h>

uint32_t rotl32(uint32_t x,unsigned int n){
    n=n%32;
    if(n==0){
        return x;
    }else{
        return(x<<n|x>>(32-n));
    }
}
void quarterround(uint32_t x[4]){
    x[1]=x[1]^rotl32(x[0]+x[3],7);
    x[2]=x[2]^rotl32(x[1]+x[0],9);
    x[3]=x[3]^rotl32(x[2]+x[1],13);
    x[0]=x[0]^rotl32(x[3]+x[2],18);
}