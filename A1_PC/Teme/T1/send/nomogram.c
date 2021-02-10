// Simache Cristian Ioan 314CA
#include <stdio.h>
#define MMAX 101
#define GMAX 52

int is_correct(int n, int m, int M[MMAX][MMAX],
			   int L[MMAX][GMAX], int C[MMAX][GMAX])
{
	//verificarea conditilor de pe lini
	for (int i = 1; i <= n; i++) {
		// j:contor pt parcurgerea liniei
		int j = 1;
		for (int k = 1; k <= L[i][0]; k++) {
			//pentru fiecare grup de puncte
			//mutat pana la urmatorul punct
			while (!M[i][j] && j <= m)
				j++;
			//verifica daca nu a iesit din limite
			if (j > m)
				return 0;
			//merge lungimea grupului
			for (int l = 1; l <= L[i][k]; l++) {
				//verifica daca este punct
				if (!M[i][j])
					return 0;
				j++;
			}
			//verifica daca grupul nu e prea lung
			if (M[i][j] && j <= m)
				return 0;
		}
		//parcurge restul liniei
		for (; j <= m; j++)
			if (M[i][j])
				return 0;
	}

	//verificarea conditilor de pe coloane
	for (int j = 1; j <= m; j++) {
		// i:contor pt parcurgerea coloanei
		int i = 1;
		for (int k = 1; k <= C[j][0]; k++) {
			//pentru fiecare grup de puncte
			//mutat pana la urmatorul punct
			while (!M[i][j] && i <= n)
				i++;
			//verifica daca nu a iesit din limite
			if (i > n)
				return 0;
			//merge lungimea grupului
			for (int l = 1; l <= C[j][k]; l++) {
				//verifica daca este punct
				if (!M[i][j])
					return 0;
				i++;
			}
			//verifica daca grupul nu e prea lung
			if (M[i][j] && i <= n)
				return 0;
		}
		//parcurge restul coloanei
		for (; i <= n; i++)
			if (M[i][j])
				return 0;
	}
	return 1;
}

int main(void)
{
	int T;
	scanf("%d", &T);

	int n, m;
	int M[MMAX][MMAX];
	int L[MMAX][GMAX];
	int C[MMAX][GMAX];
	while (T) {
		T--;

		//multa citire
		scanf("%d %d", &n, &m);
		for (int i = 1; i <= n; i++) {
			scanf("%d", L[i]);
			for (int j = 1; j <= L[i][0]; j++)
				scanf("%d", L[i] + j);
		}

		for (int i = 1; i <= m; i++) {
			scanf("%d", C[i]);
			for (int j = 1; j <= C[i][0]; j++)
				scanf("%d", C[i] + j);
		}

		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= m; j++)
				scanf("%d", M[i] + j);

		if (is_correct(n, m, M, L, C))
			printf("Corect\n");
		else
			printf("Eroare\n");
	}
	return 0;
}

