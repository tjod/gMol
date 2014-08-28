/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
/*% cc -g sphere.c -o sphere -lm
 *
 * sphere - generate a triangle mesh approximating a sphere by
 *  recursive subdivision. First approximation is an platonic
 *  solid; each level of refinement increases the number of
 *  triangles by a factor of 4.
 *
 * Level 3 (128 triangles for an octahedron) is a good tradeoff if
 *  gouraud shading is used to render the database.
 *
 * Usage: sphere [level] [-p] [-c] [-f] [-t] [-i]
 *	level is an integer >= 1 setting the recursion level (default 1).
 *	-c causes triangles to be generated with vertices in counterclockwise
 *	    order as viewed from the outside in a RHS coordinate system.
 *	    The default is clockwise order.
 *	-t starts with a tetrahedron instead of an octahedron
 *	-i starts with a icosahedron instead of an octahedron
 *
 *  The subroutines print_object() and print_triangle() should
 *  be changed to generate whatever the desired database format is.
 *
 * Jon Leech (leech @ cs.unc.edu) 3/24/89
 * icosahedral code added by Jim Buddenhagen (jb1556@daditz.sbc.com) 5/93

 * Modified slightly to interface with GRAMPS tjo 10/2002
 */
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#include <stdlib.h>

typedef struct {
    double  x, y, z;
} point;

typedef struct {
    point     pt[3];	/* Vertices of triangle */
    double    area;	/* Unused; might be used for adaptive subdivision */
} triangle;

typedef struct {
    int       npoly;	/* # of triangles in object */
    triangle *poly;	/* Triangles */
} object;

/* Six equidistant points lying on the unit sphere */
#define XPLUS {  1,  0,  0 }	/*  X */
#define XMIN  { -1,  0,  0 }	/* -X */
#define YPLUS {  0,  1,  0 }	/*  Y */
#define YMIN  {  0, -1,  0 }	/* -Y */
#define ZPLUS {  0,  0,  1 }	/*  Z */
#define ZMIN  {  0,  0, -1 }	/* -Z */

/* Vertices of a unit octahedron */
triangle octahedron[] = {
    { { XPLUS, ZPLUS, YPLUS }, 0.0 },
    { { YPLUS, ZPLUS, XMIN  }, 0.0 },
    { { XMIN , ZPLUS, YMIN  }, 0.0 },
    { { YMIN , ZPLUS, XPLUS }, 0.0 },
    { { XPLUS, YPLUS, ZMIN  }, 0.0 },
    { { YPLUS, XMIN , ZMIN  }, 0.0 },
    { { XMIN , YMIN , ZMIN  }, 0.0 },
    { { YMIN , XPLUS, ZMIN  }, 0.0 }
};

/* A unit octahedron */
object oct = {
    sizeof(octahedron) / sizeof(octahedron[0]),
    &octahedron[0]
};

/* Vertices of a tetrahedron */
#define sqrt_3 0.5773502692
#define PPP {  sqrt_3,	sqrt_3,  sqrt_3 }   /* +X, +Y, +Z */
#define MMP { -sqrt_3, -sqrt_3,  sqrt_3 }   /* -X, -Y, +Z */
#define MPM { -sqrt_3,	sqrt_3, -sqrt_3 }   /* -X, +Y, -Z */
#define PMM {  sqrt_3, -sqrt_3, -sqrt_3 }   /* +X, -Y, -Z */

/* Structure describing a tetrahedron */
triangle tetrahedron[] = {
   { { PPP, MMP, MPM } , 0.0 },
   { { PPP, PMM, MMP } , 0.0 },
   { { MPM, MMP, PMM } , 0.0 },
   { { PMM, PPP, MPM } , 0.0 }
};

object tet = {
    sizeof(tetrahedron) / sizeof(tetrahedron[0]),
    &tetrahedron[0]
};

/* Twelve vertices of icosahedron on unit sphere */
#define tau 0.8506508084      /* t=(1+sqrt(5))/2, tau=t/sqrt(1+t^2)  */
#define one 0.5257311121      /* one=1/sqrt(1+t^2) , unit sphere     */
#define ZA {  tau,  one,    0 }
#define ZB { -tau,  one,    0 }
#define ZC { -tau, -one,    0 }
#define ZD {  tau, -one,    0 }
#define YA {  one,   0 ,  tau }
#define YB {  one,   0 , -tau }
#define YC { -one,   0 , -tau }
#define YD { -one,   0 ,  tau }
#define XA {   0 ,  tau,  one }
#define XB {   0 , -tau,  one }
#define XC {   0 , -tau, -one }
#define XD {   0 ,  tau, -one }

/* Structure for unit icosahedron */
triangle icosahedron[] = {
    { { YA, XA, YD }, 0.0 },
    { { YA, YD, XB }, 0.0 },
    { { YB, YC, XD }, 0.0 },
    { { YB, XC, YC }, 0.0 },
    { { ZA, YA, ZD }, 0.0 },
    { { ZA, ZD, YB }, 0.0 },
    { { ZC, YD, ZB }, 0.0 },
    { { ZC, ZB, YC }, 0.0 },
    { { XA, ZA, XD }, 0.0 },
    { { XA, XD, ZB }, 0.0 },
    { { XB, XC, ZD }, 0.0 },
    { { XB, ZC, XC }, 0.0 },
    { { XA, YA, ZA }, 0.0 },
    { { XD, ZA, YB }, 0.0 },
    { { YA, XB, ZD }, 0.0 },
    { { YB, ZD, XC }, 0.0 },
    { { YD, XA, ZB }, 0.0 },
    { { YC, ZB, XD }, 0.0 },
    { { YD, ZC, XB }, 0.0 },
    { { YC, XC, ZC }, 0.0 }
};

/* A unit icosahedron */
object ico = {
    sizeof(icosahedron) / sizeof(icosahedron[0]),
    &icosahedron[0]
};

int Flatflag = 0;   /* Don't generate per-vertex normals */

/* Forward declarations */
point *normalize(/* point *p */);
point *midpoint(/* point *a, point *b */);
void flip_object(/* object *obj */);
void print_object(/* object *obj, int level, float *buf */);
void print_triangle(/* triangle *t, float *buf */);

//extern char *malloc(/* unsigned */);

int g0sphmak_(int *ntv, float *buf, int *bufsiz) {

    object *old = &oct,		/* Default is octahedron */
	   *new;
    int     ccwflag = 0,	/* Reverse vertex order if true */
	    i,
	    level,		/* Current subdivision level */
	    maxlevel = 1;	/* Maximum subdivision level */

    int     ntri = 0;

    /* Parse arguments */
/*
    for (i = 1; i < ac; i++) {
	if (!strcmp(av[i], "-c"))
	    ccwflag = 1;
	else if (!strcmp(av[i], "-f"))
	    Flatflag = 1;
	else if (!strcmp(av[i], "-t"))
	    old = &tet;
	else if (!strcmp(av[i], "-i"))
	    old = &ico;
	else if (isdigit(av[i][0])) {
	    if ((maxlevel = atoi(av[i])) < 1) {
		fprintf(stderr, "%s: # of levels must be >= 1\n", av[0]);
		exit(1);
	    }
	} else {
	    fprintf(stderr, "Usage: %s [-p] [-c] [-f] [-t] [-i] [n]\n", av[0]);
	    exit(1);
	}
    }
*/

    maxlevel = *ntv;

    old = &ico;

    if (ccwflag)
	flip_object(old);

    /* Subdivide each starting triangle (maxlevel - 1) times */
    for (level = 1; level < maxlevel; level++) {
	/* Allocate a new object */
	new = (object *)malloc(sizeof(object));
	if (new == NULL) {
	    fprintf(stderr, "makesph: Out of memory on subdivision level %d\n",
		level);
	    exit(1);
	}
	new->npoly = old->npoly * 4;

/* GRAMPS buf has limits.  Each tri takes 9 float coords. */
	if (new->npoly * 9 > *bufsiz) {
		ntri = -(new->npoly);
	    	fprintf(stderr, "makesph: too many triangles %d\n", ntri);
		return ntri;
	}

	/* Allocate 4* the number of points in the current approximation */
	new->poly  = (triangle *)malloc(new->npoly * sizeof(triangle));
	if (new->poly == NULL) {
	    fprintf(stderr, "makesph: Out of memory on subdivision level %d\n",
		level);
	    exit(1);
	}

	/* Subdivide each triangle in the old approximation and normalize
	 *  the new points thus generated to lie on the surface of the unit
	 *  sphere.
	 * Each input triangle with vertices labelled [0,1,2] as shown
	 *  below will be turned into four new triangles:
	 *
	 *			Make new points
	 *			    a = (0+2)/2
	 *			    b = (0+1)/2
	 *			    c = (1+2)/2
	 *	  1
	 *	 /\		Normalize a, b, c
	 *	/  \
	 *    b/____\ c		Construct new triangles
	 *    /\    /\		    [0,b,a]
	 *   /	\  /  \		    [b,1,c]
	 *  /____\/____\	    [a,b,c]
	 * 0	  a	2	    [a,c,2]
	 */
	for (i = 0; i < old->npoly; i++) {
	    triangle
		 *oldt = &old->poly[i],
		 *newt = &new->poly[i*4];
	    point a, b, c;

	    a = *normalize(midpoint(&oldt->pt[0], &oldt->pt[2]));
	    b = *normalize(midpoint(&oldt->pt[0], &oldt->pt[1]));
	    c = *normalize(midpoint(&oldt->pt[1], &oldt->pt[2]));

	    newt->pt[0] = oldt->pt[0];
	    newt->pt[1] = b;
	    newt->pt[2] = a;
	    newt++;

	    newt->pt[0] = b;
	    newt->pt[1] = oldt->pt[1];
	    newt->pt[2] = c;
	    newt++;

	    newt->pt[0] = a;
	    newt->pt[1] = b;
	    newt->pt[2] = c;
	    newt++;

	    newt->pt[0] = a;
	    newt->pt[1] = c;
	    newt->pt[2] = oldt->pt[2];
	}

	if (level > 1) {
	    free(old->poly);
	    free(old);
	}

	/* Continue subdividing new triangles */
	old = new;
    }

    /* Print out resulting approximation */
    print_object(old, maxlevel, buf);

    /* return number of triangle, free memory */
    ntri = old->npoly;
    if (maxlevel > 1) {
      free(old->poly);
      free(old);
    }
    return ntri;
}

/* Normalize a point p */
point *normalize(p)
point *p;
{
    static point r;
    double mag;

    r = *p;
    mag = r.x * r.x + r.y * r.y + r.z * r.z;
    if (mag != 0.0) {
	mag = 1.0 / sqrt(mag);
	r.x *= mag;
	r.y *= mag;
	r.z *= mag;
    }

    return &r;
}

/* Return the midpoint on the line between two points */
point *midpoint(a, b)
point *a, *b;
{
    static point r;

    r.x = (a->x + b->x) * 0.5;
    r.y = (a->y + b->y) * 0.5;
    r.z = (a->z + b->z) * 0.5;

    return &r;
}

/* Reverse order of points in each triangle */
void flip_object(obj)
object *obj;
{
    int i;
    for (i = 0; i < obj->npoly; i++) {
	point tmp;
		       tmp = obj->poly[i].pt[0];
	obj->poly[i].pt[0] = obj->poly[i].pt[2];
	obj->poly[i].pt[2] = tmp;
    }
}

/* Write out all triangles in an object */
void print_object(obj, level, buf)
object *obj;
int level;
float *buf;
{
    int i;
    int j=0;

    /* Spit out coordinates for each triangle */
    for (i = 0; i < obj->npoly; i++) {
	print_triangle(&obj->poly[i], &buf[j]);
        j += 9;
    }

}

/* Output a triangle */
void print_triangle(t, buf)
triangle *t;
float *buf;
{
    int i;
    int j;

/* Modify this to generate your favorite output format
 * Triangle vertices are in t->pt[0..2].{x,y,z}
 * A generic format is provided.
 */
// printf("triangle\n");
 j = 0;
 for (i = 0; i < 3; i++) {
//  printf("\t%g %g %g\n", t->pt[i].x, t->pt[i].y, t->pt[i].z);
   buf[j++] = t->pt[i].x;
   buf[j++] = t->pt[i].y;
   buf[j++] = t->pt[i].z;
 }
}
