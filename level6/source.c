#include <stdlib.h>

typedef void(*func_ptr)(void);

void m(void *param_1,int param_2,char *param_3,int param_4,int param_5) {
  puts("Nope");
  return;
}

void n() {
    system("/bin/cat /home/user/level7/.pass");
    return;
}

void main(int ac, char **av) {
    char *dest;
    func_ptr *puVar1;
  
    dest = malloc(0x40);
    puVar1 = malloc(4);
    *puVar1 = (void *)m;
    strcpy(dest, av[1]);
    (**puVar1)();
    return;
}