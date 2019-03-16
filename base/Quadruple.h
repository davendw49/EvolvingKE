#ifndef QUADRUPLE_H
#define QUADRUPLE_H
#include "Setting.h"

struct Quadruple {

	// add int d; for calculate time
	INT h, r, t, d;

	static INT minimal(INT a,INT b) {
		if (a > b) return b;
		return a;
	}
	
	static bool cmp_list(const Quadruple &a, const Quadruple &b) {
		return (minimal(a.h, a.t) > minimal(b.h, b.t));
	}

	static bool cmp_head(const Quadruple &a, const Quadruple &b) {
		return (a.h < b.h)||(a.h == b.h && a.r < b.r)||(a.h == b.h && a.r == b.r && a.t < b.t);
	}

	static bool cmp_tail(const Quadruple &a, const Quadruple &b) {
		return (a.t < b.t)||(a.t == b.t && a.r < b.r)||(a.t == b.t && a.r == b.r && a.h < b.h);
	}

	static bool cmp_rel(const Quadruple &a, const Quadruple &b) {
		return (a.h < b.h)||(a.h == b.h && a.t < b.t)||(a.h == b.h && a.t == b.t && a.r < b.r);
	}

	static bool cmp_rel2(const Quadruple &a, const Quadruple &b) {
		return (a.r < b.r)||(a.r == b.r && a.h < b.h)||(a.r == b.r && a.h == b.h && a.t < b.t);
	}

};

#endif
