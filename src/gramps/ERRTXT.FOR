C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
C
	Integer j
      CHARACTER*50 ERRTXT(LASTERROR)
      DATA (ERRTXT(J),J=1,10)/
     &      'I can''t find that COMMAND name',
     &      'I can''t find that object name',
     &      'I don''t recognize that KEYWORD',
     &      'I can''t make sense of that DEVICE',
     &      'I can''t open that file; non-existant or protected',
     &      'That NAME is already being used',
     &      'I need more information for that COMMAND',
     &      'I don''t know how to group things in that way',
     &      'I can''t make sense out of that NUMBER',
     &      'I can''t recognize that DEVICE, VALUE or FUNCTION'/

      DATA (ERRTXT(J),J=11,20)/
     &      'You need to give a NAME here',
     &      'Collect is already in effect; only one is allowed',
     &      'Sorry; that would nest groups too deeply',
     &      'I don''t know how to insert things in that way',
     &      'Perspectives or moves apply only to the WORLDs',
     &      'That would create too many objects',
     &      'There''s not enough host memory to do that',
     &      'You''ve nested DO''s too deeply',
     &      'That object is not in the tree',
     &      'I don''t understand that $SYMBOL or $PARAMETER'/
      DATA (ERRTXT(J),J=21,30)/
     &      'That''s an unreasonable time value',
     &      'I can''t make sense of those boundaries',
     &      'I can''t make sense of that directory name',
     &      'I can''t find the table entry for that object',
     &      'That number is too big or too small',
     &      'There''s too much data; buffering is now off',
     &      'PS memory full; blank something and ''set display''',
     &      'That file has no data in it',
     &      'An unforseen catastrophy has occurred; bye',
     &      'There''s something funny in that picture file'/
      DATA (ERRTXT(J),J=31,40)/
     &      'That command does''nt understand arguments',
     &      'That is too much data to display in one object',
     &      'Error opening gin file',
     &      'That text line is too long to center',
     &      'Axis endpoints cannot be the same',
     &      'I don''t recognize that coordinate #',
     &      'Please consult GRAMPS documentation',
     &      'That argument is too long',
     &      'That is not allowed for the WORLD',
     &      'I don''t recognize that /SWITCH'/
      DATA (ERRTXT(J),J=41,50)/
     &      'That command is not valid within a GIN file',
     &      'That command is only valid within a GIN file',
     &      'Only ordinary objects may be on the menu',
     &      'That item is not on the menu',
     &      'Please finish drawing first',
     &      'I can''t make sense out of that picture''s data',
     &      'PUT can only handle ordinary objects',
     &      'That object isn''t deep enough to loop that much',
     &      'Only one display OUTPUT file at a time, please',
     &      'Please use SET OUTPUT to specify a device or file'/
      DATA (ERRTXT(J),J=51,60)/
     &      'Sorry, that feature is not working in this version',
     &      'I don''t recognize that OUTPUT device or file type',
     &      'Axis endpoints cannot have W=0',
     &      'Please SET OUTPUT device first',
     &      'Synonyms nested too deeply or recursively',
     &      'Please finish picking',
     &      'I can''t decide which object was PICKed',
     &      'I don''t recognize that font number',
     &      'That would create too many triangles',
     &      'Environment variable $gramps_lib is not defined'/
      DATA (ERRTXT(J),J=61,LASTERROR)/
     &      'Rayt uses normals with triangles, not polygons',
     &      'System commands not allowed over socket',
     &      'I can''t open a socket to accept picture data',
     &      'Unused error message',
     &      'Please wait for ADVANCE to finish',
     &      'Some unspecified error has occured!'/
