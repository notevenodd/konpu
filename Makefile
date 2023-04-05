.PHONY: all clean cleanall examples src

all: examples

examples:
	cd examples && $(MAKE) all

src:
	cd src && $(MAKE) all

clean:
	cd src      && $(MAKE) clean
	cd examples && $(MAKE) clean

cleanall:
	cd src      && $(MAKE) cleanall
	cd examples && $(MAKE) cleanall
