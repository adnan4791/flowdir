#include <stdio.h>
#include <math.h>

int main() {
    char str[80];
    for(int i=0;i<8;i++) {
        float x = (float)(i);
        sprintf(&str[i*10],"%f, ",x);
    }
       

    puts(str);
    return 0;
}