
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef TESTMASTER_H
#define TESTMASTER_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 TestMaster_valueRangeTest (UNS8 typeValue, void * value);
const indextable * TestMaster_scanIndexOD (CO_Data *d, UNS16 wIndex, UNS32 * errorCode);

/* Master node data struct */
extern CO_Data TestMaster_Data;
extern UNS16 Slave0x2000;		/* Mapped at index 0x2000, subindex 0x00*/
extern INTEGER32 Slave0x2001;		/* Mapped at index 0x2001, subindex 0x00*/
extern UNS16 Slave0x2002;		/* Mapped at index 0x2002, subindex 0x00*/
extern UNS16 Slave0x2003;		/* Mapped at index 0x2003, subindex 0x00*/
extern UNS16 Slave0x2004;		/* Mapped at index 0x2004, subindex 0x00*/
extern UNS16 Slave0x2005;		/* Mapped at index 0x2005, subindex 0x00*/
extern UNS16 Slave0x2006;		/* Mapped at index 0x2006, subindex 0x00*/
extern INTEGER16 Slave0x2007;		/* Mapped at index 0x2007, subindex 0x00*/
extern INTEGER32 Slave0x2008;		/* Mapped at index 0x2008, subindex 0x00*/
extern UNS16 Slave0x2009;		/* Mapped at index 0x2009, subindex 0x00*/
extern UNS16 Slave0x200A;		/* Mapped at index 0x200A, subindex 0x00*/
extern UNS16 Slave0x200B;		/* Mapped at index 0x200B, subindex 0x00*/
extern UNS8 Slave0x200C;		/* Mapped at index 0x200C, subindex 0x00*/
extern UNS8 Slave0x200D;		/* Mapped at index 0x200D, subindex 0x00*/
extern UNS8 Slave0x200E;		/* Mapped at index 0x200E, subindex 0x00*/
extern UNS8 Slave0x200F;		/* Mapped at index 0x200F, subindex 0x00*/
extern UNS8 Slave0x2010;		/* Mapped at index 0x2010, subindex 0x00*/
extern UNS8 Slave0x2011;		/* Mapped at index 0x2011, subindex 0x00*/
extern UNS8 Slave0x2012;		/* Mapped at index 0x2012, subindex 0x00*/

#endif // TESTMASTER_H
