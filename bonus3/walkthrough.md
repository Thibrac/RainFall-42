Dans ce dernier niveau on a un `main()` qui prend un arg

Il y a une simple comparaison ici:
```sh
0x08048533 <+63>:	cmpl   $0x0,0x9c(%esp)
0x0804853b <+71>:	je     0x8048543 <main+79>
```
On a donc besoin d'un arg mais juste une chaine vide pour atteindre
```sh
0x080485eb      c744240407..   mov dword [var_4h], 0x8048707 ; [0x8048707:4]=0x2f006873
0x080485f3      c704240a87..   mov dword [esp], str._bin_sh ; [0x804870a:4]=0x6e69622f ; "/bin/sh"
0x080485fa      e821feffff     call sym.imp.execl
```
On peut donc `cat /home/user/end/.pass` = `3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c`