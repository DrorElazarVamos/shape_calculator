#ifndef VECTOROPS_H
#define VECTOROPS_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define EPSILON 1e-6
#define PI 3.1415

// --- Data Structures ---

// Forward declaration
struct vector_struct;

/**
 * @brief Main Vector ADT.
 * Defined as a pointer to the struct to allow easy passing by reference.
 */
typedef struct vector_struct {
    unsigned int dim : 4;       // Increased to 4 bits to allow dimensions up to 15 (3 bits maxes at 7)
    double *val;                // Dynamic array of values
    struct vector_struct *next; // Linked list pointer for the Set
} *vector;

typedef struct vectorSet {
    vector head;        // Head of the linked list
    unsigned int count; // Number of vectors in set
} *vectorSet;

// Geometry Structures
typedef struct line_equation {
    vector direction;
    vector point;
} line_equation;

typedef struct plain {
    vector normal;
    vector point; // A point on the plane
} plain;

// --- Constructors & Memory Management ---

vector cnstVector(unsigned int dim);
void dcnstVector(vector dst);

vectorSet cnstVectorSet();
void dcnstrVectorSet(vectorSet dst);
void addToSet(vectorSet set, vector v);

// --- Basic Vector Operations ---

/** * @brief Calculates scalar (dot) product. 
 */
double scalaricProduct(vector v1, vector v2);

/** * @brief Calculates cross product (Orthogonal vector). Only for 3D. 
 */
vector crossProduct(vector v1, vector v2);

/** * @brief Vector addition or subtraction.
 * @param plusminus true for (+), false for (-).
 */
vector addition(vector v, vector u, bool plusminus);

/** * @brief Returns the normalized (unit) vector. 
 */
vector getNormal(vector v); 

/** * @brief Creates a vector from Point P to Point Q (Q - P). 
 */
vector getVectorFromPoints(vector p, vector q);

/** * @brief Euclidean distance between two vectors/points. 
 */
double getDist(vector p1, vector p2);

/** * @brief Checks if vectors are parallel. 
 */
bool checkParallel(vector v, vector u);

/** * @brief Angle in radians between two vectors. 
 */
float getAngleRad(vector v, vector u);

// --- Advanced Operations ---

/** * @brief Scalar Triple Product volume. 
 */
double volumeParallelepiped(vector vectors[], double k);

/** * @brief Calculates determinant of a matrix (helper for volume/intersection).
 * @param matrix Square matrix represented as array of vectors or double**.
 * Note: Implementation adapted to take array of vectors for consistency.
 */
double determinant(vector *matrix, int n);

// --- Geometry Generation (Lines) ---

line_equation *getLine(vector V, vector p);

line_equation *getLine2Points(vector p1, vector p2);

/** * @brief Finds intersection of two lines. Returns vector intersection or NULL. 
 */
vector getIntersection2Lines(line_equation *L1, line_equation *L2);

// --- Geometry Generation (Plains) ---

/** * @brief Construct plane from normal and point. 
 */
plain *getPlain(vector orthogonal_V, vector point);

/** * @brief Construct plane from two vectors and a point. 
 */
plain *getPlain2Vector(vector v1, vector v2, vector p);

/** * @brief Construct plane from 3 points. 
 */
plain *getPlain3point(vector p1, vector p2, vector p3);

// --- Geometry Checks (Plains) ---

bool checkPointInPlain(plain p, vector point);

bool checkPlainParallel(plain p1, plain p2);

double distPointPlain(vector p, plain plain_obj);

// Utilities
void printVector(vector v);
void printSet(vectorSet set);

#endif // VECTOROPS_H