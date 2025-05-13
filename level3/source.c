#include <stdlib.h>
#include <stdio.h>

void v() {
    char buffer [520];
  
    fgets(buffer,0x200,stdin);
    printf(buffer);
    if (m == 0x40) { //'m' represente une adresse
        fwrite("Wait what?!\n",1,0xc,stdout);
        system("/bin/sh");
    }
    return;
}

int main(){
    v();
    return(0);
}