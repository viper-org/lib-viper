override VPR=viper-lang/compiler/viper
override VAS=viper-lang/_deps/vasm-build/compiler/vasm
override LD=ld

override VPRFLAGS += \
	-I ./src

override VASFLAGS += \
	-felf

override LDFLAGS += \
	-shared

override TARGET := libvpr.so

override VPRFILES:=$(shell cd src && find -L * -type f -name '*.vpr')
override VASFILES:=$(shell cd src && find -L * -type f -name '*.vas')
override OBJ:=$(addprefix obj/,$(VPRFILES:.vpr=.vpr.o) $(VASFILES:.vas=.vas.o))

.PHONY: all
all: $(TARGET)

.PHONY: viper
viper:
	cd viper-lang && git pull
	cd viper-lang && cmake .
	cd viper-lang && cmake --build . -j

obj/%.vpr.o: src/%.vpr viper Makefile
	mkdir -p "$$(dirname $@)"
	$(VPR) -o $@ $(VPRFLAGS) $<

obj/%.vas.o: src/%.vas viper Makefile
	mkdir -p "$$(dirname $@)"
	$(VAS) $< -o $@ $(VASFLAGS)

$(TARGET): $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf obj