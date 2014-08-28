C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      CHARACTER DEFDIR*100,DIR*100,NAME*39,REST*39,DEFEXT*10
      CHARACTER*255 FULLFILE
      CHARACTER*40 GRLIB
      COMMON /FILEINFO/ DEFDIR,DIR,NAME,REST,DEFEXT,GRLIB, FULLFILE
      SAVE /FILEINFO/
