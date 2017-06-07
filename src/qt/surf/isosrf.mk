FFLAGS = -g
FC = gfortran $(FFLAGS)
FC = gfortran-mp-4.5 $(FFLAGS)
CFLAGS = -g -stdlib=libstdc++
GCC = g++ $(CFLAGS)
CC = clang++ $(CFLAGS)
#INC = -I/usr/local/include/openbabel-2.0/ -I..
INC = -I$(HOME)/Documents/openbabel/include/openbabel-2.0/ -I..
#INC = $(HOME)/openbabel-2.2.2/include/
LIB = -L$(HOME)/Documents/openbabel/lib/ -lopenbabel -L/usr/local/gfortran/lib  -lgfortran

isosrf: isosrf.o isolib.o readgrid.o
	$(CC) -o $@ $^ $(LIB)

isolib.o: isolib.F
	$(FC) -c -o $@ $^

readgrid.o: readgrid.cpp
	$(CC) -c -o $@ $(INC) $^
