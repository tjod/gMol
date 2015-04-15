#include "chem.h"
#include "spline.h"

int ChemWidget::sendChain(int ntrace, float *coord, float cradius, float sradius, int spline) {
  float spec[3];
  int size = 3;
  int err = 0;

  if (sradius > 0) {
    // spheres
    spec[0] = ntrace; spec[1] = 5; spec[2] = 3;
    err += getMemMore("", spec, size, 0);
    int n = ntrace * 3;
    err += getMemMore("", coord, n, 0);

  } else {
    int nspline = 8;
    int nwrap = 12;
    float *outbuff = makeSpline(coord, ntrace, nspline, spline, true);
    int nout;
    if (cradius > 0) {
    // cylinders
      outbuff[1] = 3; outbuff[2] = 3;
      float *woutbuff = wrap((float (*)[3])&outbuff[3], NULL, outbuff[0], cradius, cradius, 1, nwrap);
      free (outbuff);
      outbuff = woutbuff;
      nout = 3 + outbuff[0]*6 + 6;
    } else {
      // lines
      outbuff[1] = 2; outbuff[2] = 3;
      nout = 3 + outbuff[0]*3;
    }
    err += getMemMore("", outbuff, nout, 0);
    free (outbuff);
  }
  return err;
}

int ChemWidget::drawChain(int imol, QString molnam, char chain, int mode, float sradius, float cradius, bool endcaps) {
    
    // set cylinder radius and open object
    float spec[3] = {0,1,cradius};
    int size = 3;
    int  err = 0;
    if (mode & DRAW_OPEN)  err = getMem(molnam.toLocal8Bit().data(), spec, size, molnam.length());
    if (err) return err;
    if (sradius > 0) {
        // sphere radius
        spec[1] = 9;
        spec[2] = sradius;
        err = getMemMore("", spec, size, 0);
        if (err) return err;
    }
    
    // draw an alpha-carbon trace for this chain.
    int numres = chainQuery::numRes(imol, chain);
    float (*coord)[3] = new float[numres+1][3];
    int ntrace = 0;
    //qDebug() << "drawChain " << chain;
    float ax=0, ay=0, az=0;
    int reslast = 0;
    QString chainType = "";
    //QSqlQuery qchain = Db::iterChainCoords(imol, chain, FILTER_TRACE);
    chainQuery cq;
    for (cq.iter(imol, chain, FILTER_TRACE); cq.next(); ) {
        if (ntrace) {
            if (cq.resnum-reslast > 1 ) {
                if (!endcaps) {
                    err += sendChain(ntrace, (float *)coord, cradius, sradius, CatRom);
                }
                ntrace = 0;
            }
            if (chainType != cq.name) continue; // don't mix CA and P
        } else {
            chainType = cq.name;  // CA or P trace; assume using first atom
        }
        reslast = cq.resnum;
        if (endcaps && ntrace) { // skip all but first and last spheres
            ax = cq.x;
            ay = cq.y;
            az = cq.z;
        } else {
            coord[ntrace][0] = ax = cq.x;
            coord[ntrace][1] = ay = cq.y;
            coord[ntrace][2] = az = cq.z;
        }
        ntrace++;
    }
    if (endcaps) {
        coord[1][0] = ax;
        coord[1][1] = ay;
        coord[1][2] = az;
        ntrace = 2;
    }
    if (!err) {
        if (ntrace > 0) err += sendChain(ntrace, (float *)coord, cradius, sradius, CatRom);
        if (mode & DRAW_CLOSE) err += getMemDone("", NULL, 0, 0);
    }
    delete [] coord;
    return err;
}

QList<QString> ChemWidget::addChainsRow(QTreeWidgetItem *root, int imol) {

  QList<QString> gnames; // names of gramps objects created to know how to group
  QTreeWidgetItem *chainRow;
  //QString molnam;
  int chainsToDraw = 5;  // how many chains to draw into initially
  QHash<QString, int> chainCount;
  QSqlQuery qchain = Db::iterChainCounts(imol);
  int nchain = 0;
  QColor chainColor = nextColor(COLOR_DEFAULT);
  for (chainCount = Db::nextChainCounts(qchain); chainCount.count() > 0; chainCount = Db::nextChainCounts(qchain)) {
      ++nchain;
      chainColor.setHsv(chainColor.hue() + 30, chainColor.saturation(), chainColor.value());
      int ndrawn = 0; // objects drawn in this chain; limit to 1 initially
      // add a row for this chain - will be a gramps group.
      //nres = Db::resCount(imol, chain.name, &ntrace, &nhoh, &nhet, &nmain, &nside);
      QString text;
      char chain = chainCount["chain"];
      //int nres = chainCount["nres"];
      QTextStream(&text) << tr("Chain") + " " << chain << "(" << (chainCount["nres"]-chainCount["Water"]) << ")";
      Qt::CheckState drawChecked = (nchain < chainsToDraw) ? Qt::Checked : Qt::Unchecked;
      chainRow = addMolRow(root, text, NORESNUM, drawChecked, imol, NOATOM, chain, "Chain", STYLE_NONE, COLOR_BY_SOLID, chainColor, FILTER_NONE);
      //updateColorIcon(chainRow, chainColor, COLOR_BY_SOLID);
      //if (drawChecked == Qt::Checked) expandItem(chainRow);
      if (nchain == 1) expandItem(chainRow);
      QString chainGrampsName = currentRow.grampsName;
      QString mname = "";
      for (int i=0; i<Db::filters.count(); ++i) {
          if (Db::filters[i].appearance != "T") continue;
          QString name = Db::filters[i].menu;
          int nres = chainCount[name];
          //qDebug() << name << n << "atoms.";
          if (nres == 0) continue;
          int filterid = Db::filters[i].id;
          int style = STYLE_DEFAULT;
          int colorby = COLOR_BY_ATOMS;
          QColor color = chainColor;
          QString text = name;
          if (name == "Trace") {
              style =  (nres < 20) ? STYLE_CYLINDERS : STYLE_LINES;
          } else if (name == "Cartoon") {
              style = STYLE_CARTOON_THICK;
              colorby = COLOR_BY_SOLID;
          } else if (name == "Main") {
              style =  (nres < 20) ? STYLE_DEFAULT : STYLE_LINES;
          } else if (name == "Side") {
              style =  (nres < 20) ? STYLE_DEFAULT : STYLE_LINES;
          } else if (name == "Water") {
              style = STYLE_ATOM_SPHERE;
              colorby = COLOR_BY_SOLID;
              color = COLOR_WATER;
              QTextStream(&text) << " (" << nres << ")";
          }
          QTreeWidgetItem *newRow = addMolRow(chainRow, text, NORESNUM, drawChecked, imol, NOATOM, chain, name, style, colorby, color, filterid);
          if (ndrawn == 0 && nchain < chainsToDraw) {
              mname = drawMol(newRow);
              ++ndrawn;
              drawChecked = Qt::Unchecked;
          }
      }

    bool dummy = false;
    if (mname.isEmpty()) {
      // dummy placeholder to create group for later objects
      mname = encodeMolName(imol, chain, NORESNUM, "dummy");
      drawDummy(mname);
      dummy = true;
    }
    QString cmd = "group " + mname   + "," + mname   + " " + chainGrampsName;
    emit cmdReady(cmd);
    emit cmdReady("rebuild "+mname);  // workaround for odd bug that prevents first obj from being shown
    gnames.append(chainGrampsName);
    if (dummy) {
      QString cmd = "forget " + mname;
      emit cmdReady(cmd);
    }
  }

  // collect all the chains into one group...caller will add the MOLname to complete this command.
  //QString cmd = "group " + gnames.first() + "," + gnames.last();
  //return cmd;
  return gnames;
}
