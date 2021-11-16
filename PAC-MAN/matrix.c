#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

char **matrix_alloc(unsigned int nrow, unsigned int ncol) {
	char **M = (char **)calloc(nrow,sizeof(char *));

	if(M != NULL) {
		M[0] = (char *)calloc(nrow*ncol,sizeof(char));
		if(M[0] == NULL) {
			free(M);
			M = NULL;
		} else {
			unsigned int i;

			for(i = 1; i < nrow; i++)
				M[i] = &M[0][i*ncol];
		}
	}
	return M;
}

static int matrix_size(char *file, unsigned int *nrow, unsigned int *ncol) {
	FILE *in = fopen(file,"r");

	if(in == NULL) {
		return 1;
	} else {
		unsigned int tmp;
		char c;
		
		*ncol = tmp = 0;
		while((c = fgetc(in)) != EOF && c != '\n') (*ncol)++;
		*nrow = 1;

		while((c = fgetc(in)) != EOF) {
			if(c =='\n') {
				(*nrow)++;
				if(*ncol != tmp) // All rows must have the same number of chars
					return 1;
				else
					tmp = 0;
			} else { 
				tmp++;
			}
		}

		return 0;
	}
}

char **matrix_read(char *file, unsigned int *nrow, unsigned int *ncol) {
	char **M = NULL;
	
	if(matrix_size(file,nrow,ncol) != 0) {
		return NULL;
	} else {
		FILE *in = fopen(file,"r");
		unsigned int i, j; 

		M = matrix_alloc(*nrow,*ncol);
	
		for(i = 0; i < *nrow; i++) {
			for(j = 0; j < *ncol; j++) 
				if((M[i][j] = fgetc(in)) == EOF || M[i][j] == '\n') {
					matrix_free(M);
					return NULL;
				}
		
			if(fgetc(in) != '\n') {
				matrix_free(M);
				return NULL;
			}
		}
		
		fclose(in);
	}

	return M;
}

char **matrix_copy(char **M, unsigned int nrow, unsigned int ncol) {
	char **N = NULL;

	if(M != NULL && M[0] != NULL) {
		unsigned int i, j;
		N = matrix_alloc(nrow,ncol);	
		for(i = 0; i < nrow; i++)
			for(j = 0; j < ncol; j++)
				N[i][j] = M[i][j];
	}

	return N;
}

void matrix_free(char **M) {
	if(M != NULL) {
		free(M[0]);
		free(M);
	}
}
