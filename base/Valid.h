#ifndef VALID_H
#define VALID_H
#include "Setting.h"
#include "Reader.h"
#include "Corrupt.h"

INT lastValidHead = 0;
INT lastValidTail = 0;
	
REAL l_valid_filter_tot = 0, l_valid_filter_valid_rank = 0, l_valid_rank = 0, l_valid_filter_reci_rank = 0, l_valid_reci_rank = 0;
REAL r_valid_filter_tot = 0, r_valid_filter_valid_rank = 0, r_valid_rank = 0, r_valid_filter_reci_rank = 0, r_valid_reci_rank = 0;


extern "C"
void validInit() {
    lastValidHead = 0;
    lastValidTail = 0;
    
    l_valid_filter_tot = 0;
    r_valid_filter_tot = 0;

    l_valid_filter_valid_rank = 0, l_valid_rank = 0, l_valid_filter_reci_rank = 0, l_valid_reci_rank = 0;
    r_valid_filter_valid_rank = 0, r_valid_rank = 0, r_valid_filter_reci_rank = 0, r_valid_reci_rank = 0;
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
    pd[i] = validList[lastValidHead].d;
    }
}

extern "C"
void validHead0(REAL *con) {
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
    if (l_filter_s < 10) l_valid_filter_tot += 1;
    lastValidHead ++;
  //  printf("head: l_valid_filter_tot = %f | l_filter_hit10 = %f\n", l_valid_filter_tot, l_valid_filter_tot / lastValidHead);
}

extern "C"
void validTail0(REAL *con) {
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
    lastValidTail ++;
//    printf("tail: r_valid_filter_tot = %f | r_filter_hit10 = %f\n", r_valid_filter_tot, r_valid_filter_tot / lastValidTail);
}

extern "C"
void validHead(REAL *con) {
    INT h = validList[lastValidHead].h;
    INT t = validList[lastValidHead].t;
    INT r = validList[lastValidHead].r;
    // time
    INT d = validList[lastValidHead].d;

    INT lef = head_lef[r], rig = head_rig[r];

    REAL minimal = con[h];
    INT l_s = 0;
    INT l_filter_s = 0;

    for (INT j = 0; j < entityTotal; j++) {
        if (j != h) {
            REAL value = con[j];
            if (value < minimal) {
                l_s += 1;
                if (not _find(j, t, r, d))
                    l_filter_s += 1;
            }
        }
    }

    //hit@10
    if (l_filter_s < 10) l_valid_filter_tot += 1;
    if (l_s < 10) l_tot += 1;

    // meanRank
    l_valid_filter_valid_rank += (l_filter_s+1);
    l_valid_rank += (1+l_s);
    l_valid_filter_reci_rank += 1.0/(l_filter_s+1);
    l_valid_reci_rank += 1.0/(l_s+1);

    lastValidHead++;

    //printf("l_filter_s: %ld\n", l_filter_s);
    //printf("%f %f %f %f \n", l_tot / lastValidHead, l_filter_tot / lastValidHead, l_valid_rank / lastValidHead, l_valid_filter_valid_rank / lastValidHead);
}

extern "C"
void validTail(REAL *con) {
    INT h = validList[lastValidTail].h;
    INT t = validList[lastValidTail].t;
    INT r = validList[lastValidTail].r;
    // time
    INT d = validList[lastValidTail].d;

    INT lef = tail_lef[r], rig = tail_rig[r];
    REAL minimal = con[t];
    INT r_s = 0;
    INT r_filter_s = 0;
    INT r_s_constrain = 0;
    INT r_filter_s_constrain = 0;
    for (INT j = 0; j < entityTotal; j++) {
        if (j != t) {
            REAL value = con[j];
            if (value < minimal) {
                r_s += 1;
                if (not _find(h, j, r, d))
                    r_filter_s += 1;
            }
        }
        
    }

    // hit@10
    if (r_filter_s < 10) r_valid_filter_tot += 1;
    if (r_s < 10) r_tot += 1;

    // meanRank
    r_valid_filter_valid_rank += (1+r_filter_s);
    r_valid_rank += (1+r_s);
    r_valid_filter_reci_rank += 1.0/(1+r_filter_s);
    r_valid_reci_rank += 1.0/(1+r_s);

    lastValidTail++;
    //printf("r_filter_s: %ld\n", r_filter_s);
    //printf("%f %f %f %f\n", r_tot /lastValidTail, r_filter_tot /lastValidTail, r_valid_rank /lastValidTail, r_valid_filter_valid_rank /lastValidTail);
}

REAL validHit10 = 0;
extern "C"
REAL  getValidHit10() {
    l_valid_filter_tot /= validTotal;
    r_valid_filter_tot /= validTotal;
    validHit10 = (l_valid_filter_tot + r_valid_filter_tot) / 2;
   // printf("result: %f\n", validHit10);
    return validHit10;
}

REAL validMeanRank = 0;
extern "C"
REAL  getValidMeanRank() {
    l_valid_reci_rank /= validTotal;
    r_valid_reci_rank /= validTotal;
    validMeanRank = (l_valid_reci_rank + r_valid_reci_rank)/2;
   // printf("result: %f\n", validHit10);
    return validMeanRank;
}

#endif
