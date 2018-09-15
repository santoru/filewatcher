UNAME_S := $(shell uname -s)

fresh:
	mkdir -p bin
	cd lib && rm -f *.gch

ifeq "$(UNAME_S)" "FreeBSD"
	cc -lutil -lbsm filewatcher.c lib/*.c -o bin/filewatcher
endif

ifeq "$(UNAME_S)" "Darwin"
	gcc -lbsm filewatcher.c lib/*.c -o bin/filewatcher
endif

clean:
	rm -f lib/*.gch
	rm -rf bin
