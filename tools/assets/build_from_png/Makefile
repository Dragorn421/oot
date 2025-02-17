CFLAGS := -Wall -O3

default: build_from_png

clean:
	$(RM) build_from_png

.PHONY: default clean

build_from_png: build_from_png.c ../n64texconv/libn64texconv.a
	$(CC) $(CFLAGS) -o $@ $^ -fopenmp -lz -lm
