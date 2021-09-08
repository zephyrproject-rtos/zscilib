BASEDIR = ../../..
TARGET  = zscilib
CC      = gcc
CFLAGS  = -Wall -Wconversion -Wno-sign-conversion -I. -I$(BASEDIR)/include
ODIR    = obj
BINDIR  = bin
LIBS    = -lm

# Optionally enable ARM THUMB-2 ASM optimised functions
# CFLAGS += -DCONFIG_ZSL_PLATFORM_OPT=2

# Optionally force single-precision floats (default is double)
# CFLAGS += -DCONFIG_ZSL_SINGLE_PRECISION=y

_OBJ = main.o matrices.o vectors.o zsl.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	@mkdir -p $(ODIR)
	@echo Compiling $@
	@$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGET)

$(ODIR)/matrices.o: $(BASEDIR)/src/matrices.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/matrices.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/vectors.o: $(BASEDIR)/src/vectors.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/vectors.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/zsl.o: $(BASEDIR)/src/zsl.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/zsl.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BINDIR)/$@ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	-@rm -rf $(ODIR) $(BINDIR)
