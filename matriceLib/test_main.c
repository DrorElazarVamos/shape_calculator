#include "matrice.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    printf("Generating Matrice... \n");

    matrice *M = generateMatrice(4,5,543);
    if (M==NULL) return 1;
    
    print_matrice(M);

    printf("\nRemove row 2\n");
    matrice *new_M = remove_row(M,2);
    print_matrice(new_M);

    printf("\nRemove col 2\n");
    matrice *new_new_M = remove_col(new_M,4);
    print_matrice(new_new_M);

    free_matrice(M);
    free_matrice(new_M);
    free_matrice(new_new_M);

    return 0;
}