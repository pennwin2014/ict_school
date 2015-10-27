#ifndef __NCSRVDEF___
#define __NCSRVDEF___

#include "ncportal.h"

typedef struct{
	     long aid;           /* Ö÷¼ü */
			 char acname[32];    /* ACÃû³Æ */
			 char acip[20];      /* ACIP */
			 char acport[8];     /* AC¶Ë¿Ú */
}acinfo_s ;

typedef struct {
    	char mac[20];
    	char ip[20];
    	char broadip[20];	
    	char netmask[20];	
}netdev_s;




#endif
