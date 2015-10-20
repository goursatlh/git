
con: condition.o
	gcc -o con condition.o -lpthread
all: workcrew.o control.o queue.o
	gcc -o all workcrew.o control.o queue.o -lpthread
workcrew.o: workcrew.c
	gcc -c workcrew.c
control.o: control.c
	gcc -c control.c
queue.o: queue.c
	gcc -c queue.c
condition.o:
	gcc -c condition.c

clean:
	rm -f all *.o *~
	
