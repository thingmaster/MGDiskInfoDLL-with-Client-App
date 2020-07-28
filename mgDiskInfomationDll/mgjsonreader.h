#pragma once

//#include <stdio.h>
class mgjsonscanner {
	//constructor 
#define TAGWID 128
	char mgjsondataset[100][128];
	struct mgjstruct {
		char jdatatag[TAGWID];
		int jdataint;
		float jdatafloat;
		char* jdatastr;
	} mgjsonvalue[100];
	int mglevelcounter, mgcurrentlevel;
	int mgseqcounter[10] = { 0 };
public:
	mgjsonscanner() {
		mglevelcounter = 0;
		mgcurrentlevel = 0 ;
	}

public:
	int mgjsonscanline(char* pbuff) {
		if (mgcurrentlevel > mglevelcounter) {
			mglevelcounter++;
		}
		char * n = strstr(pbuff, "{");
		if (n) {
			printf("   +++++ %d (%d)", ++mgcurrentlevel, mglevelcounter);
			mgseqcounter[mgcurrentlevel]++;
			return TRUE;
		}
		n = strstr(pbuff, "}");
		if (n ) {
			printf("   ------ %d (%d)", --mgcurrentlevel , mglevelcounter);
			return TRUE;
		}
		printf(" %s : %d=%d ", pbuff, mgcurrentlevel,mgseqcounter[mgcurrentlevel]);
		return FALSE;
	}
};