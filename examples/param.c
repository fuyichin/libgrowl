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

/**
 * retrieve mode eg. --console, from parameter
 * return TRUE or FALSE
 */
int get_param_mode(char *args[], int argc, char *mode) {
	int result= FALSE;
	int i;

	for (i=0; i<argc; i++) {
		if (strcmp(args[i],mode)==0) {
			result= TRUE;
			/* if extra value after mode name, check is 0|off */
			if (i+1<argc)
				if (strcmp(args[i+1],"0")==0 || strcmp(args[i+1],"off")==0) {
					result= FALSE;
					break;
				}
		}
	}
	return result;
}
