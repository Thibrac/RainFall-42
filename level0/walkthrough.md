Pour ce premier niveau en se connectant on voit un executable ```level0```

Il segfault sans param donc on va le passer dans gdb avec nimporte quel param ```gdb ./level0```

Ensuite on disas le main et on voit la comparaison du param ici ```0x08048ed9 <+25>:	cmp    $0x1a7,%eax```

On convertit la valeur ```0x1a7``` en deci qui donne ```423```

On run le prog a nouveau avec ```./level0 423``` qui nous ouvre un invit de commande en tant que ```uid=2030(level1)```

On va simplement executer la commande ```cat /home/user/level1/.pass``` pour trouver le flag ```1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a```