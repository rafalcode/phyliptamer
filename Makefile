CC=gcc
CFLAGS=-g -Wall
DCFLAGS=-g -Wall -DDBG
EXES=plptmr plptmr_d

plptmr: plptmr.c
	${CC} ${CFLAGS} -o $@ $^

plptmr_d: plptmr.c
	${CC} ${DCFLAGS} -o $@ $^

.PHONY: clean

clean:
	rm -f ${EXES}
