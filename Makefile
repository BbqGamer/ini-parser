SDIR=src
ODIR=build
CC=gcc
CFLAGS = -Wall
LIBS = 

_DEPS = utils.h
DEPS = $(patsubst %,$(SDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bin/program: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f build/*.o bin/*