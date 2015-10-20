
PROGS =	sh sh1 sh2 sh3
CLEANFILES = *.o core* *~ *.swp

all:	${PROGS}

sh:	share.o
	${CC} ${CFLAGS} -g share.c  -o $@ ${LIBS}
sh1:    share1.o
	${CC} ${CFLAGS} -g share1.c -o $@ ${LIBS}

sh2:    share2.o
	${CC} ${CFLAGS} -g share2.c -o $@ ${LIBS}

sh3:    share3.o
	${CC} ${CFLAGS} -g share3.c -o $@ ${LIBS}
clean:
	rm -f ${PROGS} ${CLEANFILES} 
