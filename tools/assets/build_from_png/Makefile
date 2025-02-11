CFLAGS := -Wall -O2 -g

default: build_from_png

clean:
	$(RM) build_from_png

.PHONY: default clean

build_from_png: build_from_png.c
	$(CC) $(CFLAGS) -o $@ $< ../../n64texconv/libn64texconv.a
