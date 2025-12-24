#include <stdio.h>
#include <math.h>
void cofactor(float mat[100][100], float temp[100][100], int p, int q, int n)
{
    int i = 0, j = 0;
    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            if (r != p && c != q)
            {
                temp[i][j++] = mat[r][c];
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

float detr(float mat[100][100], int n)
{
    if (n == 1)
    {
        return mat[0][0];
    }

    float det = 0;
    float temp[100][100];
    int sign = 1;

    for (int f = 0; f < n; f++)
    {
        cofactor(mat, temp, 0, f, n);
        det += sign * mat[0][f] * detr(temp, n - 1);
        sign *= (-1);
    }
    return det;
}

void inverse(float mat[100][100], int n)
{
    int sign ;
    float temp[100][100], x[100][100], inverse[100][100];
    if (detr(mat, n) == 0)
    {
        printf("Determinant of matrix is zero\n");
        printf("Hence,inverse is not possible.\n");
    }
    else
    {

        for (int p = 0; p < n; p++)
        {
            for (int q = 0; q < n; q++)
            {
                cofactor(mat, temp, p, q, n);
                sign = ((p + q) % 2 == 0) ? 1 : -1;

                x[p][q] = sign * detr(temp, n - 1) / detr(mat, n);
            }
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                inverse[j][i] = x[i][j];
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%.2f ", inverse[i][j]);
            }
            printf("\n");
        }
    }
}

int rank(float mat[100][100], int n)
{
    float temp[100][100];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            temp[i][j] = mat[i][j];

    int rank = n;
    const float eps = 1e-6f;

    for (int row = 0; row < rank; row++)
    {
        float diag = temp[row][row];
        if ((diag > eps) || (diag < -eps))
        {
            for (int r = 0; r < n; r++)
            {
                if (r != row)
                {
                    float mult = temp[r][row] / temp[row][row];
                    for (int c = 0; c < rank; c++)
                        temp[r][c] -= mult * temp[row][c];
                }
            }
        }
        else
        {
            int reduce = 1;
            for (int i = row + 1; i < n; i++)
            {
                if ((temp[i][row] > eps) || (temp[i][row] < -eps))
                {
                    for (int c = 0; c < rank; c++)
                    {
                        float t = temp[row][c];
                        temp[row][c] = temp[i][c];
                        temp[i][c] = t;
                    }
                    reduce = 0;
                    break;
                }
            }

            if (reduce)
            {
                rank--;
                for (int i = 0; i < n; i++)
                    temp[i][row] = temp[i][rank];
                row--;
            }
        }
    }
    return rank;
}

int main()
{
    float mat[100][100];
    int opt;
    int n, k = 0;
    printf("Write the number of rows/columns of square matrix:");
    scanf("%d", &n);
    printf("Write the elements of matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%f", &mat[i][j]);
    printf("\n\n");

    while (k == 0)
    {
        printf("Options:\n\n");

        printf("1.Find the determinant of matrix\n");
        printf("2.Find the inverse of the following matrix:\n");
        printf("3.Find the rank of the matrix function:\n");
        printf("4.Terminate the program\n\n");

        printf("Select the option:");

        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            printf("The determinant of the matrix is %.2f\n\n\n", detr(mat, n));
            break;
        case 2:
            if (detr(mat, n) == 0)
            {
                printf("The determinant of the matrix is zero,hence not possible\n\n\n");
            }
            else
            {
                printf("The inverse matrix of the above matrix is:\n");
                inverse(mat, n);
                printf("\n\n\n");
            }
            break;
        case 3:
            printf("The rank of the matrix is %d\n\n\n", rank(mat, n));
            break;
        case 4:
            printf("Terminated\n");
            return 0;
        default:
            printf("Invalid option\n\n\n");
        }
    }
}