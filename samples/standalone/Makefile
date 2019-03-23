TARGET = zscilib
CC     = gcc
CFLAGS = -Wall -I. -I../../include
ODIR   = obj
BINDIR = bin
LIBS   = -lm

_OBJ = main.o matrices.o vectors.o zsl.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	@mkdir -p $(ODIR)
	@echo Compiling $@
	@$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGET)

obj/matrices.o: ../../src/matrices.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/matrices.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/vectors.o: ../../src/vectors.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/vectors.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/zsl.o: ../../src/zsl.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/zsl.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BINDIR)/$@ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	-@rm -rf $(ODIR) $(BINDIR)
