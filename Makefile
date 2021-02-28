CC=g++
CFLAGS=-Wall
LIBS=-lGL -lglfw -lGLEW

OpenGLapp: src/app.o
	$(CC) src/app.o -o app $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f app src/*.o
