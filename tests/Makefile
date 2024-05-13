override CXX=g++-13
override LD=g++-13

override CXXFLAGS += \
	-std=c++20 \
	-Iinclude

override LDFLAGS += \
	-L../lib \
	-lvpr

override TARGET := vstd_tests

override CXXFILES:=$(shell cd src && find -L * -type f -name '*.cpp')
override OBJ:=$(addprefix obj/,$(CXXFILES:.cpp=.cpp.o))
override HEADER_DEPS:=$(addprefix obj/,$(CXXFILES:.cpp=.cpp.d))

.PHONY: all
all: $(TARGET)

-include $(HEADER_DEPS)

obj/%.cpp.o: src/%.cpp Makefile
	mkdir -p "$$(dirname $@)"
	$(CXX) -o $@ $(CXXFLAGS) -c $<

$(TARGET): $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(LD) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf obj