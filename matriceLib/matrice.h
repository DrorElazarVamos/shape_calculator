#ifndef MATRICE_H
#define MATRICE_H

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

    void swap(pix *p1, pix *p2);

// memory check generate free
pix *generatePixel(unsigned short r, unsigned short g, unsigned short b);

matrice *generateMatrice(unsigned short r, unsigned c, int seed);

void rand_fill_matrice(int seed, unsigned int r, unsigned int c, pix ***mat);

// matrice utilies

matrice *transposeMatrice(matrice *M, matrice *Dst,const size_t Diagnol);

void inverse(unsigned short **mat, unsigned int n);

pix *Det(matrice *M);

void cofactor(matrice *M, matrice *tmp, const unsigned int p, const unsigned int q, const unsigned int n);
//helpers

void swap_pixels(pix *a, pix *b);

void print_matrice(matrice *M);

void print_pixel(pix *p);

pix *multiply_pixels(pix *p1, pix *p2);

pix *add_pixels(pix *p1, pix *p2);

pix *average_pixel(pix *pix_arr, unsigned int arrSize);

matrice *remove_row(matrice *M, unsigned short row);

matrice *remove_col(matrice *M, unsigned short col);

void free_matrice(matrice *M);

void free_pixel(pix *p);
#endif // MATRICE_H