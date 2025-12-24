#include "vectorOps.h"

double scalaricProduct(vector v1, vector v2) {
    double scalarProd = 0.0;
    for (int i=0; i<3; i++){
        scalarProd += v1.direction[i]*v2.direction[i];
    } 
    return scalarProd;
}

vector crossProduct(vector v1, vector v2){
    vector result;
    double x1 = v1.direction[0] ,y1 = v1.direction[1] ,z1 = v1.direction[2];
    double x2 = v2.direction[0] ,y2 = v2.direction[1] ,z2 = v2.direction[2];

    result.direction[0] = (y1 * z2) - (y2 * z1);
    result.direction[1] = (z1 * x2) - (x1 * z2);
    result.direction[2] = (x1 * y2) - (x2 * y1);

    result.magnitude = sqrt(
        pow(result.direction[0],2) + pow(result.direction[1],2) + pow(result.direction[2],2)
    );
    return result;
}

double volumeParallelepiped(vector vectors[],double k){
    vector v_cross = crossProduct(vectors[0], vectors[1]);
    double scalar_triple_product = scalaricProduct(v_cross, vectors[2]);
    return fabs(scalar_triple_product)/k;
}

void free_vector_list(VectorList *list) {
    if (list != NULL && list->vectors != NULL) {
        free(list->vectors);
        list->vectors = NULL;
        list->count = 0;
    }
}

vector getNoraml(vector v) {
    // Assuming magnitude is calculated correctly upon creation
    vector normal = v;
    if (v.magnitude > EPSILON) {
        for (int i = 0; i < 3; i++) {
            normal.direction[i] /= v.magnitude;
        }
        normal.magnitude = 1.0;
    } else {
        // Handle zero vector case (e.g., set to zero vector)
        for (int i = 0; i < 3; i++) {
            normal.direction[i] = 0.0;
        }
        normal.magnitude = 0.0;
    }
    return normal;
}

vector *addition(vector *v, vector *u, unsigned short n ,bool plusminus){
    vector *V_add_U = (vector *)malloc(sizeof(vector));
    if (V_add_U == NULL) return NULL;
    short tmp = 1;
    if (plusminus == false) tmp = -1;

    V_add_U->magnitude = 0.0;
    for (unsigned short i=0; i<n; i++){
        V_add_U->direction[i] =  v->direction[i] + (tmp * u->direction[i]); // FIX: Used i instead of 0
        V_add_U->magnitude += pow(V_add_U->direction[i],2);
    }

    V_add_U->magnitude = sqrt(V_add_U->magnitude);
    return V_add_U;
}

vector *getVectorFromPoints(unsigned short n, double *p, double *q){
    if ( n == 0){
         // In a real library, handle this error properly
         return NULL; 
    }

    vector *PQ = (vector *)malloc(sizeof(vector));
    if (PQ == NULL) return NULL;
    PQ->magnitude = 0.0;
    for (unsigned short i=0; i<n; i++){
        PQ->direction[i] = q[i]-p[i];
        PQ->magnitude += pow(PQ->direction[i],2);
    }

    PQ->magnitude = sqrt(PQ->magnitude);
    return PQ;
}

double getDist(double *p1, double *p2, unsigned short n){
    double dist_sq = 0.0;
    for (unsigned short i = 0; i<n; i++){
        dist_sq += pow((p2[i]-p1[i]),2);
    }
    return sqrt(dist_sq);
}

bool checkParrallel(vector *v, vector *u, unsigned short n){
    double *t = (double *)malloc(n * sizeof(double));
    if (t == NULL) return false;

    for (unsigned short i = 0; i < n; i++){
        if (fabs(u->direction[i]) < EPSILON) {
            if (fabs(v->direction[i]) > EPSILON) {
                free(t);
                return false;
            }
            continue; 
        }

        t[i] = v->direction[i] / u->direction[i];
        
        if (i > 0) {
            // Find the last non-skipped ratio for comparison
            unsigned short j = i - 1;
            while (j > 0 && fabs(u->direction[j]) < EPSILON) { 
                j--; 
            }
            // If the ratio exists and is non-zero, compare
            if (fabs(u->direction[j]) >= EPSILON && fabs(t[i] - t[j]) > EPSILON) {
                free(t);
                return false;
            }
        }
    }

    free(t);
    return true;
}

line_equation *getLine(vector *V, double *p){
    line_equation *result = (line_equation *)malloc(sizeof(line_equation));
    if (result == NULL) return NULL;

    result->direction_vector = *V; // Direct copy of the vector struct
    result->point = p;
    result->t = 1.0;

    return result;
}

line_equation *getLine2Points(double *p1, double *p2){
    line_equation *result = (line_equation *)malloc(sizeof(line_equation));
    if (result == NULL) return NULL;
    
    vector *d_vec_ptr = getVectorFromPoints(3, p1, p2); 
    if (d_vec_ptr == NULL) {
        free(result);
        return NULL;
    }

    result->direction_vector = *d_vec_ptr;
    free(d_vec_ptr); // Free the temporary vector used to calculate direction

    result->point = p1;
    result->t = 1.0;

    return result;
}

float getAngleRad(vector v, vector u){
    float top = scalaricProduct(v,u);
    float bottom = v.magnitude * u.magnitude;
    // Check for zero magnitude to prevent division by zero or domain errors
    if (bottom < EPSILON) return 0.0f; 
    
    // Clamp the argument to acos to [-1, 1] to prevent domain errors
    double arg = top / bottom;
    if (arg > 1.0) arg = 1.0;
    if (arg < -1.0) arg = -1.0;
    
    return acos(arg);
}


double *getIntersection2Lines(line_equation *L1, line_equation *L2) {

    // 1. Extract Coefficients (Setup the 2x2 System: x and y components)
    // (a1)t1 + (-a2)t2 = (x02 - x01)
    // (b1)t1 + (-b2)t2 = (y02 - y01)

    double a1 = L1->direction_vector.direction[0]; 
    double b1 = L1->direction_vector.direction[1]; 

    double a2 = -L2->direction_vector.direction[0]; 
    double b2 = -L2->direction_vector.direction[1]; 

    double c1 = L2->point[0] - L1->point[0]; 
    double c2 = L2->point[1] - L1->point[1]; 
    
    // 2. Solve for t1 and t2 using Cramer's Rule (Determinants)
    
    double D = (a1 * b2) - (b1 * a2);

    if (fabs(D) < EPSILON) {
        return NULL; 
    }

    double D_t1 = (c1 * b2) - (c2 * a2);
    double D_t2 = (a1 * c2) - (b1 * c1);

    double t1 = D_t1 / D;
    double t2 = D_t2 / D; 

    // 3. Verification: Check the Z-component (The Skew Test)

    double z1 = L1->point[2] + t1 * L1->direction_vector.direction[2]; 
    double z2 = L2->point[2] + t2 * L2->direction_vector.direction[2]; 

    if (fabs(z1 - z2) > EPSILON) {
        return NULL; 
    }
    
    // 4. Intersection Found: Calculate and Return Point
    
    double *intersection_point = (double *)malloc(3 * sizeof(double));
    if (intersection_point == NULL) return NULL;

    intersection_point[0] = L1->point[0] + t1 * L1->direction_vector.direction[0];
    intersection_point[1] = L1->point[1] + t1 * L1->direction_vector.direction[1];
    intersection_point[2] = z1; 

    return intersection_point;
}


double detrminant(double **matrix){
    // This is a placeholder for a complex function that needs the matrix dimensions
    // to be passed explicitly, as sizeof on a pointer doesn't return array size.
    
    /* unsigned short rows = sizeof(matrix)/8;
    unsigned short cols = sizeof(matrix[0])/8; */
    
    return 0.0;
}

plain *getPlain(vector orthogonal_V, double *p){
    plain *result = (plain *)malloc(sizeof(plain));

    result->orthogonal_vector = orthogonal_V;
    result->point = p;

    return result;
}

plain *getPlain2Vector(vector v1, vector v2, double *p){
    plain *result = (plain *)malloc(sizeof(plain));

    result->orthogonal_vector = crossProduct(v1,v2);
    result->point = p;

    return result;
}

bool checkPointInPlain(plain plainCheck, double *p1){
    //get the D in the equation Ax + By + Cz + d =0
    double D = 0;
    for (int i=0; i<3; i++){
        D += plainCheck.orthogonal_vector.direction[i]*plainCheck.point[i];
    }
    double temp = D;
    for (int i=0; i<3; i++){
        temp += plainCheck.orthogonal_vector.direction[i]*p1[i];
    }

    return (temp == 0)? true:false;
}   

bool checkPlainParrallel(plain Plain1, plain Plain2){
    return (checkParrallel(&Plain1.orthogonal_vector,&Plain2.orthogonal_vector,3)== true)? true:false;

}

// vectorOps.c: In function 'getPlain3point'

plain *getPlain3point(double *p1, double *p2, double *p3){
    plain *result = (plain *)malloc(sizeof(plain));
    if (result == NULL) return NULL; // Safety check

    // CORRECTED: Only declare V1 and V2 as pointers, since getVectorFromPoints returns pointers.
    vector *V1 = getVectorFromPoints(3,p1,p2);
    vector *V2 = getVectorFromPoints(3,p1,p3);
    
    if (V1 == NULL || V2 == NULL) {
        free(result);
        if (V1) free(V1); // Only free V1 if it was successfully allocated
        if (V2) free(V2); // Only free V2 if it was successfully allocated
        return NULL;
    }

    // result->orthogonal_vector is a 'vector' struct, so we dereference the pointers (*V1, *V2).
    result->orthogonal_vector = crossProduct(*V1, *V2);
    result->point = p1;

    // CRITICAL: Free the dynamically allocated vectors created by getVectorFromPoints.
    free(V1);
    free(V2);

    return result;
}

double get_D_forCanonicPlain(plain plainCheck){
    //get the D in the equation Ax + By + Cz + d =0
    double D = 0;
    for (int i=0; i<3; i++){
        D += plainCheck.orthogonal_vector.direction[i]*plainCheck.point[i];
    }
    return D;
}

double distPointPlain(double *p, plain Plain){
    if (checkPointInPlain(Plain,p) == true){
        return 0.0;
    }
    //dist equation point from plain : Distance = (abs(A*x0 + B*y0 + C*z + D))/(sqrt(A^2 + B^2 + C^2))
    double distTop = get_D_forCanonicPlain(Plain);
    double distBottom = 0.0;
    for (int i=0; i<3; i++){
        distTop += (Plain.orthogonal_vector.direction[i])*(p[i]);
        distBottom += pow(Plain.orthogonal_vector.direction[i],2);
    }
    return fabs(distTop)/sqrt(distBottom);

}

bool parrallel_or_intersec_Plains(plain Plain1, plain Plain2){
    //if the orthogonal vector are dependant than thier orthogonal vector are dependant 
    return (checkParrallel(&Plain1.orthogonal_vector,&Plain2.orthogonal_vector,3)? true:false);
}

bool same_or_distant_plain(plain Plain1, plain Plain2){
    //check if random point in plain1 exist in plain2
    return (checkPointInPlain(Plain1,Plain2.point)? true:false);
}

