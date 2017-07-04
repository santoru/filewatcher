fresh:
	mkdir -p bin
	-cd lib && rm *.gch
	gcc -lbsm filewatcher.c lib/*.c -o bin/filewatcher

clean:
	rm -f lib/*.gch
	rm -rf bin