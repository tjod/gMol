C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE REBUILD(ARG,SWITCHES)
C
      IMPLICIT NONE
      CHARACTER ARG*(*)
      CHARACTER SWITCHES*(*)
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C       This routine rebuilds the display list for an object
C       from the copy of the pix file stored with GET.
C       This is useful (necessary) when colored sphere/cylinders
C       are in the pix file and the user wants to change the
C       material via INTENSITY and the ambient, diffuse or specular
C       keywords.
C
C       GET THE OBJ NAME.
C
      POINT=1
 1    CALL NAMID(ARG,POINT,ID)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
C
      call g0refill(id)
      IF(POINT.NE.0)GOTO 1
C
      DCHANGE=.TRUE.
 999  RETURN
      END
