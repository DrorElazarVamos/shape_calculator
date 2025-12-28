#include "matrice.h"
#include <stdio.h>
#include <stdlib.h>

// Helper to consolidate swapping logic
void swap_pixels(pix *p1, pix *p2){
    if(!p1 || !p2) return;
    pix tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

pix *generatePixel(unsigned short r, unsigned short g, unsigned short b){
    pix *p = (pix *)malloc(sizeof(pix));
    if (p == NULL) return NULL;
    p->B = b;
    p->G = g;
    p->R = r;
    return p;
}

matrice *generateMatrice(unsigned short r, unsigned int c, int seed){
    matrice *m = (matrice *)malloc(sizeof(matrice));
    if (m == NULL) return NULL;
    
    m->cols = c;
    m->rows = r;
    m->Mat = (pix ***)malloc(sizeof(pix **) * r);
    
    for (unsigned short i = 0; i < r; i++){
        m->Mat[i] = (pix **)malloc(sizeof(pix *) * c);
    }
    
    rand_fill_matrice(seed, r, c, m->Mat);
    return m;
}

void rand_fill_matrice(int seed, unsigned int r, unsigned int c, pix ***mat){
    if (seed != 0) srand(seed);

    for (unsigned short i = 0; i < r; i++){
        for (unsigned short j = 0; j < c; j++){
            mat[i][j] = generatePixel((rand()%256), (rand()%256), (rand()%256));
        }
    }
}

// Fixed: Renamed to avoid conflict
void transposeMatriceInPlace(matrice *M){
    if (!M) return;
    unsigned short rows = M->rows, cols = M->cols;
    for (unsigned short i = 0; i < rows; i++){
        for (unsigned short j = i + 1; j < cols; j++){
            swap_pixels(M->Mat[i][j], M->Mat[j][i]);
        }
    }
}

pix *multiply_pixels(pix *p1, pix *p2){
    pix *res = (pix *)malloc(sizeof(pix));
    if (res == NULL) return NULL;

    res->R = p1->R * p2->R;
    res->G = p1->G * p2->G;
    res->B = p1->B * p2->B;
    return res;
}

pix *add_pixels(pix *p1, pix *p2){
    pix *res = (pix *)malloc(sizeof(pix));
    if (res == NULL) return NULL;

    res->R = p1->R + p2->R;
    res->G = p1->G + p2->G;
    res->B = p1->B + p2->B;
    return res; 
}

matrice *transposeMatrice(matrice *M, matrice *Dst, const size_t Diagonal){
    if (M == NULL) { fprintf(stderr, "Matrice is empty\n"); return NULL; }
    // Note: Assuming Diagonal param logic is handled by caller, keeping check simple
    if (Dst == NULL) Dst = M;

    const unsigned short n = M->rows;
    for (size_t i = 1; i < n; i++){
        for (size_t j = i; j < n; j++){ // Fixed loop logic (j < n)
            swap_pixels(Dst->Mat[i][j], Dst->Mat[j][i]);
        }
    }
    return Dst;
}

pix *Det(matrice *M){
    if (M->cols != M->rows) { fprintf(stderr, "Invalid matrice\n"); return NULL; }
    
    // Fixed: Base case must return a COPY, because caller will try to free it
    if (M->rows == 1) {
        return generatePixel(M->Mat[0][0]->R, M->Mat[0][0]->G, M->Mat[0][0]->B);
    }

    const size_t n = M->rows;
    
    // Fixed: Initialize to 0
    pix *Det_pix = generatePixel(0, 0, 0); 
    
    matrice *temp = generateMatrice(n-1, n-1, 0);
    int sign = 1; // Changed to int for easier math

    for (size_t f = 0; f < n; f++){
        cofactor(M, temp, 0, f, n);
        
        // Fixed: Calculate sub-determinant ONCE to prevent exponential recursion and leaks
        pix *subDet = Det(temp);
        
        Det_pix->B += sign * M->Mat[0][f]->B * subDet->B;
        Det_pix->G += sign * M->Mat[0][f]->G * subDet->G;
        Det_pix->R += sign * M->Mat[0][f]->R * subDet->R;
        
        free(subDet); // Fixed: Free the sub-determinant
        sign *= (-1);
    }

    free_matrice(temp); // Fixed: Free the temporary matrix
    return Det_pix;     // Fixed: Added return statement
}

void cofactor(matrice *M, matrice *tmp, const unsigned int p, const unsigned int q, const unsigned int n){
    unsigned int i = 0, j = 0;
    for (unsigned int r = 0; r < n; r++) {
        for (unsigned int c = 0; c < n; c++) {
            if (r != p && c != q) {
                // Fixed: Do not increment j++ three times for one pixel!
                tmp->Mat[i][j]->B = M->Mat[r][c]->B;
                tmp->Mat[i][j]->G = M->Mat[r][c]->G;
                tmp->Mat[i][j]->R = M->Mat[r][c]->R;
                
                j++; // Increment col index once per pixel
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

matrice *remove_row(matrice *M, unsigned short row){
    if (row >= M->rows) return NULL;

    matrice *neo_M = (matrice *)malloc(sizeof(matrice));
    neo_M->cols = M->cols;
    neo_M->rows = M->rows - 1;
    neo_M->Mat = (pix ***)malloc(sizeof(pix **) * neo_M->rows);

    int target_i = 0;
    for (unsigned short i = 0; i < M->rows; i++){
        if (i == row) continue;
        neo_M->Mat[target_i] = (pix **)malloc(sizeof(pix *) * neo_M->cols);
        for (unsigned short j = 0; j < M->cols; j++) {
            // Fixed: Deep Copy (create new pixel) instead of Shallow Copy
            neo_M->Mat[target_i][j] = generatePixel(
                M->Mat[i][j]->R, M->Mat[i][j]->G, M->Mat[i][j]->B
            );
        }
        target_i++;
    }  
    return neo_M;
}

matrice *remove_col(matrice *M, unsigned short col){
    if (col >= M->cols) return NULL;

    matrice *neo_M = (matrice *)malloc(sizeof(matrice));
    neo_M->cols = M->cols - 1;
    neo_M->rows = M->rows;
    neo_M->Mat = (pix ***)malloc(sizeof(pix **) * neo_M->rows);

    for (unsigned short i = 0; i < neo_M->rows; i++){
        neo_M->Mat[i] = (pix **)malloc(sizeof(pix *) * neo_M->cols);
        unsigned short target_j = 0;

        for (unsigned short j = 0; j < M->cols; j++){
            if (j == col) continue;
            // Fixed: Deep Copy
            neo_M->Mat[i][target_j] = generatePixel(
                M->Mat[i][j]->R, M->Mat[i][j]->G, M->Mat[i][j]->B
            );
            target_j++;
        }
    }  
    return neo_M;
}

void print_matrice(matrice *M){
    if(!M) return;
    unsigned short rows = M->rows, cols = M->cols;
    for (unsigned short i = 0; i < rows; i++){
        for (unsigned short j = 0; j < cols; j++){
            print_pixel(M->Mat[i][j]);
        }
        printf("\n");
    }
}

void print_pixel(pix *p){
    if(p) printf("<%hu|%hu|%hu>\t", p->B, p->G, p->R);
    else printf("<NULL>\t");
}

void free_matrice(matrice *M){
    if (M == NULL) return;
    for (unsigned short i = 0; i < M->rows; i++){
        for (unsigned short j = 0; j < M->cols; j++){
            if(M->Mat[i][j]) free(M->Mat[i][j]); // Free pixels
        }
        free(M->Mat[i]); // Free row pointers
    }  
    free(M->Mat); // Free rows array
    free(M);      // Free struct
}