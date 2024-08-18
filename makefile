CFLAGS=-Wall -c -O2

OBJS= bigcalc.o bigio.o bigmath.o bigmisc.o bigprint.o conio32.o

all: bigcalc.exe

clean:
	rm -f *.exe *.o *~

bigcalc.exe: $(OBJS)
	gcc -s $^ -o $@

bigcalc.o: bigcalc.c bigcalc.h biggvar.h
	gcc $(CFLAGS) $< -o $@

bigio.o: bigio.c conio32.h bigcalc.h biggvar.h
	gcc $(CFLAGS) $< -o $@

bigmath.o: bigmath.c bigcalc.h biggvar.h
	gcc $(CFLAGS) $< -o $@

bigmisc.o: bigmisc.c bigcalc.h biggvar.h
	gcc $(CFLAGS) $< -o $@

bigprint.o: bigprint.c bigcalc.h biggvar.h
	gcc $(CFLAGS) $< -o $@

conio32.o: conio32.c conio32.h
	gcc $(CFLAGS) $< -o $@

