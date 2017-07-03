fresh:
	mkdir -p bin
	-cd lib && rm *.gch
	gcc -lbsm filewatcher.c lib/*.c -o bin/filewatcher
