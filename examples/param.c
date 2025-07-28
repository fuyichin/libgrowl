#include<stdio.h>
#include<string.h>
#include "param.h"

/*
default is 0
-v output info
-vv debug
*/
int get_param_verbose_level(char *args[], int argc) {
	int i;
	int verbose= VERBOSE_ERROR; /* only output error */
	for (i=0; i<argc; i++) {
		if (strlen(args[i])<2)
			continue;
		if (strcmp(args[i],"-v")==0) {
			verbose= VERBOSE_OUTUT;
			break;
		}
		else if (strcmp(args[i],"-vv")==0) {
			verbose= VERBOSE_DEBUG;
			break;
		}
	}
	return verbose;
}

