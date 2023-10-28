BIN := tinyhttpd
OBJ := main.o string.o

%.o: %.S
	as -o $@ $<

$(BIN): $(OBJ)
	ld $(OBJ) -o $(BIN)

clean: 
	rm -f $(OBJ) $(BIN)

.PHONY: $(BIN) clean
