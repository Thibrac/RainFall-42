#include <stdlib.h>
#include <stdio.h>

void p(char *param_1) {
    printf(param_1);
    return;
}

void n() {
    char buffer [520];

    fgets(buffer,0x200,stdin);
    p(buffer);
    if (m == 0x1025544) { //'m' represente une adresse
        system("/bin/cat /home/user/level5/.pass");
    }
    return;
}

int main() {
  n();
  return(0);
}