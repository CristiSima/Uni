// Simache Cristian Ioan 314CA
#include <stdio.h>
#define NMAX 101

int min_FV(int v[])
{//genereaza cel mai mic numar format din cifrele numarului
	int n = 0;
	for (int i = 0; i <= 9; i++)
		for (int j = 0; j < v[i]; j++)
			n = n * 10 + i;
	return n;
}

int max_FV(int v[])
{//genereaza cel mai mare numar format din cifrele numarului
	int n = 0;
	for (int i = 9; i >= 0; i--)
		for (int j = 0; j < v[i]; j++)
			n = n * 10 + i;
	return n;
}

void build_V(int nr, int v[])
{//genereaza vectorul cu numarul de apariti al numarului
	for (int i = 0; i <= 9; i++)
		v[i] = 0;
	while (nr) {
		v[nr % 10]++;
		nr /= 10;
	}
}

int in_H(int nr, int H[], int h)
{//verifica daca nr e in vector si intoarce pozitia
	for (int i = 0; i < h; i++)
		if (nr == H[i])
			return i;
	return -1;
}

int main(void)
{
	int H[NMAX], h = 0;// vector cu numerele precedente si limita lui
	int F[10];
	int n;
	scanf("%d", &n);

	while (in_H(n, H, h) == -1) {
		H[h] = n;
		h++;
		build_V(n, F);
		n = max_FV(F) - min_FV(F);
	}

	//check edgecase
	if (h == 1) {
		printf("0\n%d", H[0]);
		return 0;
	}
	printf("%d\n", in_H(n, H, h) - 1);
	for (int i = in_H(n, H, h); i < h; i++)
		printf("%d ", H[i]);
	printf("\n");
	return 0;
}
