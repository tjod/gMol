C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      FUNCTION TYPOBJ(NID)
C
      IMPLICIT NONE
	INTEGER NID
C
#include "UDLIST.FOR"
C
C       THIS ROUTINE IS A GRAMPS UTILITY ROUTINE TO
C       TELL WHICH TYPE OF OBJECT WE'RE DEALING WITH.
C
C       THE CONVENTION IS THAT THE HIGH ORDER BYTE
C       OF THE ELEMENTS OF KINDOBJ ARE MODIFIER BITS
C       AND THE LOW ORDER BYTE IS AN INTEGER 
C       IDENTIFYING THE TYPE OF OBJECT.
C
C       REMOVE SECOND BYTE OF KINDOBJ AND RETURN JUST THE
C       INTEGER TYPE OF OBJECT.
C
C     TYPOBJ=IAND(KINDOBJ(NID),'000377'O)
      TYPOBJ=IAND(KINDOBJ(NID),255)
      RETURN
      END
C
      FUNCTION LOCOBJ(JID)
C
      IMPLICIT NONE
	INTEGER JID
C
#include "UDLIST.FOR"
C
      LOCOBJ=OBJFRLOC(IDOBJ(JID))
C
      RETURN
      END
C     
      FUNCTION SIZEOBJ(JID)
C
      IMPLICIT NONE
	INTEGER JID
C
#include "UDLIST.FOR"
C
      SIZEOBJ=OBJFRSIZ(IDOBJ(JID))
C
      RETURN
      END
