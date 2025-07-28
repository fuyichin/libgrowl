#include <stdio.h>
//#include <string.h>
#include "param.h"

static int g_verbose_level= 0;

void output_message(int level, char *text) {
	if (level<=g_verbose_level)
		printf("%s", text);
}

int main(int argc, char *argv[]) {
	g_verbose_level= get_param_verbose_level(argv, argc);

#ifdef _DEBUG
	printf("verbose level=%d\n", g_verbose_level);
#endif
	perror("This is error!");
	fprintf(stderr, "output to standard error\n");
//	printf("normal output\n");
	output_message(VERBOSE_OUTUT, "normal output\n");

//	printf("debug message\n");
	output_message(VERBOSE_DEBUG, "debug message\n");

#ifdef _DEBUG
	output_message(VERBOSE_OUTUT, "_DEBUG is enable\n");
#endif
	return 0;
}