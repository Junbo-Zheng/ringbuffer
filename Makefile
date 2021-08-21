cxx = clang

prom = main
deps = $(shell find ./ -name "*.h")
# src = $(shell find ./ -name "*.c")
src = main.c
src += ./src/ringbuffer.c

obj = $(src:%.c=%.o)
LIBS:= -std=c17
CFLAGS:=-g -Wall -O0 -fsanitize=address -fno-omit-frame-pointer -ggdb

$(prom): $(obj)
	$(cxx) -o $(prom) $(obj) $(LIBS) $(CFLAGS) $(LIBS)

%.o: %.c $(deps)
	$(cxx) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -rf $(prom)
	rm -rf *.o
	rm -rf ./src/*.o
