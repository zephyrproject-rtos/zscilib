TARGET = zscilib
CC     = gcc
CFLAGS = -Wall -Wconversion -Wno-sign-conversion -I. -I../../include
ODIR   = obj
BINDIR = bin
LIBS   = -lm

# Additional build flags used by zscilib, since we don't have access to the
# normal Zephyr KConfig system to define these and set default values.
CFLAGS += -DCONFIG_ZSL_MATRIX_QRD_USE_SCRATCH
CFLAGS += -DCONFIG_ZSL_MATRIX_QRD_SCRATCH_SIZE=100

_OBJ = main.o matrices.o vectors.o zsl.o
_OBJ += atomic.o dynamics.o eleccomp.o electric.o energy.o fluids.o gases.o
_OBJ += gravitation.o kinematics.o magnetics.o mass.o misc.o momentum.o
_OBJ += optics.o photons.o projectiles.o relativity.o rotation.o sound.o
_OBJ += thermo.o waves.o work.o
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

obj/atomic.o: ../../src/physics/atomic.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/atomic.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/dynamics.o: ../../src/physics/dynamics.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/dynamics.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/eleccomp.o: ../../src/physics/eleccomp.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/eleccomp.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/electric.o: ../../src/physics/electric.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/electric.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/energy.o: ../../src/physics/energy.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/energy.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/fluids.o: ../../src/physics/fluids.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/fluids.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/gases.o: ../../src/physics/gases.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/gases.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/gravitation.o: ../../src/physics/gravitation.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/gravitation.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/kinematics.o: ../../src/physics/kinematics.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/kinematics.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/magnetics.o: ../../src/physics/magnetics.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/magnetics.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/mass.o: ../../src/physics/mass.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/mass.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/misc.o: ../../src/physics/misc.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/misc.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/momentum.o: ../../src/physics/momentum.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/momentum.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/optics.o: ../../src/physics/optics.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/optics.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/photons.o: ../../src/physics/photons.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/photons.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/projectiles.o: ../../src/physics/projectiles.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/projectiles.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/relativity.o: ../../src/physics/relativity.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/relativity.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/rotation.o: ../../src/physics/rotation.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/rotation.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/sound.o: ../../src/physics/sound.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/sound.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/thermo.o: ../../src/physics/thermo.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/thermo.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/waves.o: ../../src/physics/waves.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/waves.o
	@$(CC) -c -o $@ $< $(CFLAGS)

obj/work.o: ../../src/physics/work.c
	@mkdir -p $(ODIR)
	@echo Compiling $(ODIR)/work.o
	@$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BINDIR)/$@ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	-@rm -rf $(ODIR) $(BINDIR)
