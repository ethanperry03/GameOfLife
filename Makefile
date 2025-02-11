GXX = gcc
CFLAGS = -pedantic -g -Wall -Wvla -Werror -Wno-error=unused-variable
OFILES = gol.o gol_cmd.o gol_io.o gol_sim.o

all: print

print: $(OFILES)
	$(GXX) $(CFLAGS) gol.o gol_cmd.o gol_io.o gol_sim.o -o print

gol.o: gol.c gol_cmd.h gol_io.h gol_sim.h
	$(GXX) $(CFLAGS) gol.c -c

gol_cmd.o: gol_cmd.c gol_cmd.h
	$(GXX) $(CFLAGS) gol_cmd.c -c

gol_io.o: gol_io.c gol_io.h
	$(GXX) $(CFLAGS) gol_io.c -c

gol_sim.o: gol_sim.c gol_sim.h gol_io.h
	$(GXX) $(CFLAGS) gol_sim.c -c

clean:
	rm print *.o *~