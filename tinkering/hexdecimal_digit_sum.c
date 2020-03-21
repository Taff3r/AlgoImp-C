#include<stdio.h>
unsigned int hex_digit_sum(unsigned int);
int main(){
    unsigned int val = 0xABC;
    unsigned int sum = hex_digit_sum(val); // Should be 15
    printf("The hex %x (in decimal %d)has the digit sum of %d", val, val, sum);
    return 0;
}

unsigned int hex_digit_sum(unsigned int hex) {
    unsigned int sum = 0;
    while(hex > 0){
        sum += hex & 0xf;
        hex >>= 0x4;
    }
    return sum;
}
