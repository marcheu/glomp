
#define BUG() \
	do{\
	printf("error at %s %s\n",__LINE__,__FUNCTION__);\
	exit(0);\
	}while(0)

