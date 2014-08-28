C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE MATCON(T,U)
C
      DIMENSION S(4,4)
      DIMENSION T(4,4),U(4,4)
C
C       THIS ROUTINE CONCATENATES T AND U SO THAT T=UT.
C       S IS TEMPORARY, HOLDING T, UNTIL IT IS REPLACED.
C
      DO 10 J=1,4
      DO 10 I=1,4
      S(I,J)=T(I,J)
 10   CONTINUE
C
      DO 40 I=1,4
      DO 30 J=1,4
      TEMP=0.0
        DO 20 K=1,4
        TEMP=TEMP + U(I,K)*S(K,J)
 20     CONTINUE
      T(I,J)=TEMP
 30   CONTINUE
C
 40   CONTINUE
C
      RETURN
      END
