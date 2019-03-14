#ifndef VALID_H
#define VALID_H
#include "Setting.h"
#include "Reader.h"
#include "Corrupt.h"

INT lastValidHead = 0;
INT lastValidTail = 0;
	
REAL l_valid_filter_tot = 0, l_valid_filter_rank = 0;
REAL r_valid_filter_tot = 0, r_valid_filter_rank = 0;


extern "C"
void validInit() {
    lastValidHead = 0;
    lastValidTail = 0;
    
    l_valid_filter_tot = 0;
    r_valid_filter_tot = 0;

    l_valid_filter_rank = 0;
    r_valid_filter_rank = 0;
}

extern "C"
void getValidHeadBatch(INT *ph, INT *pt, INT *pr, INT *pd) {
    for (INT i = 0; i < entityTotal; i++) {
	ph[i] = i;
	pt[i] = validList[lastValidHead].t;
	pr[i] = validList[lastValidHead].r;
    pd[i] = validList[lastValidHead].d;
    }
}

extern "C"
void getValidTailBatch(INT *ph, INT *pt, INT *pr, INT *pd) {
    for (INT i = 0; i < entityTotal; i++) {
	ph[i] = validList[lastValidTail].h;
	pt[i] = i;
	pr[i] = validList[lastValidTail].r;
    pd[i] = validList[lastValidTail].d;
    }
}

extern "C"
void validHead(REAL *con) {
    INT h = validList[lastValidHead].h;
    INT t = validList[lastValidHead].t;
    INT r = validList[lastValidHead].r;
    INT d = validList[lastValidHead].d;
    REAL minimal = con[h];
    INT l_filter_s = 0;
    for (INT j = 0; j < entityTotal; j++) {
    	if (j != h) {
    	    REAL value = con[j];
       	    if (value < minimal && ! _find(j, t, r, d)) {
    		  l_filter_s += 1;
    	    }
    	}
    }
    //hit@10
    if (l_filter_s < 10) l_valid_filter_tot += 1;
    // meanRank
    l_valid_filter_rank += (l_filter_s+1);

    lastValidHead ++;
    //printf("head: l_valid_filter_rank = %f\n", l_valid_filter_rank);
}

extern "C"
void validTail(REAL *con) {
    INT h = validList[lastValidTail].h;
    INT t = validList[lastValidTail].t;
    INT r = validList[lastValidTail].r;
    INT d = validList[lastValidTail].d;
    REAL minimal = con[t];
    INT r_filter_s = 0;
    for (INT j = 0; j < entityTotal; j++) {
	if (j != t) {
	    REAL value = con[j];
	    if (value < minimal && ! _find(h, j, r, d)) {
	        r_filter_s += 1;
	    }
	}
    }
    if (r_filter_s < 10) r_valid_filter_tot += 1;
    // meanRank
    r_valid_filter_rank += (1+r_filter_s);

    lastValidTail ++;

    //printf("tail: r_valid_filter_rank = %f\n", r_valid_filter_rank);
}

REAL validHit10 = 0;
extern "C"
REAL  getValidHit10() {
    l_valid_filter_tot /= validTotal;
    r_valid_filter_tot /= validTotal;
    validHit10 = (l_valid_filter_tot + r_valid_filter_tot) / 2;
    //printf("result: %f\n", validHit10);
    return validHit10;
}

REAL validMeanRank = 0;
extern "C"
REAL  getValidMeanRank() {
    l_valid_filter_rank /= validTotal;
    r_valid_filter_rank /= validTotal;
    validMeanRank = (l_valid_filter_rank + r_valid_filter_rank)/2;
    //printf("result: %f\n", validMeanRank);
    return validMeanRank;
}

#endif
