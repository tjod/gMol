// based heavily on
// Marching Cubes Example Program 
// by Cory Bloyd (corysama@yahoo.com)
//
// A simple, portable and complete implementation of the Marching Cubes
// and Marching Tetrahedrons algorithms in a single source file.
//
// For a description of the algorithm go to
// http://astronomy.swin.edu.au/pbourke/modelling/polygonise/
//
#include "atom_properties.h"
#include "atom_values.h"
#include "db.h"
#include "dbchem.h"
#include <QApplication>
QString gmolLib;

#include "stdio.h"
#include <iostream>
#include "math.h"
#include "stdlib.h"
#include "types.h"

//These tables are used so that everything can be done in little loops that you can look at all at once
// rather than in pages and pages of unrolled code.

//a2fVertexOffset lists the positions, relative to vertex0, of each of the 8 vertices of a cube
static const float a2fVertexOffset[8][3] =
{
    {0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, 1.0, 0.0},{0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},{1.0, 0.0, 1.0},{1.0, 1.0, 1.0},{0.0, 1.0, 1.0}
};

//a2iVertexOffset lists the positions, relative to vertex0, of each of the 8 vertices of a cube
static const int a2iVertexOffset[8][3] =
{
    {0, 0, 0},{1, 0, 0},{1, 1, 0},{0, 1, 0},
    {0, 0, 1},{1, 0, 1},{1, 1, 1},{0, 1, 1}
};
//a2iEdgeConnection lists the index of the endpoint vertices for each of the 12 edges of the cube
static const int a2iEdgeConnection[12][2] = 
{
    {0,1}, {1,2}, {2,3}, {3,0},
    {4,5}, {5,6}, {6,7}, {7,4},
    {0,4}, {1,5}, {2,6}, {3,7}
};

//a2fEdgeDirection lists the direction vector (vertex1-vertex0) for each edge in the cube
static const float a2fEdgeDirection[12][3] =
{
    {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{-1.0, 0.0, 0.0},{0.0, -1.0, 0.0},
    {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{-1.0, 0.0, 0.0},{0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},{0.0, 0.0, 1.0},{ 0.0, 0.0, 1.0},{0.0,  0.0, 1.0}
};

//a2iTetrahedronEdgeConnection lists the index of the endpoint vertices for each of the 6 edges of the tetrahedron
static const int a2iTetrahedronEdgeConnection[6][2] =
{
    {0,1},  {1,2},  {2,0},  {0,3},  {1,3},  {2,3}
};

//a2iTetrahedronEdgeConnection lists the index of verticies from a cube 
// that made up each of the six tetrahedrons within the cube
static const int a2iTetrahedronsInACube[6][4] =
{
    {0,5,1,6},
    {0,1,2,6},
    {0,2,3,6},
    {0,3,7,6},
    {0,7,4,6},
    {0,4,5,6},
};

gridValue ***fSample; // the data grid
Point corner; // the start point of the grid
Point incs; // the increments along the grid
int Nx,Ny,Nz; // number of cubes
float rProbe;

// which algorithm to use: marching cubes or tetrahedra
void vMarchCube1(int ix, int iy, int iz, float fTargetValue, bool gradients); // marching cubes
void vMarchCube2(int ix, int iy, int iz, float fTargetValue, bool gradients); // marching tetrahedra
void (*vMarchCube)(int ix, int iy, int iz, float fTargetValue, bool gradients) = vMarchCube1;
void vMarch(int nx, int ny, int nz, float fTargetValue, bool gradients);

// which potential energy function to use
float fvdW(float radius, float fX, float fY, float fZ);
float fLJ(float radius, float fX, float fY, float fZ);
float fGauss(float radius, float fX, float fY, float fZ);
float (*fSampleValue)(float radius, float fX, float fY, float fZ) = fvdW;
void makeSample(atomQuery atom, int nx, int ny, int nz);

void makeGradients(int itemid, int molid);
void vGetNormal(Point &rfNormal, Point &rfEdge);
void vNormalizeVector(Point &rfVectorResult, Point &rfVectorSource);

QSqlQuery *addTriangle; // prepared in main, used in triangleOutput
atomQuery *atom; // prepared in main, used in triangleOutput

#define PIX 1
#define DB  3
#define VDW 1
#define GAUSS 2
#define LJ 3
#define MINDIST 4.0 // greater than this, assume no energy contribution from atom

void usage() {
    std::cerr << "Usage: dbsurf <dbname> [options]" << std::endl;
    std::cerr << "Options:          Description:" << std::endl;
    std::cerr << "  -i              itemid in tree table" << std::endl;
    std::cerr << "  -v              contour value" << std::endl;
    std::cerr << "  -n              assign nearest atom (-db only)" << std::endl;
    std::cerr << "  -db             output triangles to database" << std::endl;
    std::cerr << "  -pix            output triangles to stdout" << std::endl;
    std::cerr << "  -m              use marching cubes" << std::endl;
    std::cerr << "  -t              use marching tetrahedra" << std::endl;
    std::cerr << "  -g              use gradient normals (slower); no effect with -t" << std::endl;
    std::cerr << "  -f <type>       potential energy function: vdW, LJ or Gauss" << std::endl;    
    std::cerr << "  -s <stepsize>   step size" << std::endl;
    std::cerr << "  -p <padding>    padding" << std::endl;
    std::cerr << "  -r <radius>     probe radius" << std::endl;
}

bool getArgs(int argc, char **argv, QString *dbname, int *itemid, float *fTargetValue, int *outtype,
             float *step, float *padding, float *r, bool *gradients) {
    
    if (argc < 2) return false;
    
    std::string option;
    *dbname = argv[1];  
    for (int i = 2; i < argc; i++) {
        option = argv[i];    
        if ((option == "-i") && (argc > (i+1))) {
            *itemid = atoi(argv[++i]);

        } else if ((option == "-v") && (argc > (i+1))) {
            *fTargetValue = atof(argv[++i]);

        } else if ((option == "-pix")) {
            *outtype = PIX;

        } else if ((option == "-db")) {
            *outtype = DB;
            qDebug() << "output to" << *dbname;

        } else if ((option == "-s") && (argc > (i+1))) {
            *step = atof(argv[++i]);
            
        } else if ((option == "-f") && (argc > (i+1))) {
            QString pname = QString::fromLocal8Bit(argv[++i]);
            if      (pname.compare("vdW",   Qt::CaseInsensitive) == 0) fSampleValue = fvdW;
            else if (pname.compare("LJ",    Qt::CaseInsensitive) == 0) fSampleValue = fLJ;
            else if (pname.compare("Gauss", Qt::CaseInsensitive) == 0) fSampleValue = fGauss;
            //qDebug() << pname << *ptype;
            
        } else if (option == "-m") {
            vMarchCube = vMarchCube1;
            
        } else if (option == "-t") {
            vMarchCube = vMarchCube2;
            
        } else if (option == "-g") {
            *gradients = true;
            
        } else if ((option == "-p") && (argc > (i+1))) {
            *padding = atof(argv[++i]);
            
        } else if ((option == "-r") && (argc > (i+1))) {
            *r = atof(argv[++i]);
        }
    }
    return true;
}

// prepare tables and statements needed for output to db
void dbstart() {
    QSqlQuery q;
    q.exec("Create Table If Not Exists vertex (itemid Integer, vid Integer, atid Integer, x Real, y Real, z Real, nx Real, ny Real, nz Real, Unique (itemid,vid) Foreign Key (itemid) References tree (itemid) On Delete Cascade)");
    q.exec("Create Table If Not Exists triangle (itemid Integer, atid Integer, tid Integer, tvid Integer, vid Integer, Foreign Key (itemid) References tree (itemid) On Delete Cascade)");
    q.exec("Create Temporary Table If Not Exists tri_vertex (vid Integer Primary Key, tid Integer, atid Integer, x Real, y Real, z Real, nx Real, ny Real, nz Real)");
    //q.exec("Delete From tmp_vertex");

    /* addTriangle will be used by addtri */
    addTriangle = new QSqlQuery();
    addTriangle->prepare("Insert Into tri_vertex (tid,atid,x,y,z,nx,ny,nz) Values (?,?,?,?,?,?,?,?)");
}
// process output triangles into unique set of vertices
void dbfinish(int itemid, int molid, bool gradients) {
    QSqlQuery q;
    q.prepare("Delete From vertex Where itemid=?");
    q.bindValue(0,itemid);
    q.exec();
    q.finish();
    q.prepare("Delete From triangle Where itemid=?");
    q.bindValue(0,itemid);
    q.exec();
    q.finish();
    
    //q.exec("Create Index tri_xyz  On tri_vertex(x,y,z)");
    QString sql = "Create Temporary Table uniq_vertex As Select ? itemid,vid,atid,x,y,z,Avg(nx)nx,Avg(ny)ny,Avg(nz)nz From tri_vertex Group By x,y,z";
    //QString sql = "Create Temporary Table uniq_vertex As Select ? itemid,vid,atid,x,y,z,nx,ny,nz From tri_vertex Group By x,y,z";
    if (!q.prepare(sql)) Db::tellError(q, "prepare");
    q.bindValue(0,itemid);
    if (!q.exec()) Db::tellError(q, "exec");
    q.finish();
    
    //q.exec("Create Index uniq_xyz  On uniq_vertex(x,y,z)");        
    sql = "Insert Into triangle (itemid,atid,tid,tvid,vid) Select ?,v.atid,tid,v.vid,u.oid From tri_vertex v join uniq_vertex u Using (x,y,z)";
    if (!q.prepare(sql)) Db::tellError(q, "prepare");
    q.bindValue(0,itemid);
    if (!q.exec()) Db::tellError(q, "exec");
    q.finish();

    // compute gradients for use as normals, or just copy averaged triangle normals
    if (gradients) {
            makeGradients(itemid, molid);
    } else {
        sql = "Insert into vertex (itemid,vid,atid,x,y,z,nx,ny,nz) Select itemid,u.oid,atid,x,y,z,nx,ny,nz From uniq_vertex u";
        if (!q.exec(sql)) Db::tellError(q, "exec");
        q.finish();
    }
}

// compute gradients at each vertex point
void makeGradients(int itemid, int molid) {
    QString sql = "Select vid,a.atid,atnum,a.x,a.y,a.z,u.x,u.y,u.z From uniq_vertex u Join atom a Where molid=? \
            And a.x Between u.x-? And u.x+? And a.y Between u.y-? And u.y+? And a.z Between u.z-? and u.z+? Order By u.oid";
    QSqlQuery q;
    if (!q.prepare(sql)) {
        Db::tellError(q, "prepare");
        return;
    }
    q.addBindValue(molid);
    q.addBindValue(MINDIST);q.addBindValue(MINDIST);q.addBindValue(MINDIST);
    q.addBindValue(MINDIST);q.addBindValue(MINDIST);q.addBindValue(MINDIST);
    QSqlQuery insert;
    if (!insert.prepare("Insert Into vertex (itemid,vid,atid,x,y,z,nx,ny,nz) Values (?,?,?,?,?,?,?,?,?)")) {
        Db::tellError(insert, "prepare");
        return;
    }
    insert.bindValue(0,itemid);
    Point normal;
    Point vtx;
    double ax,ay,az,dx,dy,dz;
    double dxyz = 0.05;
    double fposx = 0.0, fposy = 0.0, fposz = 0.0, fminx = 0.0, fminy = 0.0, fminz = 0.0;    
    int vid=0, pvid=0;
    int atid;
    int atnum;
    q.exec();
    //q.next();
    int nvtx = 0;
    while (true) {
        bool atend = !(q.next());        
        if (!atend) vid = q.value(0).toInt();
        if (atend || (pvid > 0 && vid != pvid)) {
            // new vertex, output gradient sum for previous vertex
            normal.x = (fminx - fposx) / dxyz;
            normal.y = (fminy - fposy) / dxyz;
            normal.z = (fminz - fposz) / dxyz;
            vNormalizeVector(normal, normal);
            ++nvtx;            
            insert.bindValue(1, nvtx);
            insert.bindValue(2, atid);
            insert.bindValue(3,vtx.x);
            insert.bindValue(4,vtx.y);
            insert.bindValue(5,vtx.z);
            insert.bindValue(6,normal.x);
            insert.bindValue(7,normal.y);
            insert.bindValue(8,normal.z);
            if (!insert.exec()) {
                Db::tellError(insert, "insert");
                return;
            }
            if (atend) {
                return;
            }
            fposx=fposy=fposz=fminx=fminy=fminz=0.0;
        }
        atid = q.value(1).toInt();
        atnum = q.value(2).toInt();
        ax = q.value(3).toDouble();
        ay = q.value(4).toDouble();
        az = q.value(5).toDouble();
        vtx.x = q.value(6).toFloat();
        vtx.y = q.value(7).toFloat();
        vtx.z = q.value(8).toFloat();
        dx = vtx.x - ax;
        dy = vtx.y - ay;
        dz = vtx.z - az;
        fposx += fSampleValue(Atom::radius[atnum], dx+dxyz, dy,      dz);
        fposy += fSampleValue(Atom::radius[atnum], dx,      dy+dxyz, dz);
        fposz += fSampleValue(Atom::radius[atnum], dx,      dy,      dz+dxyz);
        fminx += fSampleValue(Atom::radius[atnum], dx-dxyz, dy,      dz);
        fminy += fSampleValue(Atom::radius[atnum], dx,      dy-dxyz, dz);
        fminz += fSampleValue(Atom::radius[atnum], dx,      dy,      dz-dxyz);
        pvid = vid;
    }
}
/*
void makeGradients(int itemid, int molid) {
    QString sql = "Select vid,atid,x,y,z From uniq_vertex";
    QSqlQuery q;
    if (!q.prepare(sql)) {
        Db::tellError(q, "prepare");
        return;
    }
    QSqlQuery insert;
    if (!insert.prepare("Insert Into vertex (itemid,vid,atid,x,y,z,nx,ny,nz) Values (?,?,?,?,?,?,?,?,?)")) {
        Db::tellError(insert, "prepare");
        return;
    }
    insert.bindValue(0,itemid);
    Point normal;
    Point vtx;
    q.exec();
    while (q.next()) {
        int vid = q.value(0).toInt();
        int atid = q.value(1).toInt();
        vtx.x = q.value(2).toFloat();
        vtx.y = q.value(3).toFloat();
        vtx.z = q.value(4).toFloat();
        vGetNormal(normal, vtx);
        vNormalizeVector(normal, normal);
        insert.bindValue(1, vid);
        insert.bindValue(2, atid);
        insert.bindValue(3,vtx.x);
        insert.bindValue(4,vtx.y);
        insert.bindValue(5,vtx.z);
        insert.bindValue(6,normal.x);
        insert.bindValue(7,normal.y);
        insert.bindValue(8,normal.z);
        if (!insert.exec()) {
            Db::tellError(insert, "insert");
            return;
        }
    }
}
*/
int main(int argc, char **argv) 
{
    QApplication app(argc, argv); // needed to get SQLITE plugin to work
    
    // some default params, settable on command line
    float fTargetValue = 1.0; // countour value
    rProbe = 0.0; // probe radius
    int itemid = 0; // from the gMol tree table
    float padding = 4.0; // padding around atom extremes
    float step = 0.75; // size of cubes
    int outtype = 0;  // output to db, or stdout in pix file format
    bool gradients = false;
    
    // info comes from sqlite db
    QSqlDatabase db;
    QString dbname = "";
    if (!getArgs(argc, argv, &dbname, &itemid, &fTargetValue, &outtype, &step, &padding, &rProbe, &gradients)) {
        usage();
        exit(-1);
    }
    if (!dbname.isEmpty()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbname);
        if (!db.open()) {
            qDebug() << "can't open database" << dbname;
            usage();
        }
        QSqlQuery q;
        q.exec("Pragma foreign_keys = ON");
    }
 
    // get essential info from db
    int imol = 0;
    char chain = NOCHAIN;
    int resnum = NORESNUM;
    int filter = 0;
    int hydrogen = 0;
    if (itemid) {
      treeQuery item;
      item.getRow(itemid);
      chain = item.chain;
      imol = item.imol;
      filter = item.filter;
      resnum = item.resnum;
      hydrogen = item.hydrogens;
    }
    // when output goes back to db
    if (outtype == DB) {
        dbstart();
        db.transaction();
    }        
    
    // size of each grid cube
    incs.x = step;
    incs.y = step;
    incs.z = step;
    
    // set lower corner based on mol bounds; finally number of cubes in each dimension
    float min[3], max[3], avg[3];  
    Db::molBounds(imol, resnum, chain, filter, min, max, avg);
    corner.x = floor(min[0] - padding);
    corner.y = floor(min[1] - padding);
    corner.z = floor(min[2] - padding);
    Nx = ( ceil(max[0] + padding) - floor(min[0] - padding) ) / incs.x;
    Ny = ( ceil(max[1] + padding) - floor(min[1] - padding) ) / incs.y;
    Nz = ( ceil(max[2] + padding) - floor(min[2] - padding) ) / incs.z;
    
    // fill the grid with data
    fSample = Create3D<gridValue>(Nx, Ny, Nz);
    int natom = 0;
    atom = new atomQuery();
    for (atom->iter(imol, resnum, chain, filter, hydrogen); atom->next() ; ) {  
       makeSample(*atom, Nx, Ny, Nz);       
       ++natom;
    }
    //qDebug() << natom << "atoms.";
    
    // make the isocontour
    vMarch(Nx-1, Ny-1, Nz-1, fTargetValue, gradients);
    
    // finish up, possibly computing gradient normals
    if (outtype == DB) {
        dbfinish(itemid, imol, gradients);
        db.commit();
        db.close();
    }
  
    Delete3D(fSample);
    fprintf (stderr, "itemid=%d\n", itemid); // communicate id back to calling proc
    return 1;
}

//fGetOffset finds the approximate point of intersection of the surface
// between two points with the values fValue1 and fValue2
float fGetOffset(float fValue1, float fValue2, float fValueDesired)
{
    double fDelta = fValue2 - fValue1;
    
    if(fDelta == 0.0)
    {
        return 0.5;
    }
    return (fValueDesired - fValue1)/fDelta;
}


void vNormalizeVector(Point &rfVectorResult, Point &rfVectorSource)
{
    float fOldLength;
    float fScale;
    
    fOldLength = sqrtf( (rfVectorSource.x * rfVectorSource.x) +
                        (rfVectorSource.y * rfVectorSource.y) +
                        (rfVectorSource.z * rfVectorSource.z) );
    
    if(fOldLength == 0.0)
    {
        rfVectorResult.x = rfVectorSource.x;
        rfVectorResult.y = rfVectorSource.y;
        rfVectorResult.z = rfVectorSource.z;
    }
    else
    {
        fScale = 1.0/fOldLength;
        rfVectorResult.x = rfVectorSource.x*fScale;
        rfVectorResult.y = rfVectorSource.y*fScale;
        rfVectorResult.z = rfVectorSource.z*fScale;
    }
}
// Lennard-Jones potential
float fLJ(float radius, float fDx, float fDy, float fDz)
{
    // suggested contour 0.0
    double fResult = 0.0;
    float r = radius + rProbe;
    //double fDx, fDy, fDz;
    //fDx = fX - atom.x;
    //fDy = fY - atom.y;
    //fDz = fZ - atom.z;
    double sigmaOverRsquared = (r*r) / (fDx*fDx + fDy*fDy + fDz*fDz);
    fResult = pow(sigmaOverRsquared,6) - pow(sigmaOverRsquared,3);
    return fResult;
}
// Gaussian distribution
float fGauss(float radius, float fDx, float fDy, float fDz)
{
    // https://bionano.cent.uw.edu.pl/Software/SurfaceDiver/UsersManual/Surface
    // suggested contour 1.0
    double fResult = 0.0;
    float r = radius + rProbe;    
    //double fDx, fDy, fDz;
    //fDx = fX - atom.x;
    //fDy = fY - atom.y;
    //fDz = fZ - atom.z;
    fResult =  2.0 * exp( -log2(2.0) * (fDx*fDx + fDy*fDy + fDz*fDz) / (r*r) );
    return fResult;
}
// van der Waal's potential
float fvdW(float radius, float fDx, float fDy, float fDz)
{
    // suggested contour 1.0
    double fResult = 0.0;
    float r = radius + rProbe;    
    //double fDx, fDy, fDz;
    //fDx = fX - radius.x;
    //fDy = fY - radius.y;
    //fDz = fZ - radius.z;
    double sigmaOverRsquared = (r*r) / (fDx*fDx + fDy*fDy + fDz*fDz);   
    fResult = pow(sigmaOverRsquared,3);
    return fResult;
}
// fill the fSample array with data to be contoured
void makeSample(atomQuery atom, int nx, int ny, int nz) {
    
    float x,y,z;
    float fval;
    for (int i=0; i<nx; ++i) {
        x = corner.x + i*incs.x;
        if ( abs(x-atom.x) > MINDIST) continue; // should be 0 by then
        for (int j=0; j<ny; ++j) {
            y = corner.y + j*incs.y;
            if ( abs(y-atom.y) > MINDIST) continue;
            for (int k=0; k<nz; ++k) {
                z = corner.z + k*incs.z;
                if ( abs(z - atom.z) > MINDIST) continue;
                fval = fSampleValue(Atom::radius[atom.atnum], x-atom.x, y-atom.y, z-atom.z);
                if (fval > fSample[i][j][k].value) fSample[i][j][k].ownAtom = atom.atid;
                fSample[i][j][k].value += fval;             
            }
        }
    }
}

//finds the vector perpendicular to a triangle of 3 Points            
Point vGetNormal(Point v[3]) {
    Point a,b;
    Point normal;
    // cross product
    a.x=v[1].x-v[0].x;
    a.y=v[1].y-v[0].y;
    a.z=v[1].z-v[0].z;
    b.x=v[2].x-v[0].x;
    b.y=v[2].y-v[0].y;
    b.z=v[2].z-v[0].z;
    normal.x = a.y * b.z - a.z * b.y;
    normal.y = a.z * b.x - a.x * b.z;
    normal.z = a.x * b.y - a.y * b.x;
    // normalize it to length of 1
    double flen = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
    if (flen > 0.0) {
        normal.x = normal.x/flen;
        normal.y = normal.y/flen;
        normal.z = normal.z/flen;
    }
    return normal;
}

// gets the gradient of the sample data at a Point
int NearAtoms = 0;
int NCalls = 0;
void vGetNormal(Point &rfNormal, Point &rfEdge)
{
    double fposx = 0.0, fposy = 0.0, fposz = 0.0, fminx = 0.0, fminy = 0.0, fminz = 0.0;
    double dxyz = 0.05;
    int natom = 0;
    for (atom->seek(-1, false); atom->next(); ) {
        double dx = rfEdge.x - atom->x;
        double dy = rfEdge.y - atom->y;
        double dz = rfEdge.z - atom->z;
        if ( abs(dx) > MINDIST) continue;
        if ( abs(dy) > MINDIST) continue;
        if ( abs(dz) > MINDIST) continue;
        fposx += fSampleValue(Atom::radius[atom->atnum], dx+dxyz, dy,      dz);
        fposy += fSampleValue(Atom::radius[atom->atnum], dx,      dy+dxyz, dz);
        fposz += fSampleValue(Atom::radius[atom->atnum], dx,      dy,      dz+dxyz);
        fminx += fSampleValue(Atom::radius[atom->atnum], dx-dxyz, dy,      dz);
        fminy += fSampleValue(Atom::radius[atom->atnum], dx,      dy-dxyz, dz);
        fminz += fSampleValue(Atom::radius[atom->atnum], dx,      dy,      dz-dxyz);
        ++natom;
    }
    NearAtoms += natom;
    ++NCalls;
    rfNormal.x = (fminx - fposx) / dxyz;
    rfNormal.y = (fminy - fposy) / dxyz;
    rfNormal.z = (fminz - fposz) / dxyz;
}

int Ntri = 0;
void vertexNormalOutput(Point Edge, Point Norm, int owner) {
    //printf("%f,%f,%f %f,%f,%f\n", Edge.x, Edge.y, Edge.z, Norm.x, Norm.y, Norm.z);
    //printf("%f,%f,%f\n", Edge.x, Edge.y, Edge.z);
    ++Ntri;
    addTriangle->addBindValue(Ntri);
    addTriangle->addBindValue(owner); // atom id        
    addTriangle->addBindValue(Edge.x);
    addTriangle->addBindValue(Edge.y);
    addTriangle->addBindValue(Edge.z);
    addTriangle->addBindValue(Norm.x);
    addTriangle->addBindValue(Norm.y);
    addTriangle->addBindValue(Norm.z);
    addTriangle->exec();
}
void triangleOutput(Point v[3], int owner) {
    ++Ntri;
    //printf("3,6,1\n");    
    Point normal = vGetNormal(v);
    for (int i=0; i<3; ++i) {
        //printf("%f,%f,%f %f,%f,%f\n", v[i].x, v[i].y, v[i].z, normal.x, normal.y, normal.z);
        addTriangle->addBindValue(Ntri);
        addTriangle->addBindValue(owner); // atom id        
        addTriangle->addBindValue(v[i].x);
        addTriangle->addBindValue(v[i].y);
        addTriangle->addBindValue(v[i].z);
        addTriangle->addBindValue(normal.x);
        addTriangle->addBindValue(normal.y);
        addTriangle->addBindValue(normal.z);
        addTriangle->exec();
    }
}
//vMarchCube1 performs the Marching Cubes algorithm on a single cube
void vMarchCube1(int ix, int iy, int iz, float fTargetValue, bool gradients)
{
    extern int aiCubeEdgeFlags[256];
    extern int a2iTriangleConnectionTable[256][16];
    
    int iCorner, iVertex, iVertexTest, iEdge, iTriangle, iFlagIndex, iEdgeFlags;
    float fOffset;
    float afCubeValue[8];
    Point asEdgeVertex[12];
    Point asEdgeNorm[12];
    
    int owner = 0;
    //Make a local copy of the values at the cube's corners
    for(iVertex = 0; iVertex < 8; iVertex++)
    {
        afCubeValue[iVertex] = fSample
                [ix+a2iVertexOffset[iVertex][0]]
                [iy+a2iVertexOffset[iVertex][1]]
                [iz+a2iVertexOffset[iVertex][2]].value;
        owner = std::max(owner, fSample
                [ix+a2iVertexOffset[iVertex][0]]
                [iy+a2iVertexOffset[iVertex][1]]
                [iz+a2iVertexOffset[iVertex][2]].ownAtom);
    }
    
    // the coordinates of the cube bottom corner
    float fX = corner.x + ix*incs.x;
    float fY = corner.y + iy*incs.y;
    float fZ = corner.z + iz*incs.z;
    
    //Find which vertices are inside of the surface and which are outside
    iFlagIndex = 0;
    for(iVertexTest = 0; iVertexTest < 8; iVertexTest++)
    {
        if(afCubeValue[iVertexTest] <= fTargetValue) 
            iFlagIndex |= 1<<iVertexTest;
    }
    
    //Find which edges are intersected by the surface
    iEdgeFlags = aiCubeEdgeFlags[iFlagIndex];

    //If the cube is entirely inside or outside of the surface, then there will be no intersections
    if(iEdgeFlags == 0) 
    {
        return;
    }
    
    //Find the point of intersection of the surface with each edge
    //Then find the normal to the surface at those points
    for(iEdge = 0; iEdge < 12; iEdge++)
    {
        //if there is an intersection on this edge
        if(iEdgeFlags & (1<<iEdge))
        {
            fOffset = fGetOffset(afCubeValue[ a2iEdgeConnection[iEdge][0] ], 
                                 afCubeValue[ a2iEdgeConnection[iEdge][1] ], fTargetValue);
            
            asEdgeVertex[iEdge].x = fX + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][0]  +  fOffset * a2fEdgeDirection[iEdge][0]) * incs.x;
            asEdgeVertex[iEdge].y = fY + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][1]  +  fOffset * a2fEdgeDirection[iEdge][1]) * incs.y;
            asEdgeVertex[iEdge].z = fZ + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][2]  +  fOffset * a2fEdgeDirection[iEdge][2]) * incs.z;
//            if (gradients) {
//                vGetNormal(asEdgeNorm[iEdge], asEdgeVertex[iEdge]);
//                vNormalizeVector(asEdgeNorm[iEdge], asEdgeNorm[iEdge]);
//            }
        }
    }
    
    //Draw the triangles that were found.  There can be up to five per cube
    Point tri[3];    
    for(iTriangle = 0; iTriangle < 5; iTriangle++)
    {
        if(a2iTriangleConnectionTable[iFlagIndex][3*iTriangle] < 0)
            break;
        
        for(iCorner = 0; iCorner < 3; ++iCorner)
        {
            iVertex = a2iTriangleConnectionTable[iFlagIndex][3*iTriangle+iCorner];
            if (gradients) {
                vertexNormalOutput(asEdgeVertex[iVertex], asEdgeNorm[iVertex], owner);
            } else {
                tri[iCorner] = asEdgeVertex[iVertex];
            }
        }
        if (!gradients) triangleOutput(tri, owner);        
    }
}

//vMarchTetrahedron performs the Marching Tetrahedrons algorithm on a single tetrahedron
void vMarchTetrahedron(Point *pasTetrahedronPosition, float *pafTetrahedronValue, float fTargetValue, bool /*gradients*/, int owner)
{
        extern int aiTetrahedronEdgeFlags[16];
        extern int a2iTetrahedronTriangles[16][7];

        int iEdge, iVert0, iVert1, iEdgeFlags, iTriangle, iCorner, iFlagIndex = 0;
        float fOffset, fInvOffset;
        Point asEdgeVertex[6];
        //Point asEdgeNorm[6];
        //Point sColor;

        //Find which vertices are inside of the surface and which are outside
        for(int iVertex = 0; iVertex < 4; iVertex++)
        {
                if(pafTetrahedronValue[iVertex] <= fTargetValue) 
                        iFlagIndex |= 1<<iVertex;
        }

        //Find which edges are intersected by the surface
        iEdgeFlags = aiTetrahedronEdgeFlags[iFlagIndex];

        //If the tetrahedron is entirely inside or outside of the surface, then there will be no intersections
        if(iEdgeFlags == 0)
        {
                return;
        }
        //Find the point of intersection of the surface with each edge
        // Then find the normal to the surface at those points
        for(iEdge = 0; iEdge < 6; iEdge++)
        {
                //if there is an intersection on this edge
                if(iEdgeFlags & (1<<iEdge))
                {
                        iVert0 = a2iTetrahedronEdgeConnection[iEdge][0];
                        iVert1 = a2iTetrahedronEdgeConnection[iEdge][1];
                        fOffset = fGetOffset(pafTetrahedronValue[iVert0], pafTetrahedronValue[iVert1], fTargetValue);
                        fInvOffset = 1.0 - fOffset;

                        asEdgeVertex[iEdge].x = fInvOffset*pasTetrahedronPosition[iVert0].x  +  fOffset*pasTetrahedronPosition[iVert1].x;
                        asEdgeVertex[iEdge].y = fInvOffset*pasTetrahedronPosition[iVert0].y  +  fOffset*pasTetrahedronPosition[iVert1].y;
                        asEdgeVertex[iEdge].z = fInvOffset*pasTetrahedronPosition[iVert0].z  +  fOffset*pasTetrahedronPosition[iVert1].z;
                }
        }
        //Draw the triangles that were found.  There can be up to 2 per tetrahedron
        for(iTriangle = 0; iTriangle < 2; iTriangle++)
        {
                if(a2iTetrahedronTriangles[iFlagIndex][3*iTriangle] < 0)
                        break;
                
                Point tri[3];
                for(iCorner = 0; iCorner < 3; iCorner++)
                {
                        int iVertex = a2iTetrahedronTriangles[iFlagIndex][3*iTriangle+iCorner];
                        tri[iCorner] = asEdgeVertex[iVertex];     
                }
                triangleOutput(tri, owner);                
        }
}



//vMarchCube2 performs the Marching Tetrahedrons algorithm on a single cube by making six calls to vMarchTetrahedron
void vMarchCube2(int ix, int iy, int iz, float fTargetValue, bool gradients)
{
        int iVertex, iTetrahedron, iVertexInACube;
        Point asCubePosition[8];
        float  afCubeValue[8];
        Point asTetrahedronPosition[4];
        float  afTetrahedronValue[4];
        
        // the coordinates of the cube bottom corner
        float fX = corner.x + ix*incs.x;
        float fY = corner.y + iy*incs.y;
        float fZ = corner.z + iz*incs.z;
        
        //Make a local copy of the cube's corner positions
        for(iVertex = 0; iVertex < 8; iVertex++)
        {
                asCubePosition[iVertex].x = fX + a2fVertexOffset[iVertex][0]*incs.x;
                asCubePosition[iVertex].y = fY + a2fVertexOffset[iVertex][1]*incs.y;
                asCubePosition[iVertex].z = fZ + a2fVertexOffset[iVertex][2]*incs.z;
        }

        int owner = 0;
        //Make a local copy of the cube's corner values
        for(int iVertex = 0; iVertex < 8; iVertex++)
        {
                afCubeValue[iVertex] = fSample
                        [ix+a2iVertexOffset[iVertex][0]]
                        [iy+a2iVertexOffset[iVertex][1]]
                        [iz+a2iVertexOffset[iVertex][2]].value;
                owner = std::max(owner, fSample
                                [ix+a2iVertexOffset[iVertex][0]]
                                [iy+a2iVertexOffset[iVertex][1]]
                                [iz+a2iVertexOffset[iVertex][2]].ownAtom);
        }

        for(iTetrahedron = 0; iTetrahedron < 6; iTetrahedron++)
        {
                for(iVertex = 0; iVertex < 4; iVertex++)
                {
                        iVertexInACube = a2iTetrahedronsInACube[iTetrahedron][iVertex];
                        asTetrahedronPosition[iVertex].x = asCubePosition[iVertexInACube].x;
                        asTetrahedronPosition[iVertex].y = asCubePosition[iVertexInACube].y;
                        asTetrahedronPosition[iVertex].z = asCubePosition[iVertexInACube].z;
                        afTetrahedronValue[iVertex] = afCubeValue[iVertexInACube];
                }
                vMarchTetrahedron(asTetrahedronPosition, afTetrahedronValue, fTargetValue, owner, gradients);
        }
}

//vMarch iterates over the entire dataset, calling vMarchCube on each cube
void vMarch(int nx, int ny, int nz, float fTargetValue, bool gradients)
{
    for(int iX = 1; iX < nx; iX++)
        for(int iY = 1; iY < ny; iY++)
            for(int iZ = 1; iZ < nz; iZ++)
            {
                vMarchCube(iX, iY, iZ, fTargetValue, gradients);
            }
}


// For any edge, if one vertex is inside of the surface and the other is outside of the surface
//  then the edge intersects the surface
// For each of the 4 vertices of the tetrahedron can be two possible states : either inside or outside of the surface
// For any tetrahedron the are 2^4=16 possible sets of vertex states
// This table lists the edges intersected by the surface for all 16 possible vertex states
// There are 6 edges.  For each entry in the table, if edge #n is intersected, then bit #n is set to 1

int aiTetrahedronEdgeFlags[16]=
{
    0x00, 0x0d, 0x13, 0x1e, 0x26, 0x2b, 0x35, 0x38, 0x38, 0x35, 0x2b, 0x26, 0x1e, 0x13, 0x0d, 0x00, 
};


// For each of the possible vertex states listed in aiTetrahedronEdgeFlags there is a specific triangulation
// of the edge intersection points.  a2iTetrahedronTriangles lists all of them in the form of
// 0-2 edge triples with the list terminated by the invalid value -1.
//
// I generated this table by hand

int a2iTetrahedronTriangles[16][7] =
{
    {-1, -1, -1, -1, -1, -1, -1},
    { 0,  3,  2, -1, -1, -1, -1},
    { 0,  1,  4, -1, -1, -1, -1},
    { 1,  4,  2,  2,  4,  3, -1},
    
    { 1,  2,  5, -1, -1, -1, -1},
    { 0,  3,  5,  0,  5,  1, -1},
    { 0,  2,  5,  0,  5,  4, -1},
    { 5,  4,  3, -1, -1, -1, -1},
    
    { 3,  4,  5, -1, -1, -1, -1},
    { 4,  5,  0,  5,  2,  0, -1},
    { 1,  5,  0,  5,  3,  0, -1},
    { 5,  2,  1, -1, -1, -1, -1},
    
    { 3,  4,  2,  2,  4,  1, -1},
    { 4,  1,  0, -1, -1, -1, -1},
    { 2,  3,  0, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1},
};

// For any edge, if one vertex is inside of the surface and the other is outside of the surface
//  then the edge intersects the surface
// For each of the 8 vertices of the cube can be two possible states : either inside or outside of the surface
// For any cube the are 2^8=256 possible sets of vertex states
// This table lists the edges intersected by the surface for all 256 possible vertex states
// There are 12 edges.  For each entry in the table, if edge #n is intersected, then bit #n is set to 1

int aiCubeEdgeFlags[256]=
{
    0x000, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c, 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00, 
    0x190, 0x099, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c, 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90, 
    0x230, 0x339, 0x033, 0x13a, 0x636, 0x73f, 0x435, 0x53c, 0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30, 
    0x3a0, 0x2a9, 0x1a3, 0x0aa, 0x7a6, 0x6af, 0x5a5, 0x4ac, 0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0, 
    0x460, 0x569, 0x663, 0x76a, 0x066, 0x16f, 0x265, 0x36c, 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60, 
    0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0x0ff, 0x3f5, 0x2fc, 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0, 
    0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x055, 0x15c, 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950, 
    0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0x0cc, 0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0, 
    0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc, 0x0cc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0, 
    0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c, 0x15c, 0x055, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650, 
    0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc, 0x2fc, 0x3f5, 0x0ff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0, 
    0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c, 0x36c, 0x265, 0x16f, 0x066, 0x76a, 0x663, 0x569, 0x460, 
    0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac, 0x4ac, 0x5a5, 0x6af, 0x7a6, 0x0aa, 0x1a3, 0x2a9, 0x3a0, 
    0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c, 0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x033, 0x339, 0x230, 
    0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c, 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x099, 0x190, 
    0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c, 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x000
};

//  For each of the possible vertex states listed in aiCubeEdgeFlags there is a specific triangulation
//  of the edge intersection points.  a2iTriangleConnectionTable lists all of them in the form of
//  0-5 edge triples with the list terminated by the invalid value -1.
//  For example: a2iTriangleConnectionTable[3] list the 2 triangles formed when corner[0] 
//  and corner[1] are inside of the surface, but the rest of the cube is not.
//
//  I found this table in an example program someone wrote long ago.  It was probably generated by hand

int a2iTriangleConnectionTable[256][16] =  
{
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
    {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
    {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
    {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
    {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
    {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
    {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
    {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
    {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
    {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
    {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
    {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
    {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
    {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
    {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
    {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
    {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
    {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
    {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
    {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
    {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
    {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
    {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
    {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
    {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
    {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
    {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
    {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
    {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
    {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
    {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
    {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
    {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
    {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
    {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
    {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
    {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
    {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
    {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
    {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
    {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
    {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
    {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
    {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
    {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
    {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
    {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
    {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
    {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
    {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
    {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
    {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
    {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
    {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
    {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
    {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
    {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
    {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
    {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
    {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
    {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
    {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
    {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
    {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
    {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
    {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
    {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
    {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
    {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
    {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
    {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
    {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
    {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
    {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
    {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
    {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
    {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
    {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
    {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
    {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
    {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
    {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
    {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
    {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
    {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
    {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
    {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
    {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
    {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
    {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
    {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
    {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
    {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
    {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
    {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
    {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
    {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
    {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
    {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
    {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
    {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
    {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
    {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
    {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
    {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
    {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
    {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
    {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
    {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
    {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
    {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
    {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
    {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
    {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
    {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
    {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
    {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
    {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
    {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
    {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
    {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
    {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
    {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
    {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
    {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
    {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
    {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
    {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
    {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
    {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
    {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
    {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
    {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
    {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
    {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
    {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
    {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
    {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
    {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
    {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
    {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
    {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
    {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
    {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
    {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
    {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
    {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
    {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
    {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
    {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
    {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
    {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
    {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
    {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
    {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
    {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};
