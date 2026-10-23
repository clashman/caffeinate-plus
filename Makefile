caffeinate+: caffeinate+.c
	gcc -o $@ $< -framework IOKit -framework CoreFoundation

clean:
	rm caffeinate
