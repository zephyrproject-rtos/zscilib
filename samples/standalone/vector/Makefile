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

_OBJ = main.o matrices.o vectors.o statistics.o probability.o quaternions.o euler.o ahrs.o zsl.o
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

$(ODIR)/statistics.o: $(BASEDIR)/src/statistics.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/statistics.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/probability.o: $(BASEDIR)/src/probability.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/probability.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/quaternions.o: $(BASEDIR)/src/orientation/quaternions.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/quaternions.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/euler.o: $(BASEDIR)/src/orientation/euler.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/euler.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/ahrs.o: $(BASEDIR)/src/orientation/ahrs.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/ahrs.o
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
