
MAKE=make

examples:
	$(MAKE) -C examples
doc:
	$(MAKE) -C doc

.PHONY: examples doc
