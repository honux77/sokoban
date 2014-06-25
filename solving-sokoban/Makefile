#CC = gcc
CC = cc
#CFLAGS = -O2 -msupersparc
#CFLAGS = -g -p

solve: main.o soukocheck.o soukoelem.o soukoio.o soukomove.o soukopre.o soukobestf.o soukosim4.o soukodb.o\
 soukodef.h extern.h
	$(CC) $(CFLAGS) main.o soukocheck.o soukoelem.o soukoio.o soukomove.o soukopre.o\
 soukobestf.o soukosim4.o soukodb.o -o solve

main.o: main.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c main.c

soukocheck.o: soukocheck.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c soukocheck.c

soukoelem.o: soukoelem.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c soukoelem.c

soukoio.o: soukoio.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c soukoio.c

soukomove.o: soukomove.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c soukomove.c

soukopre.o: soukopre.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c soukopre.c

soukobestf.o: soukobestf.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c soukobestf.c

soukosim4.o: soukosim4.c soukodef.h extern.h
	 $(CC) $(CFLAGS) -c soukosim4.c

soukodb.o: soukodb.c soukodef.h extern.h
	$(CC) $(CFLAGS) -c soukodb.c

install: solve
	cp -p solve ${HOME}/bin/solve
clean:
	rm -f *.o *.out *~ core solve 
