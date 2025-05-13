#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char c[68];

void m(void *param_1,int param_2,char *param_3,int param_4,int param_5) {
  time_t tVar1;
  
  tVar1 = time((time_t *)0x0);
  printf("%s - %d\n",c,tVar1);
  return;
}

int main(int ac, char ** av) {
    int *puVar1;
    void *pvVar2;
    int *puVar3;
    FILE *stream;
  
    puVar1 = malloc(8);
    *puVar1 = 1;

    pvVar2 = malloc(8);
    puVar1[1] = pvVar2;

    puVar3 = malloc(8);
    *puVar3 = 2;

    pvVar2 = malloc(8);
    puVar3[1] = pvVar2;

    strcpy((char *)puVar1[1], av[1]);
    strcpy((char *)puVar3[1], av[2]);

    stream = fopen("/home/user/level8/.pass","r");
    fgets(c,0x44,stream);
    puts("~~");
    return 0;
}