FFLAGS = -g
FC = gfortran $(FFLAGS)
CFLAGS = -g
GCC = g++ $(CFLAGS)
INC = -I/usr/local/include/openbabel-2.0/ -I..
#INC = $(HOME)/openbabel-2.2.2/include/
LIB = -lopenbabel

isosrf: isosrf.o isolib.o readgrid.o
	$(FC) -o $@ $^ $(LIB)

isolib.o: isolib.F
	$(FC) -c -o $@ $^

readgrid.o: readgrid.cpp
	$(GCC) -c -o $@ $(INC) $^
