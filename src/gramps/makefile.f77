# This file has not been maintained since the advent of gfortran.
# If you absolutely cannot use gfortran, you should start
# with makefile.gfortan and morph it into makefile.f77
F77 = /usr/bin/g77
FFLAGS = -g -O -DUSE_SOCK
#FFLAGS = -g -O -IMesa/include/GL

#CC = /usr/bin/gcc-3.4
#CFLAGS =  -g -IMesa/include -I/usr/X11R6/include
#CFLAGS =  -g -I/home/tjo/Mesa-4.0.4/include
CFLAGS =  -g -DUSE_SOCK
CFLAGS =  -g -DUSE_SOCK -Dlibjpeg

#XLIBS =   -lXi -lXext -lXm -lXt -lX11 -lPW
#XLIBS_s = -lXi -lXext -lXm_s -lXt_s -lX11_s -lPW
#LIBS = -lsphere -lfgl -lfm -lgl_s -lI77

#LIBDIR = -L/home/tjo/Mesa-4.0.4/lib -L/usr/X11R6/lib/
LIBDIR = -L/usr/X11R6/lib/
LIBS = $(LIBDIR) -lreadline -lglut -lGLU -lGL -lpthread -ljpeg

O1= aaxis.o advance.o argsubs.o bblank.o bitops.o blink.o \
change.o charutil.o getdsk.o closeobj.o dumpbuff.o \
ccolor.o syscall.o contupd.o coormul.o dash.o deassign.o display.o\
do.o dofile.o draw.o fix.o fnupdate.o forget.o freeze.o \
get.o getcoord.o getdev.o getfn.o getframe.o getstr.o \
getval.o groupinfo.o help.o initvar.o insert.o inten.o \
json.o grout.o sockio.o g0csubs.o g0subs.o cio.o

O2=loop.o makecyl.o makesph.o makecopy.o makegroup.o maketfm.o \
matcon.o move.o namid.o norm.o \
openobj.o ortotal.o perspect.o pick.o plot.o putdsk.o puttfm.o raytracer.o \
putwtfm.o remove.o rename.o rebuild.o reset.o restart.o rotate.o save.o \
scale.o snap.o \
set.o sframe.o show.o showgl.o showtree.o synonym.o telluser.o text.o \
textxyz.o tident.o tpush.o translate.o objutil.o udinit.o update.o use.o utfm.o

O3=net_openfile.o net_io.o 
O4=openfile.o io.o

gluttest: $(O1) $(O2) $(O4) gluttest.o
	$(CC) -o $@ gluttest.o  $(LIBS) -lm

glutskel: $(O1) $(O2) $(O4) glutskel.o
	$(CC) -o $@ glutskel.o  $(LIBS) -lm

glutskel2: $(O1) $(O2) $(O4) glutskel2.o
	$(CC) -o $@ glutskel2.o  $(LIBS) -lm

gramps: $(O1) $(O2) $(O4) gramps.o
	$(F77) -o $@.exe gramps.o $(O1) $(O2) $(O4) $(LIBS) $(XLIBS_s)
	strip $@.exe

mtk_display: $(O1) $(O2) $(O4) mtk_display.o
	$(F77) -o $@ mtk_display.o $(O1) $(O2) $(O4) $(LIBS) -lsphere -lfgl -lfm -lgl_s $(XLIBS_s)  -lI77
	strip $@

gramps.deb: $(O1) $(O2) $(O4) gramps.o
	$(F77) -o $@ -g -O1 gramps.o $(O1) $(O2) $(O4) $(LIBS) $(XLIBS)

gramps.prof: $(O1) $(O2) $(O4) gramps.o
	$(F77) -o $@ -g -p -O1 gramps.o $(O1) $(O2) $(O4) $(LIBS) -lfgl -lsphere -lfm -lgl

demo:   $(O1) $(O2) $(O4) demo.o
	$(F77) -o $@ -O1 demo.o $(O1) $(O2) $(O4) $(LIBS) -lfgl -lsphere -lfm -lgl_s
	strip $@

net_gramps: $(O1) $(O2) $(O3) gramps.o
	$(F77) -o $@ -O1 gramps.o $(O1) $(O2) $(O3) $(LIBS) -ldn -lfgl -lsphere -lfm -lgl_s $(XLIBS_s) 
	strip $@

net_gramps.deb: $(O1) $(O2) $(O3) gramps.o
	$(F77) -o $@ -g -O1 gramps.o $(O1) $(O2) $(O3) $(LIBS) -ldn -lfgl -lsphere -lfm -lgl_s

mtk_display.o: gramps.c
	$(CC) -c -DMTK_DISPLAY -o $@ gramps.c

net_openfile.o: openfile.F
	$(F77) -o $@ $(FFLAGS) -c openfile.F

cmd: cmd.o sockio.o
	$(CC) -o $@ cmd.o sockio.o -lreadline

sockclient: sockclient.o sockio.o
	$(F77) -o $@ sockclient.o sockio.o

sockserver: sockserver.o sockio.o
	$(F77) -o $@ sockserver.o sockio.o

sockrtest: sockrtest.o sockio.o
	$(F77) -o $@ sockrtest.o sockio.o

do.o: COMMANDS.FOR
help.o: COMMANDS.FOR
initvar.o: COMMANDS.FOR
dofile.o: DEFVAL.FOR
initvar.o: DEFVAL.FOR
gramps.o: DEFVAL.FOR
openobj.o: DEFVAL.FOR
fnupdate.o :DEFVAL.FOR
reset.o: DEFVAL.FOR
save.o: DEFVAL.FOR
udinit.o: DEFVAL.FOR
dofile.o: DEFVAL.OLD
initvar.o: DEFVAL.OLD
gramps.o: DEFVAL.OLD
openobj.o: DEFVAL.OLD
reset.o: DEFVAL.OLD
save.o: DEFVAL.OLD
udinit.o: DEFVAL.OLD
argsubs.o: DOLLARGS.FOR
dofile.o: DOLLARGS.FOR
initvar.o: DOLLARGS.FOR
pick.o: DOLLARGS.FOR
show.o: DOLLARGS.FOR
aaxis.o: ERRORS.FOR
advance.o: ERRORS.FOR
argsubs.o: ERRORS.FOR
bblank.o: ERRORS.FOR
blink.o: ERRORS.FOR
change.o: ERRORS.FOR
color.o: ERRORS.FOR
dash.o: ERRORS.FOR
deassign.o: ERRORS.FOR
display.o: ERRORS.FOR
do.o: ERRORS.FOR
dofile.o: ERRORS.FOR
dumpbuff.o: ERRORS.FOR
fix.o: ERRORS.FOR
forget.o: ERRORS.FOR
freeze.o: ERRORS.FOR
get.o: ERRORS.FOR
getcoord.o: ERRORS.FOR
getdev.o: ERRORS.FOR
getdsk.o: ERRORS.FOR
getfn.o: ERRORS.FOR
getframe.o: ERRORS.FOR
getstr.o: ERRORS.FOR
getval.o: ERRORS.FOR
help.o: ERRORS.FOR
insert.o: ERRORS.FOR
inten.o: ERRORS.FOR
makecopy.o: ERRORS.FOR
makegroup.o: ERRORS.FOR
move.o: ERRORS.FOR
namid.o: ERRORS.FOR
openfile.o: ERRORS.FOR
net_openfile.o: ERRORS.FOR
openobj.o: ERRORS.FOR
perspect.o: ERRORS.FOR
pick.o: ERRORS.FOR
remove.o: ERRORS.FOR
rename.o: ERRORS.FOR
reset.o: ERRORS.FOR
rebuild.o: ERRORS.FOR
restart.o: ERRORS.FOR
rotate.o: ERRORS.FOR
save.o: ERRORS.FOR
scale.o: ERRORS.FOR
set.o: ERRORS.FOR
sframe.o: ERRORS.FOR
show.o: ERRORS.FOR
showtree.o: ERRORS.FOR
synonym.o: ERRORS.FOR
text.o: ERRORS.FOR
textxyz.o: ERRORS.FOR
translate.o: ERRORS.FOR
use.o: ERRORS.FOR
telluser.o: ERRTXT.FOR ERRORS.FOR
dofile.o: FILES.FOR
dumpbuff.o: FILES.FOR
get.o: FILES.FOR
getdsk.o: FILES.FOR
getframe.o: FILES.FOR
initvar.o: FILES.FOR
openfile.o: FILES.FOR
net_openfile.o: FILES.FOR
set.o: FILES.FOR
show.o: FILES.FOR
udinit.o: FILES.FOR
display.o: G0COMMON.FOR
g0subs.o: G0COMMON.FOR
set.o: G0COMMON.FOR
udinit.o: G0COMMON.FOR
raytracer.o: G0COMMON.FOR
plot.o: G0COMMON.FOR
color.o: GETARGS.FOR
inten.o: GETARGS.FOR
move.o: GETARGS.FOR
perspect.o: GETARGS.FOR
rotate.o: GETARGS.FOR
scale.o: GETARGS.FOR
sframe.o: GETARGS.FOR
translate.o: GETARGS.FOR
coormul.o: TFMMAT.FOR
display.o: TFMMAT.FOR
fix.o: TFMMAT.FOR
maketfm.o: TFMMAT.FOR
norm.o: TFMMAT.FOR
set.o: TFMMAT.FOR
tident.o: TFMMAT.FOR
tpush.o: TFMMAT.FOR
udinit.o: TFMMAT.FOR
utfm.o: TFMMAT.FOR
aaxis.o: UDLIST.FOR
advance.o: UDLIST.FOR 
argsubs.o: UDLIST.FOR  
bblank.o: UDLIST.FOR  
blink.o: UDLIST.FOR  
change.o: UDLIST.FOR  
closeobj.o: UDLIST.FOR  
color.o: UDLIST.FOR  
contupd.o: UDLIST.FOR  
coormul.o: UDLIST.FOR  
dash.o: UDLIST.FOR  
deassign.o: UDLIST.FOR  
display.o: UDLIST.FOR  
do.o: UDLIST.FOR  
dofile.o: UDLIST.FOR  
dumpbuff.o: UDLIST.FOR  
fix.o: UDLIST.FOR  
fnupdate.o: UDLIST.FOR  
forget.o: UDLIST.FOR  
freeze.o: UDLIST.FOR  
get.o: UDLIST.FOR  
getcoord.o: UDLIST.FOR  
getdev.o: UDLIST.FOR  
getdsk.o: UDLIST.FOR  
getfn.o: UDLIST.FOR  
getframe.o: UDLIST.FOR  
getstr.o: UDLIST.FOR  
getval.o: UDLIST.FOR  
gramps.o: UDLIST.FOR  
g0subs.o: UDLIST.FOR
groupinfo.o: UDLIST.FOR  
help.o: UDLIST.FOR  
initvar.o: UDLIST.FOR  
insert.o: UDLIST.FOR  
inten.o: UDLIST.FOR  
makecopy.o: UDLIST.FOR  
makegroup.o: UDLIST.FOR  
maketfm.o: UDLIST.FOR  
move.o: UDLIST.FOR  
namid.o: UDLIST.FOR  
openobj.o: UDLIST.FOR  
ortotal.o: UDLIST.FOR  
perspect.o: UDLIST.FOR  
pick.o: UDLIST.FOR  
puttfm.o: UDLIST.FOR  
putwtfm.o: UDLIST.FOR  
raytracer.o: UDLIST.FOR  
remove.o: UDLIST.FOR  
rename.o: UDLIST.FOR  
reset.o: UDLIST.FOR  
rebuild.o: UDLIST.FOR  
restart.o: UDLIST.FOR  
rotate.o: UDLIST.FOR  
save.o: UDLIST.FOR  
scale.o: UDLIST.FOR  
set.o: UDLIST.FOR  
sframe.o: UDLIST.FOR  
show.o: UDLIST.FOR  
showtree.o: UDLIST.FOR  
synonym.o: UDLIST.FOR  
telluser.o: UDLIST.FOR  
text.o: UDLIST.FOR  
textxyz.o: UDLIST.FOR  
translate.o: UDLIST.FOR  
objutil.o: UDLIST.FOR  
udinit.o: UDLIST.FOR  
update.o: UDLIST.FOR  
use.o: UDLIST.FOR  
