.PHONY: all lib tests run_tests clean

all: tests

lib:
	$(MAKE) -C lib

tests: lib
	$(MAKE) -C tests

run_tests: tests
	cd tests && LD_LIBRARY_PATH=../lib ./vstd_tests

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C tests clean