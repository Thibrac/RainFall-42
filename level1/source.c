#include <stdlib.h>
#include <stdio.h>

int main() {
    char buffer[76];

    gets(buffer);
    return(0);
}

void run() {
    fwrite("Good... Wait what?\n",19,1,stdout);
    system("/bin/sh");
    return;
}
