override VPR=viper-lang/compiler/viper
override LD=ld

override VPRFLAGS += 

override LDFLAGS += \
	-shared

override TARGET := libvpr.so

override VPRFILES:=$(shell cd src && find -L * -type f -name '*.vpr')
override OBJ:=$(addprefix obj/,$(VPRFILES:.vpr=.vpr.o))

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

$(TARGET): $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf obj