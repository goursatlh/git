
PROGS =	sig
CLEANFILES = *.o core*

all:	${PROGS}

sig:	sig.o
	${CC} ${CFLAGS} -o $@ sig.o ${LIBS}


clean:
	rm -f ${PROGS} ${CLEANFILES}
