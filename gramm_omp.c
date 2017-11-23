#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int k, i, j;
	int m, n;
	double **r, **q, **a, mysum;
	int num_threads = omp_get_max_threads();

	FILE* in;
	in = fopen("mat_1000.txt", "r");
	
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

	double start_time = omp_get_wtime();

	for (k = 0; k < n; k++){
		r[k][k] = 0; // equivalent to sum = 0
		mysum = 0;
		
		// schedule static - default dividing the iterations in equal chunk_sizes
		#pragma omp parallel for private(i) reduction(+:mysum) num_threads(num_threads) schedule(static) 
		for (i = 0; i < m; i++)
			mysum += a[i][k] * a[i][k];

		r[k][k] = mysum;

		// r_kk = norm(a_k)
		r[k][k] = sqrt(r[k][k]);

		#pragma omp parallel for private(i) shared(k, r) num_threads(num_threads) schedule(static) 
		for (i = 0; i < m; i++)
			q[i][k] = a[i][k] / r[k][k];
	 
	 	#pragma omp parallel for private(j, mysum) shared(k, r) 
		for(j = k + 1; j < n; j++) {
			r[k][j] = 0;
			mysum = 0;

			#pragma omp parallel for private(i) shared(k, j) reduction(+:mysum) schedule(static)
			for(i = 0; i < m; i++)
				mysum += q[i][k] * a[i][j];
			r[k][j] = mysum;
			#pragma omp parallel for private(i) shared(k, j) schedule(static)
			for (i = 0; i < m; i++)
				a[i][j] = a[i][j] - r[k][j] * q[i][k];
		}
	}


	double delta_time = omp_get_wtime() - start_time;
	printf("%lf\n", delta_time);

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
