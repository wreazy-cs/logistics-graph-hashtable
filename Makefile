CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude
SRC     = src/main.c src/graph.c src/hash_table.c src/package.c
TARGET  = lojistik

# Windows icin: 'make' yerine asagidaki komutu da kullanabilirsiniz:
#   gcc -Wall -Iinclude src\main.c src\graph.c src\hash_table.c src\package.c -o lojistik.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
