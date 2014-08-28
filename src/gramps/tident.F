C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE TIDENT
C
      IMPLICIT NONE
#include "UDLIST.FOR"
C
      IF(OUTTING.EQ.OUTWINDOW)THEN
        CALL T0IDENT
      ELSE
        CALL T1IDENT
      ENDIF
C
      RETURN
      END
C
      SUBROUTINE T0IDENT
C
C      REAL TMPMAT(4,4)
C
C       THIS ROUTINE SET THE CURRENT TFM MATRIX TO
C       BE THE UNIT MATRIX.
C
C      CALL UNIT(TMPMAT)
C      CALL LOADMA(TMPMAT)
			call identity_matrix
C
      RETURN
      END
C
      SUBROUTINE T1IDENT
C
#include "TFMMAT.FOR"
C
C       THIS ROUTINE SET THE CURRENT TFM MATRIX TO
C       BE THE UNIT MATRIX.
C
      CALL UNIT(TFMMTX(1,1,CURRENT))
C
      RETURN
      END
