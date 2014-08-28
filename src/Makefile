UNAME = $(shell uname)
FLAVOR = "unknown"
ifeq ($(UNAME), MINGW32_NT-5.1)
  FLAVOR = "mingw32"
else
  ifeq ($(UNAME), Linux)
    FLAVOR = "gfortran"
    define lnexe
    cd qt && ln -s ChemDb/dbchem .
    cd qt && ln -s surf/dbsurf .
    endef
    define qmake
    qmake
    endef
  else
    ifeq ($(UNAME), Darwin)
      FLAVOR = "osx"
      define lnexe
      cd qt && ln -s gmol.app/Contents/MacOS/gmol .
      cd qt && ln -s surf/dbsurf.app/Contents/MacOS/dbsurf .
      cd qt && ln -s ChemDb/dbchem.app/Contents/MacOS/dbchem .
      endef
      define qmake
      qmake -spec macx-g++
      endef
    endif
  endif
endif

gmol:
	cd gramps && make -f makefile.$(FLAVOR)
	#cd gramps && make -f makefile.mingw32
	#cd gramps && make -f makefile.osx
	cd qt        && qmake && make
	cd qt/ChemDb && qmake && make
	cd qt/surf && qmake
	# qmake won't make a Makefile capable of this on Darwin; gcc handles gfortran on linux ok, but this works too
	cd qt/surf && gfortran -c isolib.F
	cd qt/surf && make
	$(lnexe)

clean:
	cd qt && make clean
	cd qt/ChemDb && make clean
	cd qt/surf && make clean
	cd gramps && make -f makefile.$(FLAVOR) clean
	#cd gramps && make -f makefile.mingw32 clean
	#cd gramps && make -f makefile.osx clean
	rm -f qt/dbsurf
	rm -f qt/dbchem
	rm -f qt/gmol
	rm -f qt/qtgramps.a
	rm -f qt/qtgramps.so
