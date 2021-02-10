#include <stdio.h>
int is_special(int s, int m, int d) { return s < m && m > d; }
int main()
{
	int n, k = 0;
	int s, m, d;
	int S = 0, xmax_impar = -1, xmin_par = 100000000;
	scanf("%d", &n);
	if (!n) {
		printf("%d\n%.7f\n%d\n%d\n", 0, 0, 0, 0);
		return 0;
	}
	if (n == 1) {
		scanf("%d", &n);
		printf("%d\n%.7f\n%d\n%d\n", 0, 0, 0, 0);
		return 0;
	}
	scanf("%d %d", &s, &m);
	if (n == 2) {
		printf("%d\n%.7f\n%d\n%d\n", 0, 0, 0, 0);
		return 0;
	}
	for (int i = 2; i < n; i++) {
		scanf("%d", &d);
		if (is_special(s, m, d)) {
			k++;
			S += m;
			if ((i - 1) & 1) { // par
				if (m > xmax_impar)
					xmax_impar = m;
			} else { // import
				if (m < xmin_par)
					xmin_par = m;
			}
		}
		s = m;
		m = d;
	}
	double ma = (double)S / k;
	if (S == 0)
		ma = 0;
	printf("%d\n%.7f\n%d\n%d\n", S, ma, xmax_impar, xmin_par);
	return 0;
}
