#include <stdio.h>
unsigned int binary_digit_sum(unsigned int val);

int main(){
    unsigned int val = 12;
    unsigned int sum = binary_digit_sum(val);
    printf("Number of 1s in number value %d, is: %d  \n", val, sum);
    return 0;
}

unsigned int binary_digit_sum(unsigned int val){
    unsigned int sum = 0;
    while(val > 0 ){
        sum += val & 1;
        val >>= 1;
    }
    return sum;
}
