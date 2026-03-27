CC = gcc
CFLAGS = -Wall -Wextra -std=c11	 -Iinclude	# just boilerplatee
TARGET = ./build/carapace 			# this is where the binaries goes

all: src/main.c
	@mkdir -p build
	$(CC) $(CFLAGS) src/main.c src/cgroups.c src/namespaces.c src/container.c -o $(TARGET)


clean:
	rm -rf build 					# To clear binary files 