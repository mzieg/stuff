SHELL = /bin/bash
APP   = sudoku

default: $(APP)

clean:
	@rm -f $(APP) *.o *.exe

$(APP): $(APP).o Board.o Possibilities.o
	g++ -o $@ $^
