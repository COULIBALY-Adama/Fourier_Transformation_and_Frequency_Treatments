
OB += \
frequencies.o \
filters.o
	

%.o:
	g++  -O0 -g3 -Wall -c  frequencies.cpp filters.cpp `pkg-config --cflags --libs opencv`

all:$(OB)
	@echo 'Building target: $@'
	g++  -o "filters" $(OB)  `pkg-config --cflags --libs opencv`
	@echo 'Finished building target: $@'

clear:
	rm -rf *.o

mproper: clean
	rm -rf $(OB) frequencies

