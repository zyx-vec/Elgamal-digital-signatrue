#include <stdio.h>
#include "bigint.h"
#include "prime.h"

FILE* fp;
int total = 0;
BigInt lambda, alpha, beta, d, r, s, p, p_minus1, t, x;
BigInt Ke, Ke_reverse;

// -1 neg, 1 pos. (a - b)
int signs(char *a, char *b, int s0, int s1) {
	// 
	if (s0 == -1) {
		if (s1 == -1) {	// -1, -1
			if ( compare(a, b) > 0 ) {
				return -1;
			} else {
				return 1;
			}
		} else { // -1, 1
			return -1;
		}
	} else {
		if (s1 == -1) { // 1, -1
			return 1;
		} else { // 1, 1
			if ( compare(a, b) < 0 ) {
				return -1;
			} else {
				return 1;
			}
		}
	}
}

void eea(BigInt a, BigInt b) {
	BigInt s0, s1;
	strcpy(s0.decimal, one);
	s0.len_d = 1;
	strcpy(s1.decimal, zero);
	s1.len_d = 1;
	BigInt t0, t1;
	t0 = s1; t1 = s0;

	BigInt r0, r1, r;
	r0 = a; r1 = b;

	QR tmp;
	BigInt mm0, mm1, s, t;
	
	int ss0, st0, ss1, st1;
	ss0 = st0 = ss1 = st1 = 1;
	int ii = 0;
	do {

		tmp = divd(r0.decimal, r1.decimal);

		mm0 = mul(s1.decimal, tmp.q);
		mm1 = mul(t1.decimal, tmp.q);

		//printf("%s\t%s\n", mm0.decimal, mm1.decimal);
		int stmp0, stmp1;
		stmp0 = ss1; stmp1 = st1;
		s = s1; t = t1;
		// -
		if ( signs(s0.decimal, mm0.decimal, ss0, ss1) == -1 ) {
			if ( ss0 == -1 ) {
				if ( ss1 == -1 ) {	
					s1 = sub(s0.decimal, mm0.decimal);	// - -
				} else {								
					s1 = add(s0.decimal, mm0.decimal);	// - +
				}
			} else { 
				s1 = sub(mm0.decimal, s0.decimal);		// + +
			}
			ss1 = -1;
		} else {// +
			if ( ss0 == -1 ) {
				s1 = sub(mm0.decimal, s0.decimal);		// - -
			} else {
				if ( ss1 == -1 ) {	
					s1 = add(s0.decimal, mm0.decimal);	// + -
				} else {
					s1 = sub(s0.decimal, mm0.decimal);	// + +
				}
			}
			ss1 = 1;
		}

		if ( signs(t0.decimal, mm1.decimal, st0, st1) == -1 ) {
			if ( st0 == -1 ) {
				if ( st1 == -1 ) {	
					t1 = sub(t0.decimal, mm1.decimal);	// - -
				} else {								
					t1 = add(t0.decimal, mm1.decimal);	// - +
				}
			} else { 
				t1 = sub(mm1.decimal, t0.decimal);		// + +
			}
			st1 = -1;
		} else {// +
			if ( st0 == -1 ) {
				t1 = sub(mm1.decimal, t0.decimal);		// - -
			} else {
				if ( st1 == -1 ) {	
					t1 = add(t0.decimal, mm1.decimal);	// + -
				} else {
					t1 = sub(t0.decimal, mm1.decimal);	// + +
				}
			}
			st1 = 1;
		}
		ss0 = stmp0; st0 = stmp1;
		s0 = s; t0 = t;

		r0 = r1;
		strcpy(r1.decimal, tmp.r);
		r1.len_d = strlen(r1.decimal);
		ii++;

	} while( compare( r1.decimal, zero ) != 0 );

	printf(" %d\t%d\n%s\t%s\n",  ss0, st0, s0.decimal, t0.decimal);

	if ( st0 == -1 ) {
		t0 = sub(p_minus1.decimal, t0.decimal);
	}
	strcpy(Ke_reverse.decimal, t0.decimal);
	Ke_reverse.len_d = t0.len_d;
	//printf("%d\n", ii);
}

void init() {
	fp = fopen("out.txt", "w");
	char an[] = "1508328194402833847794054351881706467721190883564783621081033257";
	char an1[] = "0508328194402833847794054351881706467721190883564783621081033257";
	strcpy(p.decimal, an);
	p.len_d = strlen(p.decimal);
	strcpy(p_minus1.decimal, an1);
	p_minus1.len_d = strlen(p_minus1.decimal);

	char k[] = "7312";	// 32532587312
	strcpy(Ke.decimal, k);
	Ke.len_d = strlen(Ke.decimal);

	BigInt tmp = gcd(p_minus1, Ke);
	printf("%s\n", tmp.decimal);

	strcpy(lambda.decimal, two);
	lambda.len_d = 1;

	r = modular_exp_(lambda, Ke, p);
	printf("%s\n%s\n%s\n", lambda.decimal, r.decimal, p.decimal);
	eea(p_minus1, Ke);
	printf("%s\n", Ke_reverse.decimal);

	
	char dd[] = "3";
	strcpy(d.decimal, dd);
	d.len_d = 1;

	BigInt dr = mul(d.decimal, r.decimal);
	printf("dr = %s\n", dr.decimal);

	BigInt ts, tmp1;
	if ( compare(x.decimal, dr.decimal) < 0 ) {

		QR tmp = divd(dr.decimal, p_minus1.decimal);
		BigInt q;
		strcpy(q.decimal, tmp.q);
		q.len_d = strlen(q.decimal);
		printf("q = %s\n", q.decimal);
		q = add(q.decimal, one);

		q = mul(q.decimal, p_minus1.decimal);
		x = add(x.decimal, q.decimal);

		printf("%s\n%s\n", x.decimal, dr.decimal);
	}
	tmp1 = sub(dr.decimal, x.decimal);
	ts = mul(tmp1.decimal, Ke_reverse.decimal);

	QR rr = divd(ts.decimal, p_minus1.decimal);

	strcpy(s.decimal, rr.r);
	s.len_d = strlen(s.decimal);

	printf("%s\n", rr.r);

	strcpy(alpha.decimal, two);
	alpha.len_d = 1;
	beta = modular_exp_(alpha, d, p);
}

void verfy() {
	BigInt lhs, rhs;
	lhs = modular_exp_(beta, r, p);
	rhs = modular_exp_(r, s, p);

	BigInt tmp = mul(lhs.decimal, rhs.decimal);
	QR tmp_ = divd(tmp.decimal, p.decimal);

	strcpy(t.decimal, tmp_.r);
	t.len_d = strlen(t.decimal);
}

int main() {
	char hashVal[] = "7234234654321";
	strcpy(x.decimal, hashVal);
	x.len_d = strlen(x.decimal);


	init();

	printf("public key (p, alpha, beta):\np := %s\nalpha := %s\nbeta := %s\n", \
				p.decimal, alpha.decimal, beta.decimal);
	
	printf("x := %s\nr := %s\ns := %s\n",\
				 x.decimal, r.decimal, s.decimal);

	verfy();

	printf("t := %s\n", t.decimal);

	BigInt cal = modular_exp_(alpha, x, p);
	printf("? := %s\n", cal.decimal);
	
	return 0;
}

