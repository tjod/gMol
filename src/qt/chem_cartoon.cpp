#include "chem.h"
int ChemWidget::drawCartoon(int imol, QString molnam, char chain, int mode, float ribbon_width, float coil_radius) {

  // set radius and open object
  //float spec[3] = {0,1,cradius};
  //float PI = 3.1415926;
  float spec[3] = {0,1,0.0};
  int size = 3;
  int  err = 0;
  if (mode & DRAW_OPEN)  err = getMem(molnam.toLocal8Bit().data(), spec, size, molnam.length());
  if (err) return err;

  // draw ribbon for this chain.
  int numres =  Db::chainNumRes(imol, chain);
  float A[3], B[3], C[3], D[3], P[3], Dp[3];
  float (*CA)[3]          = new float[numres+1][3];
  float (*O)[3]           = new float[numres+1][3];
  float (*guide_zero)[3]  = new float[numres+1][3];
  float (*guide_plus)[3]  = new float[numres+1][3];
  float (*guide_minus)[3] = new float[numres+1][3];
  float (*astrand)[3], (*bstrand)[3], (*fliptmp)[3];  // pointers to faciliate swapping sides for strands
  float coil_width = ribbon_width;
  //qDebug() << "drawCartoon " << chain;
  std::string sstype = Db::getChainSS(imol, chain, numres);
// get the CA and O coords
  int hires=0;
  int lores=0;
  int mres=0; // previous residue number
  QSqlQuery qchain = Db::iterChainCoords(imol, chain, FILTER_CARTOON);
  for (chainCoordRecord cc = Db::nextChainCoord(qchain); cc.valid; cc = Db::nextChainCoord(qchain)) {
    int ires = cc.resnum;
    int njump = ires - mres;
    if (cc.name == "CA") {
      CA[ires][0] = cc.x; CA[ires][1] = cc.y; CA[ires][2] = cc.z;
      hires = ires;
      if (sstype[ires] == '\0') {
        sstype[ires] = 'U';
      }
    } else if (cc.name == "O") {
      O[ires][0] = cc.x; O[ires][1] = cc.y; O[ires][2] = cc.z;
      if (njump > 1 && mres > 0) {
        // interpolate missing coords
        float xinc = (CA[ires][0] - CA[mres][0])/njump;
        float yinc = (CA[ires][1] - CA[mres][1])/njump;
        float zinc = (CA[ires][2] - CA[mres][2])/njump;
        for (int i=1; i<njump; ++i) {
          sstype[i+mres] = 'X'; // extrapolated
          CA[i+mres][0] = CA[mres][0] + xinc*i;
          CA[i+mres][1] = CA[mres][1] + yinc*i;
          CA[i+mres][2] = CA[mres][2] + zinc*i;
          O[i+mres][0] = O[mres][0] + xinc*i;
          O[i+mres][1] = O[mres][1] + yinc*i;
          O[i+mres][2] = O[mres][2] + zinc*i;
        }
      }
      mres = ires;
    }
    if (lores == 0) lores = ires;
  }
/*
// alter U and T of length 1 between ss of same type
  for (unsigned int i=1; i<sstype.length()-1; ++i) {
    if (sstype.at(i) == 'U' || sstype.at(i) == 'T') {
      if (sstype.at(i)   != sstype.at(i-1) &&
          sstype.at(i)   != sstype.at(i+1) &&
          sstype.at(i-1) == sstype.at(i+1)
      ) sstype.at(i) = sstype.at(i-1);
    }
  }
*/
//  compute the guide coordinates
  bool flip = false;
  bstrand = guide_minus;
  astrand = guide_plus;
  int nguide=0;
  //char current_style = '\0';
  for (int ires=lores; ires < hires; ++ires) {
    if (sstype[ires] == '\0') continue; // not encountered in pdb file
// compute guide points
    A[0] = CA[ires+1][0] - CA[ires][0];
    A[1] = CA[ires+1][1] - CA[ires][1];
    A[2] = CA[ires+1][2] - CA[ires][2];
    B[0] = O[ires][0]    - CA[ires][0];
    B[1] = O[ires][1]    - CA[ires][1];
    B[2] = O[ires][2]    - CA[ires][2];
    cross(C,A,B); // C = A x B 
    cross(D,C,A); // D = C x A
    P[0] = CA[ires][0] + (CA[ires+1][0] - CA[ires][0]) * 0.5;
    P[1] = CA[ires][1] + (CA[ires+1][1] - CA[ires][1]) * 0.5;
    P[2] = CA[ires][2] + (CA[ires+1][2] - CA[ires][2]) * 0.5;
/*
    if (current_style != sstype[ires]) {
      bstrand = guide_minus;
      astrand = guide_plus;
    } else {
*/
      if (nguide>0) flip = (vangle(D, Dp) > PI/2.);
      Dp[0] = D[0]; Dp[1] = D[1]; Dp[2] = D[2];
      if (flip) {
        fliptmp = astrand;
        astrand = bstrand;
        bstrand = fliptmp;
      }
    //}
// deal with each style
    if (sstype[ires] == 'S') {
      bstrand[nguide][0] = P[0] - D[0]*ribbon_width;
      bstrand[nguide][1] = P[1] - D[1]*ribbon_width;
      bstrand[nguide][2] = P[2] - D[2]*ribbon_width;
      astrand[nguide][0] = P[0] + D[0]*ribbon_width;
      astrand[nguide][1] = P[1] + D[1]*ribbon_width;
      astrand[nguide][2] = P[2] + D[2]*ribbon_width;
    } else if (sstype[ires] == 'H') {
      bstrand[nguide][0] = P[0] + C[0]*HELIX_OFFSET - D[0] * ribbon_width;
      bstrand[nguide][1] = P[1] + C[1]*HELIX_OFFSET - D[1] * ribbon_width;
      bstrand[nguide][2] = P[2] + C[2]*HELIX_OFFSET - D[2] * ribbon_width;
      astrand[nguide][0] = P[0] + C[0]*HELIX_OFFSET + D[0] * ribbon_width;
      astrand[nguide][1] = P[1] + C[1]*HELIX_OFFSET + D[1] * ribbon_width;
      astrand[nguide][2] = P[2] + C[2]*HELIX_OFFSET + D[2] * ribbon_width;
    } else {
      bstrand[nguide][0] = P[0] - D[0]*coil_width;
      bstrand[nguide][1] = P[1] - D[1]*coil_width;
      bstrand[nguide][2] = P[2] - D[2]*coil_width;
      astrand[nguide][0] = P[0] + D[0]*coil_width;
      astrand[nguide][1] = P[1] + D[1]*coil_width;
      astrand[nguide][2] = P[2] + D[2]*coil_width;
    }
    //current_style = sstype[ires];
    guide_zero[nguide][0] = guide_minus[nguide][0] + (guide_plus[nguide][0] - guide_minus[nguide][0]) * 0.5;
    guide_zero[nguide][1] = guide_minus[nguide][1] + (guide_plus[nguide][1] - guide_minus[nguide][1]) * 0.5;
    guide_zero[nguide][2] = guide_minus[nguide][2] + (guide_plus[nguide][2] - guide_minus[nguide][2]) * 0.5;
    //qDebug() << ires << hires << numres << nguide << astrand[nguide][0] << astrand[nguide][1] << astrand[nguide][2];
    ++nguide;
  }

// now draw the ribbon
  if (hires > 1) {
    makeRibbon(nguide, (float *)guide_minus, (float *)guide_plus, (float *)guide_zero, sstype, lores, hires, coil_radius, ribbon_width, false, true);
  } else {
    qDebug() << "no main chain atoms found to create ribbon.";
  }
  if (mode & DRAW_CLOSE) err += getMemDone("", NULL, 0, 0);

  delete[] CA;
  delete[] O;
  delete[] guide_minus;
  delete[] guide_plus;
  delete[] guide_zero;
  return err;
}
int ChemWidget::sendCoil(float *spline, float *guide, int offset, int slength, float xradius, float yradius) {
  int err = 0;
  if (xradius+yradius > 0) {
  // cylinders
    int nwrap = 12;
    float *outbuff = wrap((float (*)[3])&spline[3+offset*3], (guide ? (float (*)[3])&guide[3+offset*3] : NULL), slength,
       xradius, yradius, 1, nwrap);
    outbuff[1] = 7; outbuff[2] = 1;
    int nout = 3 + outbuff[0]*6 + 6;
    err += getMemMore("", outbuff, nout, 0);
/*
// reuse outbuff for spheres
    nout = 0;
    outbuff[nout++]=2; outbuff[nout++]=5; outbuff[nout++]=0;
    int p = 3+offset*3;
    outbuff[nout++]=spline[p]; outbuff[nout++]=spline[p+1]; outbuff[nout++]=spline[p+2]; outbuff[nout++]=xradius;
    p = 3+offset*3+slength*3 - 3;
    outbuff[nout++]=spline[p]; outbuff[nout++]=spline[p+1]; outbuff[nout++]=spline[p+2]; outbuff[nout++]=xradius;
    err += getMemMore("", outbuff, nout, 0);
*/
    free (outbuff);
   } else {
    // lines
    int size = 3;
    float spec[3] = {slength,2,3};
    err += getMemMore("", spec, size, 0);
    int nout = slength*3;
    err += getMemMore("", &spline[3+offset*3], nout, 0);
  }
  return err;
}
int ChemWidget::sendBase(float spline_minus[][3], float spline_plus[][3], float thick) {
  float spec[3] = {4,7,3};
  int err = getMemMore("", spec, 3, 0);
  float coord[4][3];
  float a[3] = {spline_minus[0][0], spline_minus[0][1], spline_minus[0][2]};
  float b[3] = { spline_plus[0][0],  spline_plus[0][1],  spline_plus[0][2]};
  float c[3] = { spline_plus[1][0],  spline_plus[1][1],  spline_plus[1][2]};
  float nml[3];
  normal(a,b,c, nml);
  nml[0] = nml[0] * thick;
  nml[1] = nml[1] * thick;
  nml[2] = nml[2] * thick;

  int nout=0;
  coord[nout][0] = a[0] - nml[0]; coord[nout][1] = a[1] - nml[1]; coord[nout][2] = a[2] - nml[2];
  ++nout;
  coord[nout][0] = b[0] - nml[0]; coord[nout][1] = b[1] - nml[1]; coord[nout][2] = b[2] - nml[2];
  ++nout;
  coord[nout][0] = b[0] + nml[0]; coord[nout][1] = b[1] + nml[1]; coord[nout][2] = b[2] + nml[2];
  ++nout;
  coord[nout][0] = a[0] + nml[0]; coord[nout][1] = a[1] + nml[1]; coord[nout][2] = a[2] + nml[2];
  ++nout;
  err += getMemMore("", &coord[0][0], nout*3, 0);
  float tri[][3] = { {2,8,1}, {3,1,2}, {1,3,4} };
  err += getMemMore("", &tri[0][0], 9, 0);
  return err;
}
int ChemWidget::sendArrow(float spline_minus[][3], float spline_plus[][3], int span, float thick) {
  
  int np = (thick > 0) ? 6 : 3;
  float spec[3] = {np,7,3};
  int size = 3;
  int err = 0;
  err += getMemMore("", spec, size, 0);
  float coord[6][3];
  int nout = 0;
  int ntri = 0;
  float a[3] = {spline_minus[0][0] - (spline_plus[0][0] - spline_minus[0][0]) * 0.25,
                spline_minus[0][1] - (spline_plus[0][1] - spline_minus[0][1]) * 0.25,
                spline_minus[0][2] - (spline_plus[0][2] - spline_minus[0][2]) * 0.25};
  float b[3] = {spline_plus[0][0] - (spline_minus[0][0] - spline_plus[0][0]) * 0.25,
                spline_plus[0][1] - (spline_minus[0][1] - spline_plus[0][1]) * 0.25,
                spline_plus[0][2] - (spline_minus[0][2] - spline_plus[0][2]) * 0.25};
  float c[3] = {spline_plus[span][0] + (spline_minus[span][0] - spline_plus[span][0]) * 0.5,
                spline_plus[span][1] + (spline_minus[span][1] - spline_plus[span][1]) * 0.5,
                spline_plus[span][2] + (spline_minus[span][2] - spline_plus[span][2]) * 0.5};
  if (thick > 0) {
    float an[3] = {spline_minus[0][0], spline_minus[0][1], spline_minus[0][2]};
    float bn[3] = {spline_plus[0][0], spline_plus[0][1], spline_plus[0][2]};
// looks better if arrow head takes normals from previous segment
    float cn[3] = {spline_plus[1][0], spline_plus[1][1], spline_plus[1][2]};
    float nml[3];
    normal(an,bn,cn, nml);
    nml[0] = nml[0] * thick;
    nml[1] = nml[1] * thick;
    nml[2] = nml[2] * thick;

    coord[nout][0] = a[0] - nml[0]; coord[nout][1] = a[1] - nml[1]; coord[nout][2] = a[2] - nml[2];
    ++nout;
    coord[nout][0] = b[0] - nml[0]; coord[nout][1] = b[1] - nml[1]; coord[nout][2] = b[2] - nml[2];
    ++nout;
    coord[nout][0] = c[0] - nml[0]; coord[nout][1] = c[1] - nml[1]; coord[nout][2] = c[2] - nml[2];
    ++nout;
    coord[nout][0] = a[0] + nml[0]; coord[nout][1] = a[1] + nml[1]; coord[nout][2] = a[2] + nml[2];
    ++nout;
    coord[nout][0] = b[0] + nml[0]; coord[nout][1] = b[1] + nml[1]; coord[nout][2] = b[2] + nml[2];
    ++nout;
    coord[nout][0] = c[0] + nml[0]; coord[nout][1] = c[1] + nml[1]; coord[nout][2] = c[2] + nml[2];
    ++nout;
    ntri = 8;
  } else {
    coord[nout][0] = a[0]; coord[nout][1] = a[1]; coord[nout][2] = a[2];
    ++nout;
    coord[nout][0] = b[0]; coord[nout][1] = b[1]; coord[nout][2] = b[2];
    ++nout;
    coord[nout][0] = c[0]; coord[nout][1] = c[1]; coord[nout][2] = c[2];
    ++nout;
    ntri = 1;
  }
  nout *= 3;
  err += getMemMore("", &coord[0][0], nout, 0);
  float tri[][3] = { {ntri,8,1}, {3,2,1}, {4,5,6}, {5,2,3}, {6,5,3}, {1,2,5}, {1,5,4}, {6,3,1}, {1,4,6} };
  nout = ntri*3 + 3;
  err += getMemMore("", &tri[0][0], nout, 0);
  return err;
}

int ChemWidget::sendRibbon(float spline_minus[][3], float spline_plus[][3], int slength, float thick, float width, int ntaper, int draw_edges) {
  int size = 3;
  int err = 0;
  int nout;
/*
  // lines/edges
  spec[0] = slength; spec[1] = 2; spec[2] = 3;
  err += getMemMore("", spec, size, 0);
  err += getMemMore("", &spline_minus[3+offset*3], nout, 0);
  err += getMemMore("", spec, size, 0);
  err += getMemMore("", &spline_plus[3+offset*3], nout, 0);
  return err;
*/

  bool use_nml = true;
  // four splines (edges of ribbons, each offset by thick) 
  int nspline = 0;
  int ldm2;
  if (thick == 0) {
    nspline = 2;
    //fprintf(stdout, "%d,0,3\n", slength*nspline);
  } else {
    //nspline = 4;
    nspline = 2;
  }
  if (use_nml) {
    nout = 6 * nspline;
    if (draw_edges > 0) ldm2 = 1;
    else ldm2 = 2;
  } else {
    nout = 3 * nspline;
    ldm2 = 3;
  }
  float spec[3] = {slength*nspline, 7, ldm2};
  err += getMemMore("", spec, size, 0);
  float edges[2][slength+1][3]; // zero element is n,ldm1,ldm2 expected but unused by sendCoil
  float nml[3];
  float oldnml[3] = {0,0,0};
  for (int i=0; i<slength; ++i) {
    float a[3] = {spline_minus[i][0], spline_minus[i][1], spline_minus[i][2]};
    float b[3] = {spline_plus [i][0], spline_plus [i][1], spline_plus [i][2]};
    float ab[3] = {(b[0] - a[0]), (b[1] - a[1]),  (b[2] - a[2])};
    float c[3] = {spline_plus[i+1][0], spline_plus[i+1][1], spline_plus[i+1][2]};
    if (ntaper > 0) {
      if (i < ntaper || i > slength-1-ntaper) {
// start to taper into mid-point for first and last ntaper points
        float f;
        if (i < ntaper) {
          // taper up to full width
          if (draw_edges || thick == 0) {
          //if (thick == 0) {
            f =  (float(ntaper-i)/ntaper) * 0.49;
          } else {
            f = (float(ntaper-i)/ntaper) * (width-abs(thick)*2)/width * 0.49;
          }
        } else {
          // taper down from full width
          if (draw_edges || thick == 0) {
          //if (thick == 0) {
            f = (float(ntaper-(slength-1-i))/ntaper) * 0.49;
          } else {
            f = (float(ntaper-(slength-1-i))/ntaper) * (width-abs(thick)*2)/width * 0.49;
          }
        }
        // adjust edges
        a[0] = a[0] + ab[0]*f; a[1] = a[1] + ab[1]*f; a[2] = a[2] + ab[2]*f;
        b[0] = b[0] - ab[0]*f; b[1] = b[1] - ab[1]*f; b[2] = b[2] - ab[2]*f;
        c[0] = c[0] - ab[0]*f; c[1] = c[1] - ab[1]*f; c[2] = c[2] - ab[2]*f;
      }
    }

    normal(a,b,c, nml);
    float d[3] = {nml[0] * thick, nml[1] * thick, nml[2] * thick};
    if(i>0) {
      nml[0] = (nml[0] + oldnml[0] ) * 0.5;
      nml[1] = (nml[1] + oldnml[1] ) * 0.5;
      nml[2] = (nml[2] + oldnml[2] ) * 0.5;
    }
    oldnml[0] = nml[0]; oldnml[1] = nml[1]; oldnml[2] = nml[2];
    if (thick != 0) {
      // add to points along normal scaled to width
      if (use_nml) {
        float xyz[][3] = {
           {a[0]+d[0], a[1]+d[1], a[2]+d[2]}, { nml[0],  nml[1],  nml[2]},
           {b[0]+d[0], b[1]+d[1], b[2]+d[2]}, { nml[0],  nml[1],  nml[2]} };
        err += getMemMore("", &xyz[0][0], nout, 0);
      } else {
        float xyz[][3] = {
           {a[0]+d[0], a[1]+d[1], a[2]+d[2]}, 
           {b[0]+d[0], b[1]+d[1], b[2]+d[2]}};
        err += getMemMore("", &xyz[0][0], nout, 0);
      }
    } else {
      if (use_nml) {
        float xyz[][3] = { {a[0], a[1], a[2]}, {nml[0], nml[1], nml[2]},
                           {b[0], b[1], b[2]}, {nml[0], nml[1], nml[2]} };
        err += getMemMore("", &xyz[0][0], nout, 0);
      } else {
        float xyz[][3] = { {a[0], a[1], a[2]}, {b[0], b[1], b[2]} };
        err += getMemMore("", &xyz[0][0], nout, 0);
      }
    }
    if (draw_edges == 1) {
      edges[0][i+1][0] = a[0]+d[0]; edges[0][i+1][1] = a[1]+d[1]; edges[0][i+1][2] = a[2]+d[2];
      edges[1][i+1][0] = a[0]-d[0]; edges[1][i+1][1] = a[1]-d[1]; edges[1][i+1][2] = a[2]-d[2];
    } else if (draw_edges == -1) {
      edges[0][i+1][0] = b[0]+d[0]; edges[0][i+1][1] = b[1]+d[1]; edges[0][i+1][2] = b[2]+d[2];
      edges[1][i+1][0] = b[0]-d[0]; edges[1][i+1][1] = b[1]-d[1]; edges[1][i+1][2] = b[2]-d[2];
    } else if (draw_edges == 2) {
      edges[0][i+1][0] = a[0]; edges[0][i+1][1] = a[1]; edges[0][i+1][2] = a[2];
    } else if (draw_edges == -2) {
      edges[0][i+1][0] = b[0]; edges[0][i+1][1] = b[1]; edges[0][i+1][2] = b[2];
    }
  }
  // automesh
  spec[0] = 1; spec[1] = 8;
  if (draw_edges > 0) spec[2] = 3;
  else spec[2] = -3;
  err += getMemMore("", spec, size, 0);
  spec[0] = nspline; spec[1] = slength; spec[2] = 0;
  err += getMemMore("", spec, size, 0);
/*
  if (thick != 0) {
    // cap off the ends
    int ntri = 4;
    int s = slength*nspline - 4;
    float tri[][3] = { {ntri,8,1}, {3,2,1}, {4,3,1}, {s+1,s+2,s+3}, {s+1,s+3,s+4}  };
    nout = ntri*3 + 3;
    err += getMemMore("", &tri[0][0], nout, 0);
  }
*/

  if (draw_edges == 2) {
    sendCoil((float *)&edges[0][0][0], NULL, 0, slength, thick*1.2, thick*1.2);
  } else if (draw_edges == -2) {
    sendCoil((float *)&edges[0][0][0], NULL, 0, slength, -thick*1.2, -thick*1.2);
  }  else if (draw_edges ==  1) {
    //sendCoil((float *)&edges[0][0][0], NULL, 0, slength, thick*1.2, thick*1.2);
    //sendCoil((float *)&edges[1][0][0], NULL, 0, slength, thick*1.2, thick*1.2);
    sendRibbon(&edges[0][1], &edges[1][1], slength, 0, thick, 0, 0);
  }  else if (draw_edges == -1) {
    sendRibbon(&edges[0][1], &edges[1][1], slength, 0, -thick, 0, 0);
  }
  return err;
}
int ChemWidget::makeRibbon(int nguide, float *guide_minus, float *guide_plus, float *guide_zero, std::string sstype, int lores, int hires, float coil_radius, float width, bool endcaps, bool showUnknown) {
  //  makes coils and ribbons, depending on the secondary struture codes 'H', 'S', 'T', 'U'/unknown, 'X'/extrapolated
  int err = 0;

  int nspline = 8; // any reasonable value should do
  float *spline_minus = makeSpline(guide_minus, nguide, nspline, Bspline, true);
  float *spline_plus  = makeSpline(guide_plus,  nguide, nspline, Bspline, true);
  float *spline_zero  = makeSpline(guide_zero,  nguide, nspline, Bspline, true);
  int spline_len = spline_zero[0]; //makeSpline returns n,ldm1,ldm1 in first 3 elements

  int begin = 0, b=0;
  int len;
  int ires=lores;
  int nspan;
  float thickness = coil_radius;
  while (ires <= hires) {
/*
    if (sstype[ires] == '\0') {
      ++ires;
      continue;
    }
*/
    // span residues of same secondary-structure type
    int n = sstype.find_first_not_of(sstype[ires], ires);
    if (n == -1)  {
      nspan = hires - ires + 1;
      len = spline_len - begin;
    } else {
      nspan = n - ires;
      if (sstype[ires] == '\0') {
        len = nspline/2;
      } else if (ires == 1) {
        len = (nspan-1)*(nspline+1) + 1;
      } else {
        len = nspan*(nspline+1) + 1;
        if (len+begin > spline_len) len = spline_len - begin;
      }
    }
    if (sstype[ires] == 'H') {
      // helix 
      b = (begin+1)*3;
      if (thickness > 0) {
        sendRibbon((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], len,  thickness, width, nspline,  2);
        sendRibbon((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], len, -thickness, width, nspline, -2);
      } else {
        sendRibbon((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], len,  thickness, width, nspline, 0);
      }
      //sendCoil(spline_minus, spline_zero, begin, len, coil_radius, coil_radius);
      //sendCoil(spline_plus, spline_zero, begin, len, coil_radius, coil_radius);
    } else if (sstype[ires] == 'S') {
      // strand with arrow end
      //sendCoil(spline_zero, spline_minus, begin, len-nspline/2-3, RIBBON_WIDTH, RIBBON_WIDTH/4); // rounded; experimental
      b = (begin+1)*3;
      if (thickness > 0) {
        sendRibbon((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], len-nspline,  thickness, width, 0,  1);
        sendRibbon((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], len-nspline, -thickness, width, 0, -1);
        sendBase((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], thickness);
      } else {
        sendRibbon((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], len-nspline,  thickness, width, 0, 0);
      }
      b = (begin+len-nspline)*3;
      sendArrow((float (*)[3])&spline_minus[b], (float (*)[3])&spline_plus[b], nspline, thickness);
    } else if (sstype[ires] == 'T') {
      // turn is a coil/rope
      if (len > 1) sendCoil(spline_zero, spline_minus, begin, len, coil_radius, coil_radius);
    } else if (sstype[ires] == 'U' && showUnknown) {
      // unknown is a coil/rope
      if (len > 1) sendCoil(spline_zero, spline_minus, begin, len, coil_radius, coil_radius);
    }
    //qDebug() << sstype[ires] << ires << n-1 << nspan << begin << len << begin+len << spline_len;
    ires += nspan;
    begin += len - 1;
  }
  if (endcaps && thickness > 0) {
    // sphere endcaps; reuse n,ldm1,ldm2 in spline_zero
    float outbuff[11] = {2,5,0}; // 2 spheres with radius
    int nout = 2;
    outbuff[++nout] = spline_zero[3];
    outbuff[++nout] = spline_zero[4];
    outbuff[++nout] = spline_zero[5];
    outbuff[++nout] = coil_radius;
    // begin is at the end now
    int p = (begin+1)*3;
    outbuff[++nout]=spline_zero[p];
    outbuff[++nout]=spline_zero[p+1];
    outbuff[++nout]=spline_zero[p+2];
    outbuff[++nout]=coil_radius;
    ++nout;
    err += getMemMore("", outbuff, nout, 0);
  }
  free(spline_zero);
  free(spline_minus);
  free(spline_plus);

  return err;
}

void ChemWidget::normal(float a[3], float b[3], float c[3], float nml[3]) {

  float v1[3], v2[3];
  float nl;

//  for triangle with points a,b,c, find normal to it.

//  First, let v1=a-c and v2=b-c (vectors)
  for (int  i=0; i<3; ++i) {
    v1[i]=a[i]-c[i];
    v2[i]=b[i]-c[i];
  }

//  Then take cross v1xv2
  nml[0] = v1[1]*v2[2] -v2[1]*v1[2];
  nml[1] = v1[2]*v2[0] -v2[2]*v1[0];
  nml[2] = v1[0]*v2[1] -v2[0]*v1[1];

// test for length
  nl = sqrt(nml[0]*nml[0] + nml[1]*nml[1] + nml[2]*nml[2]);
  if (nl < 1.e-8) {
    printf ("teeny normal length = %f\n", nl);
    return;
  }

// now normalize the normal vertor to length 1.0
  for (int  i=0; i<3; ++i) {
    nml[i]=nml[i]/nl;
  }
  return;
}

