CFLAGS := -nostdlib -Wall -Wextra -Wno-builtin-declaration-mismatch
OBJ := main.o string.o syscall.o
BIN := tinyhttpd

%.o: %.c
	cc -c -o $@ $(CFLAGS) $<

$(BIN): $(OBJ)
	ld $(OBJ) -o $(BIN)

clean: 
	rm -f $(OBJ) $(BIN)

.PHONY: $(BIN) clean
