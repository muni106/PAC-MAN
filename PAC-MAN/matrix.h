#ifndef MATRIX_H
#define MATRIX_H

/* Dynamically allocates a char matrix of size nrow*ncol */ 
char **matrix_alloc(unsigned int nrow, unsigned int ncol);

/* Makes a copy of the input matrix */
char **matrix_copy(char **M, unsigned int nrow, unsigned int ncol);

/* Reads and allocates a char matrix from file */
char **matrix_read(char *file, unsigned int *nrow, unsigned int *ncol);

/* Deallocates the char matrix M */
void matrix_free(char **M);

#endif
