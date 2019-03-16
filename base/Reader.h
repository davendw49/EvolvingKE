#ifndef READER_H
#define READER_H
#include "Setting.h"
#include "Quadruple.h"
#include <cstdlib>
#include <algorithm>

//INT daylong(INT date_e, INT date_s, INT y);

INT *freqRel, *freqEnt, *freqDat;
INT *lefHead, *rigHead;
INT *lefTail, *rigTail;
INT *lefRel, *rigRel;
REAL *left_mean, *right_mean;

Quadruple *trainList;
Quadruple *trainHead;
Quadruple *trainTail;
Quadruple *trainRel;

INT *testLef, *testRig;
INT *validLef, *validRig;

extern "C"
void importTrainFiles() {

	printf("The toolkit is importing datasets.\n");
	FILE *fin;
	int tmp;

	fin = fopen((inPath + "relation2id.txt").c_str(), "r");
	tmp = fscanf(fin, "%ld", &relationTotal);
	printf("The total of relations is %ld.\n", relationTotal);
	fclose(fin);

	fin = fopen((inPath + "entity2id.txt").c_str(), "r");
	tmp = fscanf(fin, "%ld", &entityTotal);
	printf("The total of entities is %ld.\n", entityTotal);
	fclose(fin);

    fin = fopen((inPath + "date2id.txt").c_str(), "r");
    tmp = fscanf(fin, "%ld", &dateTotal);
    printf("The total of date is %ld.\n", dateTotal);
    fclose(fin);

	fin = fopen((inPath + "train2id.txt").c_str(), "r");
	tmp = fscanf(fin, "%ld", &trainTotal);

	trainList = (Quadruple *)calloc(trainTotal, sizeof(Quadruple));
	trainHead = (Quadruple *)calloc(trainTotal, sizeof(Quadruple));
	trainTail = (Quadruple *)calloc(trainTotal, sizeof(Quadruple));
	trainRel = (Quadruple *)calloc(trainTotal, sizeof(Quadruple));
	
    freqRel = (INT *)calloc(relationTotal, sizeof(INT));
	freqEnt = (INT *)calloc(entityTotal, sizeof(INT));
    // time
    freqDat = (INT *)calloc(dateTotal, sizeof(INT));

	for (INT i = 0; i < trainTotal; i++) {
		tmp = fscanf(fin, "%ld", &trainList[i].h);
		tmp = fscanf(fin, "%ld", &trainList[i].t);
		tmp = fscanf(fin, "%ld", &trainList[i].r);
        tmp = fscanf(fin, "%ld", &trainList[i].d);

        //trainList[i].d = daylong(trainList[i].d,20160101,2016);
	}
	fclose(fin);
	std::sort(trainList, trainList + trainTotal, Quadruple::cmp_head);

	tmp = trainTotal; trainTotal = 1;
	
    trainHead[0] = trainTail[0] = trainRel[0] = trainList[0];
	
    freqEnt[trainList[0].t] += 1;
	freqEnt[trainList[0].h] += 1;
	freqRel[trainList[0].r] += 1;
    freqDat[trainList[0].d] += 1;
	
    for (INT i = 1; i < tmp; i++)
		if (trainList[i].h != trainList[i - 1].h || trainList[i].r != trainList[i - 1].r || trainList[i].t != trainList[i - 1].t) {
			trainHead[trainTotal] = trainTail[trainTotal] = trainRel[trainTotal] = trainList[trainTotal] = trainList[i];
			trainTotal++;
			freqEnt[trainList[i].t]++;
			freqEnt[trainList[i].h]++;
			freqRel[trainList[i].r]++;
            freqDat[trainList[i].d]++;
		}

	std::sort(trainHead, trainHead + trainTotal, Quadruple::cmp_head);
	std::sort(trainTail, trainTail + trainTotal, Quadruple::cmp_tail);
	std::sort(trainRel, trainRel + trainTotal, Quadruple::cmp_rel);
	printf("The total of train quadruples is %ld.\n", trainTotal);

	lefHead = (INT *)calloc(entityTotal, sizeof(INT));
	rigHead = (INT *)calloc(entityTotal, sizeof(INT));
	lefTail = (INT *)calloc(entityTotal, sizeof(INT));
	rigTail = (INT *)calloc(entityTotal, sizeof(INT));
	lefRel = (INT *)calloc(entityTotal, sizeof(INT));
	rigRel = (INT *)calloc(entityTotal, sizeof(INT));

	memset(rigHead, -1, sizeof(INT)*entityTotal);
	memset(rigTail, -1, sizeof(INT)*entityTotal);
	memset(rigRel, -1, sizeof(INT)*entityTotal);
	
    for (INT i = 1; i < trainTotal; i++) {
		if (trainTail[i].t != trainTail[i - 1].t) {
			rigTail[trainTail[i - 1].t] = i - 1;
			lefTail[trainTail[i].t] = i;
		}
		if (trainHead[i].h != trainHead[i - 1].h) {
			rigHead[trainHead[i - 1].h] = i - 1;
			lefHead[trainHead[i].h] = i;
		}
		if (trainRel[i].h != trainRel[i - 1].h) {
			rigRel[trainRel[i - 1].h] = i - 1;
			lefRel[trainRel[i].h] = i;
		}
	}
	lefHead[trainHead[0].h] = 0;
	rigHead[trainHead[trainTotal - 1].h] = trainTotal - 1;
	lefTail[trainTail[0].t] = 0;
	rigTail[trainTail[trainTotal - 1].t] = trainTotal - 1;
	lefRel[trainRel[0].h] = 0;
	rigRel[trainRel[trainTotal - 1].h] = trainTotal - 1;

	left_mean = (REAL *)calloc(relationTotal,sizeof(REAL));
	right_mean = (REAL *)calloc(relationTotal,sizeof(REAL));
	for (INT i = 0; i < entityTotal; i++) {
		for (INT j = lefHead[i] + 1; j <= rigHead[i]; j++)
			if (trainHead[j].r != trainHead[j - 1].r)
				left_mean[trainHead[j].r] += 1.0;
		if (lefHead[i] <= rigHead[i])
			left_mean[trainHead[lefHead[i]].r] += 1.0;
		for (INT j = lefTail[i] + 1; j <= rigTail[i]; j++)
			if (trainTail[j].r != trainTail[j - 1].r)
				right_mean[trainTail[j].r] += 1.0;
		if (lefTail[i] <= rigTail[i])
			right_mean[trainTail[lefTail[i]].r] += 1.0;
	}
	for (INT i = 0; i < relationTotal; i++) {
		left_mean[i] = freqRel[i] / left_mean[i];
		right_mean[i] = freqRel[i] / right_mean[i];
	}
}

Quadruple *testList;
Quadruple *validList;
Quadruple *quadrupleList;

extern "C"
void importTestFiles() {
    FILE *fin;
    INT tmp;
    
    fin = fopen((inPath + "relation2id.txt").c_str(), "r");
    tmp = fscanf(fin, "%ld", &relationTotal);
    fclose(fin);

    fin = fopen((inPath + "entity2id.txt").c_str(), "r");
    tmp = fscanf(fin, "%ld", &entityTotal);
    fclose(fin);

    fin = fopen((inPath + "date2id.txt").c_str(), "r");
    tmp = fscanf(fin, "%ld", &dateTotal);
    fclose(fin);

    FILE* f_kb1 = fopen((inPath + "test2id.txt").c_str(), "r");
    FILE* f_kb2 = fopen((inPath + "train2id.txt").c_str(), "r");
    FILE* f_kb3 = fopen((inPath + "valid2id.txt").c_str(), "r");

    tmp = fscanf(f_kb1, "%ld", &testTotal);
    tmp = fscanf(f_kb2, "%ld", &trainTotal);
    tmp = fscanf(f_kb3, "%ld", &validTotal);

    quadrupleTotal = testTotal + trainTotal + validTotal;

    testList = (Quadruple *)calloc(testTotal, sizeof(Quadruple));
    validList = (Quadruple *)calloc(validTotal, sizeof(Quadruple));
    quadrupleList = (Quadruple *)calloc(quadrupleTotal, sizeof(Quadruple));

    for (INT i = 0; i < testTotal; i++) {
        tmp = fscanf(f_kb1, "%ld", &testList[i].h);
        tmp = fscanf(f_kb1, "%ld", &testList[i].t);
        tmp = fscanf(f_kb1, "%ld", &testList[i].r);
        tmp = fscanf(f_kb1, "%ld", &testList[i].d);
        //testList[i].d = daylong(testList[i].d,20160101,2016);

        quadrupleList[i] = testList[i];
    }
    for (INT i = 0; i < trainTotal; i++) {
        tmp = fscanf(f_kb2, "%ld", &quadrupleList[i + testTotal].h);
        tmp = fscanf(f_kb2, "%ld", &quadrupleList[i + testTotal].t);
        tmp = fscanf(f_kb2, "%ld", &quadrupleList[i + testTotal].r);
        tmp = fscanf(f_kb2, "%ld", &quadrupleList[i + testTotal].d);

        //quadrupleList[i + testTotal].d = daylong(&quadrupleList[i + testTotal].d,20160101,2016);
    }
    for (INT i = 0; i < validTotal; i++) {
        tmp = fscanf(f_kb3, "%ld", &quadrupleList[i + testTotal + trainTotal].h);
        tmp = fscanf(f_kb3, "%ld", &quadrupleList[i + testTotal + trainTotal].t);
        tmp = fscanf(f_kb3, "%ld", &quadrupleList[i + testTotal + trainTotal].r);
        tmp = fscanf(f_kb3, "%ld", &quadrupleList[i + testTotal + trainTotal].d);

        //quadrupleList[i + testTotal + trainTotal].d = daylong(quadrupleList[i + testTotal + trainTotal].d,20160101,2016);

        validList[i] = quadrupleList[i + testTotal + trainTotal];
    }
    fclose(f_kb1);
    fclose(f_kb2);
    fclose(f_kb3);

    std::sort(quadrupleList, quadrupleList + quadrupleTotal, Quadruple::cmp_head);
    std::sort(testList, testList + testTotal, Quadruple::cmp_rel2);
    std::sort(validList, validList + validTotal, Quadruple::cmp_rel2);

    printf("The total of test quadruples is %ld.\n", testTotal);
    printf("The total of valid quadruples is %ld.\n", validTotal);

    testLef = (INT *)calloc(relationTotal, sizeof(INT));
    testRig = (INT *)calloc(relationTotal, sizeof(INT));

    memset(testLef, -1, sizeof(INT) * relationTotal);
    memset(testRig, -1, sizeof(INT) * relationTotal);

    for (INT i = 1; i < testTotal; i++) {
	if (testList[i].r != testList[i-1].r) {
	    testRig[testList[i-1].r] = i - 1;
	    testLef[testList[i].r] = i;
	}
    }
    testLef[testList[0].r] = 0;
    testRig[testList[testTotal - 1].r] = testTotal - 1;

    validLef = (INT *)calloc(relationTotal, sizeof(INT));
    validRig = (INT *)calloc(relationTotal, sizeof(INT));
    memset(validLef, -1, sizeof(INT)*relationTotal);
    memset(validRig, -1, sizeof(INT)*relationTotal);
    for (INT i = 1; i < validTotal; i++) {
	if (validList[i].r != validList[i-1].r) {
	    validRig[validList[i-1].r] = i - 1;
	    validLef[validList[i].r] = i;
	}
    }
    validLef[validList[0].r] = 0;
    validRig[validList[validTotal - 1].r] = validTotal - 1;
}

INT* head_lef;
INT* head_rig;
INT* tail_lef;
INT* tail_rig;
INT* head_type;
INT* tail_type;

extern "C"
void importTypeFiles() {

    head_lef = (INT *)calloc(relationTotal, sizeof(INT));
    head_rig = (INT *)calloc(relationTotal, sizeof(INT));
    tail_lef = (INT *)calloc(relationTotal, sizeof(INT));
    tail_rig = (INT *)calloc(relationTotal, sizeof(INT));
    INT total_lef = 0;
    INT total_rig = 0;
    FILE* f_type = fopen((inPath + "type_constrain.txt").c_str(),"r");
    INT tmp;
    tmp = fscanf(f_type, "%ld", &tmp);
    for (INT i = 0; i < relationTotal; i++) {
        INT rel, tot;
        tmp = fscanf(f_type, "%ld %ld", &rel, &tot);
        for (INT j = 0; j < tot; j++) {
            tmp = fscanf(f_type, "%ld", &tmp);
            total_lef++;
        }
        tmp = fscanf(f_type, "%ld%ld", &rel, &tot);
        for (INT j = 0; j < tot; j++) {
            tmp = fscanf(f_type, "%ld", &tmp);
            total_rig++;
        }
    }
    fclose(f_type);
    head_type = (INT *)calloc(total_lef, sizeof(INT)); 
    tail_type = (INT *)calloc(total_rig, sizeof(INT));
    total_lef = 0;
    total_rig = 0;
    f_type = fopen((inPath + "type_constrain.txt").c_str(),"r");
    tmp = fscanf(f_type, "%ld", &tmp);
    for (INT i = 0; i < relationTotal; i++) {
        INT rel, tot;
        tmp = fscanf(f_type, "%ld%ld", &rel, &tot);
        head_lef[rel] = total_lef;
        for (INT j = 0; j < tot; j++) {
            tmp = fscanf(f_type, "%ld", &head_type[total_lef]);
            total_lef++;
        }
        head_rig[rel] = total_lef;
        std::sort(head_type + head_lef[rel], head_type + head_rig[rel]);
        tmp = fscanf(f_type, "%ld%ld", &rel, &tot);
        tail_lef[rel] = total_rig;
        for (INT j = 0; j < tot; j++) {
            tmp = fscanf(f_type, "%ld", &tail_type[total_rig]);
            total_rig++;
        }
        tail_rig[rel] = total_rig;
        std::sort(tail_type + tail_lef[rel], tail_type + tail_rig[rel]);
    }
    fclose(f_type);
}

#endif
