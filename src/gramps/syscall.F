C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE SYSCALL(COMMAND)
C
      CHARACTER COMMAND*(*)
C
C     WRITTEN  3/1/82  TJ O'DONNELL
C
      Call SYSTEM(COMMAND)
C
      RETURN
      END
