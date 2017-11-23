#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int k, i, j;
	int m, n;
	double **r, **q, **a, mysum;

	FILE* in;
	in = fopen("mat.txt", "r");
	
	fscanf(in, "%d%d", &m, &n);

	a = malloc(sizeof(double *) * m);
	q = malloc(sizeof(double *) * m);
	r = malloc(sizeof(double *) * n);

	for (i = 0; i < m; i++) {
		a[i] = malloc(sizeof(double) * n);
		q[i] = malloc(sizeof(double) * n);
		for (j = 0; j < n; j++)
			fscanf(in, "%lf", &a[i][j]);
	}
	for (i = 0; i < n; i++) {
		r[i] = malloc(sizeof(double) * n);
	}

	clock_t start_time = clock();

	for (k = 0; k < n; k++){
		r[k][k] = 0; // equivalent to sum = 0
		mysum = 0;
		
		// schedule static - default dividing the iterations in equal chunk_sizes
		for (i = 0; i < m; i++)
			mysum += a[i][k] * a[i][k];

		r[k][k] = mysum;

		// r_kk = norm(a_k)
		r[k][k] = sqrt(r[k][k]);

		for (i = 0; i < m; i++)
			q[i][k] = a[i][k] / r[k][k];
	 
		for(j = k + 1; j < n; j++) {
			r[k][j] = 0;
			mysum = 0;

			for(i = 0; i < m; i++)
				mysum += q[i][k] * a[i][j];
			r[k][j] = mysum;

			for (i = 0; i < m; i++)
				a[i][j] = a[i][j] - r[k][j] * q[i][k];
		}
	}

	clock_t delta_time = clock() - start_time;
	printf("%lf\n", (double) delta_time / (double) CLOCKS_PER_SEC);

	// for (i = 0; i < n; i++) {
	// 	for (j = 0; j < n; j++)
	// 		printf("%f ", r[i][j]);

	// 	printf("\n");
	// }

	for (i = 0; i < m; i++) {
		free(a[i]);
		free(q[i]);
	}
	for (i = 0; i < n; i++) {
		free(r[i]);
	}
	free(a);
	free(q);
	free(r);

	return 0;
}
