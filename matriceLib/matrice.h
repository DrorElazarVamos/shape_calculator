#ifndef MATRICE_H
#define MATRICE_H

#include <stddef.h> // Fixed: Required for size_t
#include <stdio.h>
#include <stdlib.h> // Fixed: Required for malloc/free

typedef struct {
    unsigned short R;
    unsigned short G;
    unsigned short B;
} pix;

typedef struct {
    unsigned short rows;
    unsigned short cols;
    pix ***Mat;
} matrice;

// Memory Management
pix *generatePixel(unsigned short r, unsigned short g, unsigned short b);
matrice *generateMatrice(unsigned short r, unsigned int c, int seed);
void free_matrice(matrice *M);
void free_pixel(pix *p);

// Matrix Operations
void rand_fill_matrice(int seed, unsigned int r, unsigned int c, pix ***mat);

// Fixed: Renamed to avoid conflict with the other transpose function
void transposeMatriceInPlace(matrice *M); 
matrice *transposeMatrice(matrice *M, matrice *Dst, const size_t Diagonal);

pix *Det(matrice *M);
void cofactor(matrice *M, matrice *tmp, const unsigned int p, const unsigned int q, const unsigned int n);

// Helpers
void swap_pixels(pix *a, pix *b); // Consolidated swap functions
void print_matrice(matrice *M);
void print_pixel(pix *p);

pix *multiply_pixels(pix *p1, pix *p2);
pix *add_pixels(pix *p1, pix *p2);

// Note: Implementation for these two was not provided in snippet, but prototype kept
pix *average_pixel(pix *pix_arr, unsigned int arrSize);
void inverse(unsigned short **mat, unsigned int n); 

matrice *remove_row(matrice *M, unsigned short row);
matrice *remove_col(matrice *M, unsigned short col);

#endif // MATRICE_H