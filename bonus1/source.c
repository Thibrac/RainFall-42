#include <stdlib.h>

int main(int param_1, char **av) {
    int ret;
    char str[40];
    int nb;
  
    nb = atoi(av[1]);
    if (nb < 10) {
        memcpy(str, av[2], nb * 4);
        if (nb == 0x574f4c46) {
            execl("/bin/sh","sh",0);
            }
        ret = 0;
    } else {
        ret = 1;
    }
    return ret;
}