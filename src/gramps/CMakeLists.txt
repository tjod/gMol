cmake_minimum_required(VERSION 3.5)

set(CMAKE_Fortran_COMPILER "/opt/local/bin/gfortran")

project(gramps LANGUAGES C Fortran)
find_package(OpenGL REQUIRED)

set(CMAKE_Fortran_FLAGS_DEBUG "-g -O0 -gdwarf-2")

#set(CMAKE_C_COMPILER "/opt/local/bin/gcc")
add_compile_options(-ggdb -DGL_SILENCE_DEPRECATION)
# Ensure standard optimizations don't strip GDB symbols
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -O0 -gdwarf-2")

set(SOURCE_FILES
    #headers
        gramps.h
        patterns.h
        glutbitmap.h
    #main
        gramps.c
    #others .F and some .c
	aaxis.F
	advance.F
	argsubs.F
	bblank.F
	bitops.F
	blink.F
	change.F
	charutil.F
	getdsk.F
	closeobj.F
	dumpbuff.F
	ccolor.F
        cio.c
	syscall.F
	contupd.F
	coormul.F
	dash.F
	deassign.F
        display.F
	do.F
	dofile.F
	draw.F
	fix.F
	fnupdate.F
	forget.F
	freeze.F
	get.F
	getcoord.F
	getdev.F
	getfn.F
	getframe.F
	getstr.F
	getval.F
	groupinfo.F
	help.F
	initvar.F
	insert.F
	inten.F
        io.c
	json.F
	grout.F
	sockio.c
	g0csubs.c
	g0subs.F
	loop.F
	makecyl.F
	makesph.c
	makecopy.F
	makegroup.F
	maketfm.F
	matcon.F
	move.F
	namid.F
	norm.F
        openfile.F
	openobj.F
	ortotal.F
	perspect.F
	pick.F
	plot.F
	putdsk.F
	puttfm.F
	raytracer.F
	putwtfm.F
	remove.F
	rename.F
	rebuild.F
	reset.F
	restart.F
	rotate.F
	save.F
	scale.F
	snap.F
	set.F
	sframe.F
	show.F
	showgl.c
	showtree.F
	synonym.F
	telluser.F
	text.F
	textxyz.F
	tident.F
	tpush.F
	translate.F
	objutil.F
	udinit.F
	update.F
	use.F
	utfm.F
	)
	

add_executable(gramps ${SOURCE_FILES})
set(CMAKE_BUILD_TYPE Debug)

#target_link_libraries(gramps PRIVATE OpenGL::OpenGL)
target_link_libraries(gramps PRIVATE "-framework OpenGL -framework GLUT -lreadline")

include(GNUInstallDirs)
install(TARGETS gramps
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
