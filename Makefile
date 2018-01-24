
.PHONEY: clean

csdir ?= d:/OpenSource/ChezScheme-git
m ?= ti3mw
CFLAGS ?= -g -O0

bootpath = $(csdir)/$m/boot/$m
psboot = $(bootpath)/petite.boot
csboot = $(bootpath)/scheme.boot
kernel = $(bootpath)/kernel.o
binpath = $(csdir)/$m/bin/$m
scmexe = $(binpath)/scheme

CFLAGS += $(shell echo '(include "utils.ss") (format (current-output-port) "-m~a" (machine-bits))' | $(scmexe) -q -b $(psboot))

compile-chez-program: compile-chez-program.ss chez.a
	$(scmexe) -b ./boot --compile-imported-libraries --program $< $<

chez.a: embed_target.o boot.o $(kernel)
	ar rcs $@ $^

embed_target.o: embed_target.c
	gcc -c -o $@ $< -I$(bootpath) -Wall -Wextra -pedantic $(CFLAGS)

boot.o: boot.s boot
	gcc -c $(CFLAGS) $<

boot.s:
	echo '(include "utils.ss") (build-assembly-file "boot.s" "chezschemebootfile" "boot")' | $(scmexe) -q -b $(psboot)

boot: $(psboot) $(csboot)
	echo '(make-boot-file "boot" (list) "$(psboot)" "$(csboot)")' | "$(scmexe)" -q -b "$(psboot)" -b "$(csboot)"

clean:
	rm -f compile-chez-program boot chez.a *.s *.o *.chez *.so *.wpo

