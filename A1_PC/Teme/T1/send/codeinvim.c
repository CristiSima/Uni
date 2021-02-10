// Simache Cristian Ioan 314CA
#include <stdio.h>
#define NMAX 101

int main(void)
{
	int n;
	int V[NMAX];
	int temp;
	scanf("%d", &n);
	// citirea notelor in V
	for (int i = 0; i < n; i++)
		scanf("%d", V + i);

	int P = 0;	// punctajul actual
	for (int i = 0; i < n; i++) {
		scanf("%d", &temp);

		P += V[i] * temp;

		// V[i] devine numarul de puncte obtinute daca ar lua 10
		V[i] = (10 - V[i]) * temp;
	}

	int p_min;
	scanf("%d", &p_min);

	// bouble sort
	for (int i = 0; i < n - 1; i++)
		for (int j = i + 1; j < n; j++)
			if (V[i] > V[j]) {
				temp = V[i];
				V[i] = V[j];
				V[j] = temp;
			}
	// se parcurge vectorul descrescator pt a lua cat mai putine materi
	for (int i = n - 1; i > 0; i--) {
		if (P >= p_min) {
			printf("%d\n", n - i - 1);
			return 0;
		}
		P += V[i];
	}
	// se verifica daca:
	if (P >= p_min)
		// are destule puncte
		printf("%d\n", n - 1);
	else if (P + V[0] >= p_min)
		// are nevoie de ultima materie
		printf("%d\n", n);
	else
		// a esuat
		printf("-1\n");
	return 0;
}

