#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define _GNU_SOURCE

int main(int ac, char **av) {
    int input;
    char *path[2];
    uid_t uid;
    gid_t gid;
  
    input = atoi(av[1]);

    if (input == 0x1a7) {
        path[0] = strdup("/bin/sh");
        path[1] = NULL;
        gid = getegid();
        uid = geteuid();
        setresgid(gid,gid,gid);
        setresuid(uid,uid,uid);
        execv("/bin/sh", path);
    } else {
        fwrite("No !\n",1,5, stderr);
    }
    return 0;
}