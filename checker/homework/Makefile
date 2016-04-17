OPTS=-lm -std=gnu99 -D_GNU_SOURCE -Wextra -Wall -lpthread -g
all:
	gcc -c $(OPTS) Fractal.c -o runner1
	gcc -c $(OPTS) Image.c -o runner2
	gcc -c $(OPTS) Main.c -o runner3
	gcc runner1 runner2 runner3 -o fractal $(OPTS)

clean:
	rm -f runner*
	rm -f fractal
	rm -f *.ppm
