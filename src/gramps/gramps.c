#include "gramps.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <stdio.h>
#include <readline/readline.h>

#define GETCMD_READLINE 1
#define GETCMD_STDIO 2
#define GETCMD_SOCKET 3

#define NO_OUT        0
#define STANDARD_OUT  1
#define DECNET_OUT    2
#define SOCKET_OUT    3
#define COMMAND_OUT   4
#define STANDARD_ERR  5

static int chosen_cursor;
#define NORMAL_CURSOR  0
#define PICK_CURSOR    1
#define WAIT_CURSOR    2
#define STICKY_CURSOR  3
#define RESTORE_CURSOR 4
#define MOUSE_CURSOR_0 100
#define MOUSE_CURSOR_1 101
#define MOUSE_CURSOR_2 102
#define MOUSE_CURSOR_3 103
#define MOUSE_CURSOR_4 104
#define MOUSE_CURSOR_5 105
#define MOUSE_CURSOR_6 106
#define MOUSE_CURSOR_7 107

#define KEY_ON 1
#define KEY_OFF 0
#define KEY_STICKY -1 
static int keytype=KEY_ON;
static int keystate=0;

static int mouse_state=0;
static int lastx=0;
static int lasty=0;
static int currx=0;
static int curry=0;
static int glwin_width=500;
static int glwin_height=500;
static int glwin_xorig=0;
static int glwin_yorig=0;
//static int Nadvance=0;
static int display_changed=0;
static int outflag=STANDARD_OUT;
static char *gr_cmd;
static int cmd_flag = 0; // communicate between i/o thread and Idler function
static int waiting = 0;
char title[] =" \tGramps v3.1";

void choose_cursor(int which)
{
static int current_cursor;

 if(which == NORMAL_CURSOR || which == MOUSE_CURSOR_0) {
  glutSetCursor(GLUT_CURSOR_INHERIT);
  current_cursor = GLUT_CURSOR_INHERIT;
  chosen_cursor = which;
 } else if(which == STICKY_CURSOR) {
  glutSetCursor(GLUT_CURSOR_INFO);
  current_cursor = GLUT_CURSOR_INFO;
  chosen_cursor = which;
 } else if(which == PICK_CURSOR) {
  glutSetCursor(GLUT_CURSOR_CROSSHAIR);
  chosen_cursor = which;
 } else if(which == WAIT_CURSOR) {
  glutSetCursor(GLUT_CURSOR_NONE);
  chosen_cursor = which;
 } else if(which == RESTORE_CURSOR) {
  glutSetCursor(current_cursor);
  chosen_cursor = which;
/*
 } else if(which == MOUSE_CURSOR_1) {
  glutSetCursor(GLUT_CURSOR_BOTTOM_RIGHT_CORNER);
  current_cursor = GLUT_CURSOR_BOTTOM_RIGHT_CORNER;
 } else if(which == MOUSE_CURSOR_2) {
  glutSetCursor(GLUT_CURSOR_BOTTOM_SIDE);
  current_cursor = GLUT_CURSOR_BOTTOM_SIDE;
 } else if(which == MOUSE_CURSOR_3) {
  glutSetCursor(GLUT_CURSOR_TOP_RIGHT_CORNER);
  current_cursor = GLUT_CURSOR_TOP_RIGHT_CORNER;
 } else if(which == MOUSE_CURSOR_4) {
  glutSetCursor(GLUT_CURSOR_BOTTOM_LEFT_CORNER);
  current_cursor = GLUT_CURSOR_BOTTOM_LEFT_CORNER;
 } else if(which == MOUSE_CURSOR_5) {
  glutSetCursor(GLUT_CURSOR_UP_DOWN);
  current_cursor = GLUT_CURSOR_UP_DOWN;
 } else if(which == MOUSE_CURSOR_6) {
  glutSetCursor(GLUT_CURSOR_TOP_LEFT_CORNER);
  current_cursor = GLUT_CURSOR_TOP_LEFT_CORNER;
 } else if(which == MOUSE_CURSOR_7) {
  glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
  current_cursor = GLUT_CURSOR_FULL_CROSSHAIR;
*/
 }

}
void setpickcursor_() {
 choose_cursor(PICK_CURSOR);
}

void getglwinpos() {
 static int x;
 static int y;
 static int first_time=1;

/* border causes offset troubles */
/* first time thru (from initial Reshape after CreateWindow) 
   gives an indication of how much trouble, can correct in future calls */
 if (first_time == 1) {
  first_time = 0;
  x = glutGet(GLUT_WINDOW_X);
  y = glutGet(GLUT_WINDOW_Y);
  x =  x - glwin_xorig;
  y =  y - glwin_yorig;
 } else {
  glwin_xorig = glutGet(GLUT_WINDOW_X) - x;
  glwin_yorig = glutGet(GLUT_WINDOW_Y) - y;
 }
 x = 0;
 y = 0;

 glwin_width =  glutGet(GLUT_WINDOW_WIDTH);
 glwin_height = glutGet(GLUT_WINDOW_HEIGHT);

/*
 fprintf (stderr, "%dx%d+%d+%d\n",
   glwin_width, glwin_height, glwin_xorig, glwin_yorig);
*/
}
void getwinpos_(int *x0, int *width, int *y0, int *height) {

 getglwinpos();
 *x0 =     glwin_xorig;
 *y0 =     glwin_yorig;
 *width =  glwin_width;
 *height = glwin_height;
 
}

void setwinpos_(int *x0, int *width, int *y0, int *height) {

 glutPositionWindow(*x0, *y0);
 glutReshapeWindow(*width, *height);
 getglwinpos();
}

void *getcmd_stdio(void *threadid) {

 gr_cmd = (char *)calloc(120, sizeof(char));
 grout_(">gramps>\n",9);
 while (1) {
      if (cmd_flag == 0) {
          fgets(gr_cmd, 120, stdin);
          if (feof(stdin)) {
              grout_("use EXIT command to quit", 24);
          } else if (*gr_cmd) {
              gr_cmd[strlen(gr_cmd)-1] = '\0';
              cmd_flag = 1;
          }
          grout_(">gramps>\n",9);
      }
 }
}

void *getcmd_readline(void *threadid)
{
   while (1) {
    if (cmd_flag == 0) {
        if (gr_cmd) free(gr_cmd);
            gr_cmd = readline(">gramps>");
            if (!gr_cmd) {
             //exit(1);
             grout_("use EXIT command to quit", 24);
            } else if (*gr_cmd) {
             add_history(gr_cmd);
             cmd_flag = 1;  // lets i/o thread know we got a command string
            }
        }
   }

}

void setup_getcmd(int itype) {
/* create thread to collect commands */

   int rc;
   void *t = NULL;
   pthread_t std_thread;
   if (itype == GETCMD_READLINE) {
    rc = pthread_create(&std_thread, NULL, getcmd_readline, (void *)t);
   } else if (itype == GETCMD_STDIO) {
    rc = pthread_create(&std_thread, NULL, getcmd_stdio, (void *)t);
   } else if (itype == GETCMD_STDIO) {
   // rc = pthread_create(&std_thread, NULL, getcmd, (void *)t);
   }
   if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
   }
}

void Redisplay()
{
static int Nredraw=0;
/* g0redraw_ will only redisplay if a display list change has occurred.  */

 //choose_cursor(WAIT_CURSOR);

 Nredraw++;
 //fprintf(stderr, "Redraw event %d %d\n", Nredraw, display_changed);
 g0redraw_(&display_changed);
 display_changed = 0;

}

int docmd(int output_to, char *gr_cmd)
{

/* STANDARD_ERR means output any output to STANDARD_OUT, but no prompt */
if(output_to == STANDARD_ERR)
 outflag = STANDARD_OUT;
else
 outflag = output_to;

display_changed += do_(gr_cmd, strlen(gr_cmd));
if (display_changed) glutPostRedisplay();
return display_changed;
}

void mbutton_feedback() {
// create text on screen to reflect which mouse button (device) is active
// if (mouse_state == 0) return;
 if (mouse_state == 0) {
  title[0] = ' ';
 } else {
  title[0] = '0' + mouse_state;
 }
 glutSetWindowTitle(title);
}


static void Idler(int timer_id) {

 int do_it = 0;
 int j = 0;
 int wflag = 0;
 int display_changed = 0;
 static int do_flag = 0;
 int cont_flag = 0;

 if (outflag == STANDARD_OUT) {
  if (waiting) {
   wflag = get_pick_wait_();
   if (wflag == 0) {
    waiting = 0;  // the waiting is over
    cmd_flag = 0; // notifies i/o thread to allow more command input
   }
  } else {
   if (cmd_flag) do_flag = docmd(outflag, gr_cmd);
     // cmd_flag alerts that i/o thread has a command

    if (do_flag > 0) {
        // do_flag returns # to advance, but advance command did updates itself
        //printf ("do_flag = %d\n", do_flag);
     } else if (do_flag < 0) {
         //printf ("do_flag = %d\n", do_flag);
         // do continuous updates
         do_it = 0; do_flag = contupd_(&do_it);
         do_it = 1; g0redraw_(&do_it);
         //glutPostRedisplay();
     } else {
         // pause command in effect or nothing to draw
         //printf ("do_flag = %d\n", do_flag);
     }
     cmd_flag = 0; // lets i/o thread ask for another command string

//     wflag = get_pick_wait_();
//     if (wflag == 0) {
//       cmd_flag = 0;
//       waiting = 0;
//     } else {
//       waiting = 1;
//     }

  }
#ifdef USE_SOCK
 } else if (outflag == SOCKET_OUT) {
   if(sockcheck(10) > 0) {
     j = sockget(gr_cmd, sizeof(gr_cmd));
     if (j > 0) {
      docmd(outflag, gr_cmd);
     } else {
      sockserverend(2);
      sockserverinit("gramps");
     }
   }
#endif
 }

// if (display_changed > 0) {
///* do continuous updates, if necessary even if paused */
//  do_it = 1; display_changed = contupd_(&do_it);
//  advsnap_(); //write snap file if necessary and advance frame count
////  nadvance--;
// }

 glutTimerFunc(33, Idler, 0);  // 16.666... is 60 frames per second

}

static void MouseEntry(int state) {
 int flag=1;
 if (state == GLUT_ENTERED) {
//   glutPopWindow();
 } else if (state == GLUT_LEFT) {
//   glutHideWindow();
 }
}

static void MouseWheel( int wheel, int direction, int x, int y ) {
/* fprintf(stderr, "wheel = %d; direction=%d; x=%d; y=%d", wheel,direction,x,y); */
 int type;
 int ndial;
 float xinc;
 float xval;
 int flag = 0;
/* this simulates D1 */
 type = DIALS;
 ndial = 1;
 xinc = direction; /* let user decide incremental value with e.g, D1/10 */
 xval = x;
 display_changed += update_(&type, &ndial, &xval, &xinc);

 if (display_changed) {
  flag = 1; // can't tell when Wheel is let up, so hires always
  g0hires_(&flag); /* draw in lo-res mode while moving */
  glutPostRedisplay();
 }
}

void setpickmatrix_() {
  GLint viewport[4];
// only if in pick mode (set by pick_ calling setpickcursor_)
  if (chosen_cursor == PICK_CURSOR) {
   glGetIntegerv(GL_VIEWPORT,viewport);
   //fprintf(stderr,"pick matrix %d %d\n", currx, curry);
   gluPickMatrix(currx,curry,PICKWIDTH,PICKWIDTH,viewport);
  }

}
static void MouseButton( int button, int state, int x, int y )
{
 int flag=0;
 float xyzw[4];
/* global lastx, lasty, glwin_width, glwin_height */

 if (state == GLUT_DOWN && chosen_cursor == PICK_CURSOR) {
  currx = x; curry = glwin_height-y;
  if (g0pickprocess(currx, curry, &xyzw)) choose_cursor(RESTORE_CURSOR);
 }

 if (keystate == 1) return; //don't let mouse buttons override keyboard

 if        (button == GLUT_LEFT_BUTTON) {
  if (state == GLUT_UP)   mouse_state -= 4;
  if (state == GLUT_DOWN) mouse_state += 4;
 } else if (button == GLUT_MIDDLE_BUTTON) {
  if (state == GLUT_UP)   mouse_state -= 2;
  if (state == GLUT_DOWN) mouse_state += 2;
 } else if (button == GLUT_RIGHT_BUTTON) {
  if (state == GLUT_UP)   mouse_state -= 1;
  if (state == GLUT_DOWN) mouse_state += 1;
#ifdef MINGW32
 } else if (button == GLUT_WHEEL_UP) {
  MouseWheel(1,1,x,y);
 } else if (button == GLUT_WHEEL_DOWN) {
  MouseWheel(1,-1,x,y);
#endif
 }

 lastx = x;
 lasty = y;
 if (mouse_state == 0) {
  flag = 1;
  g0hires_(&flag);
  glutPostRedisplay();
 }

  mbutton_feedback();
}

static void MouseMotion( int x, int y )
{
  float xinc=0;
  float yinc=0;
  float xval=0.0;
  float yval=0.0;
  int type=0;
  int flag = 0;
  int do_it = 0;

  /* global lastx, lasty, glwin_width, glwin_height */

  xinc = x - lastx;
  yinc = y - lasty;
  lastx = x;
  lasty = y;
  //int ms = mouse_state+1; // allow for x0 and y0
  //if (xinc < 2 && xinc > -2 && yinc < 2 && yinc > -2) return;
  xinc =  xinc / (float)glwin_width;
  yinc = -yinc / (float)glwin_height;
  if(xinc) {
      xval = (float)lastx/(float)glwin_width*2. - 1.;
      type = MOUSS;
      display_changed += update_(&type, &mouse_state, &xval, &xinc);
  }
  if(yinc) {
      yval = (float)(glwin_height-lasty)/(float)glwin_height*2. - 1.;
      type = MOUSS;
      int ms = mouse_state+4;
      display_changed += update_(&type, &ms, &yval, &yinc);
  }

  if (xinc == 0 && yinc == 0) return;
  /* do continuous updates regardless of xinc, yinc */
  /*
  i don't like this effect
 do_it = 1; display_changed += contupd_(&do_it);
 */

  if (display_changed) {
      flag = 0;
      g0hires_(&flag); /* draw in lo-res mode while moving */
      glutPostRedisplay();
  }

  //fprintf(stderr, "type = %d; state = %d; motion x = %i(%8.6f) y = %i(%8.6f)\n", type, mouse_state, x, xinc, y, yinc);
}

static void PassiveMouseMotion( int x, int y )
{
float xinc=0;
float yinc=0;
float xval=0.0;
float yval=0.0;
int type=0;
int flag = 0;
int do_it = 0;

/* global lastx, lasty, glwin_width, glwin_height */
//fprintf(stderr, "passive %d %d\n", x, y);
 return;  // for now

 xinc = x - lastx;
 yinc = y - lasty;
 lastx = x;
 lasty = y;
 int ms = mouse_state+1; // allow for x0 and y0
 //if (xinc < 2 && xinc > -2 && yinc < 2 && yinc > -2) return;
 xinc =  xinc / (float)glwin_width;
 yinc = -yinc / (float)glwin_height;
 if(xinc) {
  xval = (float)lastx/(float)glwin_width*2. - 1.;
  type = XMOUSS;
  display_changed += update_(&type, &ms, &xval, &xinc);
 }
 if(yinc) {
  yval = (float)(glwin_height-lasty)/(float)glwin_height*2. - 1.;
  type = YMOUSS;
  display_changed += update_(&type, &ms, &yval, &yinc);
 }

 if (xinc == 0 && yinc == 0) return;
 /* do continuous updates regardless of xinc, yinc */
 /*
  i don't like this effect
 do_it = 1; display_changed += contupd_(&do_it);
 */

 if (display_changed) {
  flag = 0;
  g0hires_(&flag); /* draw in lo-res mode while moving */
  glutPostRedisplay();
 }

 fprintf(stderr, "type = %d; state = %d; motion x = %i(%8.6f) y = %i(%8.6f)\n", type, ms, x, xinc, y, yinc);
}

void Reshape(int x, int y) {
 int x0=0;
 int y0=0;
 //fprintf(stderr, "Reshape\n");
 int flag=1;
 g0wset_(&x0, &y0, &x, &y, &flag);
 glutPostRedisplay();
}

void setkeys_(int *k) {
 keytype = *k;
 if (keytype == KEY_STICKY) {
  choose_cursor(STICKY_CURSOR);
 } else {
  choose_cursor(NORMAL_CURSOR);
 }
 keystate = 0;
}

static void SpecialKey( int key, int x, int y )
{
   static int f1=-1;
   static int x0, y0, width, height;
 
   switch (key) {
	case GLUT_KEY_F1:
#ifdef linux
	break;
	/* doesn't play well with gnome */
#endif
	/* start/preferences/keyboard/toggle full screen (i use alt-f3) */
 	f1 = - f1;
	//fprintf(stderr,"%d: %dx%d+%d+%d ", f1, width, height, x0, y0);
	if (f1 > 0) {
		//fprintf(stderr,"go to fullscreen\n");
		x0 = glwin_xorig; y0 = glwin_yorig;
		width = glwin_width; height = glwin_height;
		glutFullScreen();
	} else {
		//fprintf(stderr,"restore screen\n");
		glutReshapeWindow(width, height);
		glutPositionWindow(x0, y0);
	}
       	break;

	case GLUT_KEY_UP:
       	break;
   }
}
static void UpKey( unsigned char key, int x, int y ) {
 if (keytype == KEY_ON) mouse_state = 0;
 if (keytype != KEY_STICKY) keystate = 0;
 mbutton_feedback();
}
static void Key( unsigned char key, int x, int y ) {

 if (keytype == KEY_OFF) return;
 keystate = 1;
 switch (key) {
      case '1':
         mouse_state = 1;
	 break;
      case '2':
         mouse_state = 2;
	 break;
      case '3':
         mouse_state = 3;
	 break;
      case '4':
         mouse_state = 4;
	 break;
      case '5':
         mouse_state = 5;
	 break;
      case '6':
         mouse_state = 6;
	 break;
      case '7':
         mouse_state = 7;
	 break;
      case '8':
         mouse_state = 8;
	 break;
      case '9':
         mouse_state = 9;
	 break;
      case 27: //esc
         mouse_state = 0; /* MouseButton is NOT always called */
         keystate = 0;
         break;
   }
 //fprintf(stderr, "key = %d; mouse_state = %d\n", key, mouse_state);
 mbutton_feedback();

}

int main (int argc, char **argv) {

   int glwinID=-1;
   int setup_flag = 0;

   int i;
   /* unless hardware supports alpha, don't use it */
   int glut_mode = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA;

   setup_flag=GETCMD_READLINE;
   outflag = STANDARD_OUT;
   for (i=0; i<argc; i++) {
    if ( !strcmp(argv[i],"-z") ) {
    } else if ( !strcmp(argv[i],"-c") ) {
     outflag = STANDARD_OUT;
     setup_flag=GETCMD_STDIO;
    } else if ( !strcmp(argv[i],"-a") ) {
     glut_mode -= GLUT_ALPHA;
    } else if ( !strcmp(argv[i],"-r") ) {
     outflag = STANDARD_OUT;
     setup_flag=GETCMD_READLINE;
#ifdef USE_SOCK
    } else if ( !strcmp(argv[i],"-s") ) {
     outflag = SOCKET_OUT;
     sockserverinit("gramps");
#endif
    }
   }
   setfrom_(&outflag); /* where grout outputs to */

   glutInit( &argc, argv );
   glutInitWindowPosition( glwin_xorig, glwin_yorig );
   glutInitWindowSize( glwin_width, glwin_height );
   glutInitDisplayMode( glut_mode );
   glwinID = glutCreateWindow(argv[0]);
   glutSetWindowTitle(title);

   glutIgnoreKeyRepeat( 1 );
   glutKeyboardFunc( Key );
   glutKeyboardUpFunc( UpKey );
   glutSpecialFunc( SpecialKey );

   glutMouseFunc( MouseButton );
   glutMotionFunc( MouseMotion );
   glutPassiveMotionFunc( PassiveMouseMotion );
#ifdef linux
   glutMouseWheelFunc( MouseWheel );
#endif
   glutEntryFunc( MouseEntry );

   glutReshapeFunc( Reshape );
   glutDisplayFunc( Redisplay );
   glPointSize(4.0);
//   glutIdleFunc( Idler );
   glutTimerFunc(16, Idler, 0);
   udinit_();
   g0pinit_();
   startup_();
   choose_cursor(NORMAL_CURSOR);
   setup_getcmd(setup_flag);

   glutMainLoop();

}
