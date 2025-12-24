#ifndef VECTOROPS_H
#define VECTOROPS_H

#define EPSILON 1e-6

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define PI 3.1415

// --- Data Structures ---

typedef struct {
    double direction[3];
    double magnitude;
} vector;

typedef struct {
    vector *vectors; // Dynamic array of vectors
    size_t count;    // Number of vectors in the array
} VectorList;

typedef struct {
    double *point; // The coordinates of a point the line goes through (p0)
    double t; // Scalar parameter (often unused in the struct itself, but part of the math)
    vector direction_vector; // The direction vector of the line (d)
} line_equation;

typedef struct {
    vector orthogonal_vector;
    double *point;
} plain;

typedef struct {
    vector base_vector1;
    vector base_vector2;
    double *point;
} plainVectoric;

// --- Function Prototypes ---

/**
 * @brief Calculates the scalar (dot) product of 2 given vectors.
 * @param v1 vector 1
 * @param v2 vector 2
 * @return scalar product of the 2 vectors
 */
double scalaricProduct(vector v1, vector v2);

/**
 * @brief Calculates the cross product of 2 given vectors (the orthogonal vector).
 * @param v1 vector 1
 * @param v2 vector 2
 * @return cross product of the 2 vectors
 */
vector crossProduct(vector v1, vector v2);

/**
 * @brief Calculates the volume of a parallelepiped (Scalar Triple Product).
 * @param vectors[] the three direction vectors of the parallelepiped
 * @param k is the constant for the shape (divisor for the volume formula)
 * @return volume
 */
double volumeParallelepiped(vector vectors[], double k);

/**
 * @brief Frees the dynamically allocated memory used by the VectorList.
 * @param list The VectorList to clean up.
 */
void free_vector_list(VectorList *list);

/**
 * @brief Calculates the normal vector (unit vector of the same direction) of a given vector.
 * @param v The input vector.
 * @return The normal vector.
 */
vector getNoraml(vector v);

/**
 * @brief Calculates the addition or subtraction of two vectors.
 * @param v Vector 1.
 * @param u Vector 2.
 * @param n Dimension of the vectors (should be 3 for the current struct).
 * @param plusminus True for addition (+), False for subtraction (-).
 * @return A dynamically allocated vector result.
 */
vector *addition(vector *v, vector *u, unsigned short n ,bool plusminus);

/**
 * @brief Creates a vector from two points (q - p).
 * @param n Dimension of the points.
 * @param p Starting point coordinates.
 * @param q Ending point coordinates.
 * @return A dynamically allocated vector from P to Q.
 */
vector *getVectorFromPoints(unsigned short n, double *p, double *q);

/**
 * @brief Calculates the Euclidean distance between two points.
 * @param p1 Point 1 coordinates.
 * @param p2 Point 2 coordinates.
 * @param n Dimension of the points.
 * @return The distance between p1 and p2.
 */
double getDist(double *p1, double *p2, unsigned short n);

/**
 * @brief Checks if two vectors are parallel by comparing the ratio of their components.
 * @param v Vector 1.
 * @param u Vector 2.
 * @param n Dimension of the vectors (should be 3 for the current struct).
 * @return True if parallel, False otherwise (uses EPSILON for safety).
 */
bool checkParrallel(vector *v, vector *u, unsigned short n);

/**
 * @brief Creates a line equation from a direction vector and a point.
 * @param V The direction vector.
 * @param p The point coordinates.
 * @return A dynamically allocated line_equation structure.
 */
line_equation *getLine(vector *V, double *p);

/**
 * @brief Creates a line equation from two points (p1 as the base point, p2-p1 as direction).
 * @param p1 Point 1 (base point).
 * @param p2 Point 2.
 * @return A dynamically allocated line_equation structure.
 */
line_equation *getLine2Points(double *p1, double *p2);

/**
 * @brief Calculates the angle in radians between two vectors using the dot product formula.
 * @param v Vector 1.
 * @param u Vector 2.
 * @return The angle in radians.
 */
float getAngleRad(vector v, vector u);

/**
 * @brief Finds the intersection point of two 3D lines (L1 and L2).
 * @param L1 Pointer to the first line_equation.
 * @param L2 Pointer to the second line_equation.
 * @return A dynamically allocated double array {x, y, z} if intersection exists, or NULL if parallel/skew.
 */
double *getIntersection2Lines(line_equation *L1, line_equation *L2); // Corrected prototype to use pointers

/**
 * @brief Placeholder for a determinant calculation function (not fully implemented in the .c file).
 * @param matrix A pointer to the matrix data.
 * @return The determinant value.
 */
double detrminant(double **matrix);

plain *getPlain(vector orthogonal_V, double *point);

plain *getPlain2Vector(vector v1, vector v2, double *p);

bool checkPointInPlain(plain plainCheck, double *p1);

bool checkPlainParrallel(plain Plain1, plain Plain2);

plain *getPlain3point(double *p1, double *p2, double *p3);

double distPointPlain(double *p, plain Plain);

bool checkPointInPlainVect(plainVectoric Plain , double *p);

bool parrallelPlains(plain Plain1, plain Plain2);

#endif