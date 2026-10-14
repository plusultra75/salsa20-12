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