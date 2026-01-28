CC = gcc
CFLAGS = -Wall -Wextra -std=c11		# just boilerplate ig
TARGET = ./build/carapace 			# this is where the binaries goes

all: src/main.c
	@mkdir -p build
	$(CC) $(CFLAGS) src/main.c -o $(TARGET)


clean:
	rm -rf build 					# To clear binary files 