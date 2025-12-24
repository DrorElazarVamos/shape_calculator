#include "matrice.h"
#include <stdio.h>
#include <stdlib.h>

void swap(pix *p1, pix *p2){
    pix tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

pix *generatePixel(unsigned short r, unsigned short g, unsigned short b){
    pix *p = (pix *)malloc(sizeof(pix));
    if (p==NULL) return NULL;
    p->B = b;
    p->G =g;
    p->R = r;

    return p;
}

matrice *generateMatrice(unsigned short r, unsigned c, int seed){
    matrice *m = (matrice *)malloc(sizeof(matrice));
    m->cols = c;
    m->rows = r;
    m->Mat = (pix ***)malloc(sizeof(pix **) * r);
    for (unsigned short i=0; i<r; i++){
        m->Mat[i] = (pix **)malloc(sizeof(pix *) * c);
    }
    rand_fill_matrice(seed,r,c,m->Mat);

    return m;
    
}

void rand_fill_matrice(int seed, unsigned int r, unsigned int c, pix ***mat){
    if (seed != 0) srand(seed);

    for (unsigned short i=0; i<r; i++){
        for (unsigned short j=0; j<c; j++){
            mat[i][j] = generatePixel((rand()%256),(rand()%256),(rand()%256));
        }
    }

}

void transposeMatrice(matrice *M){
    unsigned short rows = M->rows, cols = M->cols;
    for (unsigned short i=0; i<rows; i++){
        for (unsigned short j=i+1; j<cols; j++){
            swap_pixels(M->Mat[i][j],M->Mat[j][i]);
        }
    }
}

pix *multiply_pixels(pix *p1, pix *p2){
    pix *res = (pix *)malloc(sizeof(pix));
    if (res == NULL) return NULL;

    res->R = p1->R*p2->R;
    res->G = p1->G*p2->G;
    res->B = p1->B*p2->B;

    return res;
}

pix *add_pixels(pix *p1, pix *p2){
    pix *res = (pix *)malloc(sizeof(pix));
    if (res == NULL) return NULL;

    res->R = p1->R+p2->R;
    res->G = p1->G+p2->G;
    res->B = p1->B+p2->B;

    return res; 
}

matrice *transposeMatrice(matrice *M, matrice *Dst,const size_t Diagnol){
    if (M==NULL) fprintf(stderr,"Matrice is empty");
    if (Diagnol != 0 || Diagnol != 1) fprintf(stderr,"InValid argument for Diagnol");
    if (Dst == NULL) Dst = M;
    const unsigned short n = M->rows;
    for (size_t i=1; i<n; i++){
        for (size_t j=i; j<n; j++){
            swap(Dst->Mat[i][j],Dst->Mat[j][i]);
        }
    }
    return Dst;
}

void inverse(unsigned short **mat, unsigned int n);

pix *Det(matrice *M){
    if (M->cols!=M->rows) fprintf(stderr,"Invalid matrice");
    if (M->rows = 1) return M->Mat[0][0];
    const size_t n = M->rows;
    pix *Det_pix = (pix *)malloc(sizeof(pix));
    matrice *temp = generateMatrice(n-1, n-1,0);
    size_t sign = 1;

    for (size_t f=0; f<n; f++){
        cofactor(M, temp, 0, f, n);
        Det_pix->B += sign *  M->Mat[0][f]->B * Det(temp)->B;
        Det_pix->G += sign *  M->Mat[0][f]->G * Det(temp)->G;
        Det_pix->R += sign *  M->Mat[0][f]->R * Det(temp)->R;
        sign *= (-1);
    }

}

void cofactor(matrice *M, matrice *tmp, const unsigned int p, const unsigned int q, const unsigned int n){
    unsigned int i = 0, j = 0;
    for (unsigned int r = 0; r < n; r++)
    {
        for (unsigned int c = 0; c < n; c++)
        {
            if (r != p && c != q)
            {
                tmp->Mat[i][j++]->B = M->Mat[r][c]->B;
                tmp->Mat[i][j++]->G = M->Mat[r][c]->G;
                tmp->Mat[i][j++]->R = M->Mat[r][c]->R;
                if (j == n - 1)
                {
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
        if (i==row) continue;
        neo_M->Mat[target_i] = (pix **)malloc(sizeof(pix *) * neo_M->cols);
        for (unsigned short j = 0; j < M->cols; j++) neo_M->Mat[target_i][j] = M->Mat[i][j];
        target_i++;
    }  
    return neo_M;
}

matrice *remove_col(matrice *M, unsigned short col){
    if (col >= M->cols) return NULL; // Safety check

    matrice *neo_M = (matrice *)malloc(sizeof(matrice));
    neo_M->cols = M->cols - 1;
    neo_M->rows = M->rows;
    neo_M->Mat = (pix ***)malloc(sizeof(pix **) * neo_M->rows);
    for (unsigned short i = 0; i < neo_M->rows; i++){
        neo_M->Mat[i] = (pix **)malloc(sizeof(pix *) * neo_M->cols);
        
        unsigned short target_j = 0;

        for (unsigned short j = 0; j < M->cols; j++){
            if (j == col) continue;
            neo_M->Mat[i][target_j] = M->Mat[i][j]; 
            target_j++;
        }
    }  
    return neo_M;
}

void swap_pixels(pix *a, pix *b){
    pix temp = *a;
    *a = *b;
    *b = temp;
}

void print_matrice(matrice *M){
    unsigned short rows = M->rows, cols=M->cols;
    for (unsigned short i=0; i<rows; i++){
        for (unsigned short j=0; j<cols; j++){
            print_pixel(M->Mat[i][j]);
        }
        printf("\n");
    }
}

void print_pixel(pix *p){
    printf("<%hu|%hu|%hu>\t",p->B,p->G,p->R);
}

void free_matrice(matrice *M){
    if (M == NULL) return; // Safety check

    for (unsigned short i = 0; i < M->rows; i++){
        free(M->Mat[i]);
    }  
    free(M->Mat);
    free(M);
}