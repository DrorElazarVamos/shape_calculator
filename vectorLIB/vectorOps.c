#include "vectorOps.h"

// --- Constructors & Destructors ---

vector cnstVector(unsigned int dim) {
    vector v = (vector)malloc(sizeof(struct vector_struct));
    if (!v) return NULL;
    
    v->dim = dim;
    v->next = NULL;
    v->val = (double *)calloc(dim, sizeof(double));
    if (!v->val) {
        free(v);
        return NULL;
    }
    return v;
}

void dcnstVector(vector dst) {
    if (!dst) return;
    if (dst->val) free(dst->val);
    free(dst);
}

vectorSet cnstVectorSet() {
    vectorSet vs = (vectorSet)malloc(sizeof(struct vectorSet));
    if (!vs) return NULL;
    vs->count = 0;
    vs->head = NULL;
    return vs;
}

void dcnstrVectorSet(vectorSet dst) {
    if (!dst) return;
    vector current = dst->head;
    while (current) {
        vector temp = current;
        current = current->next;
        dcnstVector(temp);
    }
    free(dst);
}

void addToSet(vectorSet set, vector v) {
    if (!set || !v) return;
    v->next = set->head;
    set->head = v;
    set->count++;
}

// --- Basic Vector Ops ---

double scalaricProduct(vector v1, vector v2) {
    if (!v1 || !v2 || v1->dim != v2->dim) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < v1->dim; i++) {
        sum += v1->val[i] * v2->val[i];
    }
    return sum;
}

vector crossProduct(vector v1, vector v2) {
    // Defined primarily for 3D
    if (!v1 || !v2 || v1->dim != 3 || v2->dim != 3) return NULL;

    vector res = cnstVector(3);
    res->val[0] = v1->val[1] * v2->val[2] - v1->val[2] * v2->val[1];
    res->val[1] = v1->val[2] * v2->val[0] - v1->val[0] * v2->val[2];
    res->val[2] = v1->val[0] * v2->val[1] - v1->val[1] * v2->val[0];
    return res;
}

vector addition(vector v, vector u, bool plusminus) {
    if (!v || !u || v->dim != u->dim) return NULL;
    vector res = cnstVector(v->dim);
    for (int i = 0; i < v->dim; i++) {
        if (plusminus) res->val[i] = v->val[i] + u->val[i];
        else res->val[i] = v->val[i] - u->val[i];
    }
    return res;
}

vector getNormal(vector v) {
    if (!v) return NULL;
    double mag = 0;
    for (int i = 0; i < v->dim; i++) mag += v->val[i] * v->val[i];
    mag = sqrt(mag);

    vector res = cnstVector(v->dim);
    if (mag < EPSILON) return res; // Return 0 vector if magnitude 0

    for (int i = 0; i < v->dim; i++) res->val[i] = v->val[i] / mag;
    return res;
}

vector getVectorFromPoints(vector p, vector q) {
    // Vector = Head (q) - Tail (p)
    return addition(q, p, false); 
}

double getDist(vector p1, vector p2) {
    if (!p1 || !p2 || p1->dim != p2->dim) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < p1->dim; i++) {
        double d = p1->val[i] - p2->val[i];
        sum += d * d;
    }
    return sqrt(sum);
}

bool checkParallel(vector v, vector u) {
    if (!v || !u || v->dim != u->dim) return false;
    
    // Check via cross product for 3D, or ratio for ND
    if (v->dim == 3) {
        vector cp = crossProduct(v, u);
        double mag = 0;
        for(int i=0; i<3; i++) mag += cp->val[i]*cp->val[i];
        dcnstVector(cp);
        return (sqrt(mag) < EPSILON);
    }
    
    // Ratio check for other dimensions
    double k = 0;
    bool k_set = false;
    for(int i=0; i<v->dim; i++) {
        if (fabs(u->val[i]) < EPSILON) {
            if (fabs(v->val[i]) > EPSILON) return false;
            continue;
        }
        double ratio = v->val[i] / u->val[i];
        if (!k_set) { k = ratio; k_set = true; }
        else if (fabs(ratio - k) > EPSILON) return false;
    }
    return true;
}

float getAngleRad(vector v, vector u) {
    double dot = scalaricProduct(v, u);
    double magV = 0, magU = 0;
    for (int i=0; i<v->dim; i++) magV += v->val[i]*v->val[i];
    for (int i=0; i<u->dim; i++) magU += u->val[i]*u->val[i];
    
    double denom = sqrt(magV) * sqrt(magU);
    if (denom < EPSILON) return 0.0;
    
    double val = dot / denom;
    if (val > 1.0) val = 1.0;
    if (val < -1.0) val = -1.0;
    return (float)acos(val);
}

// --- Advanced Ops ---

double determinant(vector *matrix, int n) {
    // Basic implementation for 2x2 and 3x3
    if (n == 2) {
        return matrix[0]->val[0] * matrix[1]->val[1] - matrix[0]->val[1] * matrix[1]->val[0];
    }
    if (n == 3) {
        double val = 
            matrix[0]->val[0] * (matrix[1]->val[1]*matrix[2]->val[2] - matrix[1]->val[2]*matrix[2]->val[1]) -
            matrix[0]->val[1] * (matrix[1]->val[0]*matrix[2]->val[2] - matrix[1]->val[2]*matrix[2]->val[0]) +
            matrix[0]->val[2] * (matrix[1]->val[0]*matrix[2]->val[1] - matrix[1]->val[1]*matrix[2]->val[0]);
        return val;
    }
    return 0.0; // Not implemented for N > 3
}

double volumeParallelepiped(vector vectors[], double k) {
    // Volume is Scalar Triple Product: (A . (B x C))
    // vectors[0] = A, vectors[1] = B, vectors[2] = C
    if (!vectors || !vectors[0] || !vectors[1] || !vectors[2]) return 0.0;
    
    vector cross = crossProduct(vectors[1], vectors[2]);
    double vol = scalaricProduct(vectors[0], cross);
    dcnstVector(cross);
    
    return fabs(vol) / (k > EPSILON ? k : 1.0);
}

// --- Geometry: Lines ---

line_equation *getLine(vector V, vector p) {
    line_equation *l = malloc(sizeof(line_equation));
    l->direction = addition(V, cnstVector(V->dim), true); // Deep copy
    l->point = addition(p, cnstVector(p->dim), true);     // Deep copy
    return l;
}

line_equation *getLine2Points(vector p1, vector p2) {
    line_equation *l = malloc(sizeof(line_equation));
    l->direction = getVectorFromPoints(p1, p2);
    l->point = addition(p1, cnstVector(p1->dim), true); // Copy p1
    return l;
}

vector getIntersection2Lines(line_equation *L1, line_equation *L2) {
    // 3D Line Intersection is complex (lines are often skew).
    // Simple 2D logic or specific 3D check. 
    // Implementing standard closest point check or specific intersection if planar.
    // For this generic vector lib, we will assume 2D or perfect intersection for now.
    
    // Cramer's rule style for 2D intersection:
    // P1 + t*D1 = P2 + u*D2
    // t*D1 - u*D2 = P2 - P1
    
    if (L1->direction->dim != 3) return NULL; // Limiting to 3D for simplicity
    
    vector dp = addition(L2->point, L1->point, false); // P2 - P1
    vector cp1 = crossProduct(L1->direction, L2->direction);
    vector cp2 = crossProduct(dp, L2->direction);
    
    double det = scalaricProduct(cp1, cp1); // |D1 x D2|^2
    
    if (det < EPSILON) {
        // Parallel lines
        dcnstVector(dp); dcnstVector(cp1); dcnstVector(cp2);
        return NULL;
    }
    
    // t = ((P2 - P1) x D2) . (D1 x D2) / |D1 x D2|^2
    double t = scalaricProduct(cp2, cp1) / det;
    
    // Intersection = P1 + t*D1
    vector scaleD1 = cnstVector(3);
    for(int i=0; i<3; i++) scaleD1->val[i] = L1->direction->val[i] * t;
    
    vector result = addition(L1->point, scaleD1, true);
    
    // Cleanup
    dcnstVector(dp); dcnstVector(cp1); dcnstVector(cp2); dcnstVector(scaleD1);
    
    return result;
}

// --- Geometry: Plains ---

plain *getPlain(vector orthogonal_V, vector point) {
    plain *p = malloc(sizeof(plain));
    p->normal = getNormal(orthogonal_V);
    p->point = addition(point, cnstVector(point->dim), true);
    return p;
}

plain *getPlain2Vector(vector v1, vector v2, vector p) {
    vector normal = crossProduct(v1, v2);
    plain *res = getPlain(normal, p);
    dcnstVector(normal);
    return res;
}

plain *getPlain3point(vector p1, vector p2, vector p3) {
    vector v1 = getVectorFromPoints(p1, p2);
    vector v2 = getVectorFromPoints(p1, p3);
    plain *res = getPlain2Vector(v1, v2, p1);
    dcnstVector(v1);
    dcnstVector(v2);
    return res;
}

bool checkPointInPlain(plain p, vector point) {
    // Vector from plane point to target point
    vector v = getVectorFromPoints(p.point, point);
    // Dot product with normal should be 0
    double dot = scalaricProduct(v, p.normal);
    dcnstVector(v);
    return fabs(dot) < EPSILON;
}

bool checkPlainParallel(plain p1, plain p2) {
    return checkParallel(p1.normal, p2.normal);
}

double distPointPlain(vector p, plain plain_obj) {
    // D = |(P - P_plain) . Normal|
    vector v = getVectorFromPoints(plain_obj.point, p);
    double dot = scalaricProduct(v, plain_obj.normal);
    dcnstVector(v);
    return fabs(dot); // Normal is unit vector, so just dot product
}

// --- Utilities ---
void printVector(vector v) {
    if(!v) return;
    printf("[");
    for(int i=0; i<v->dim; i++) printf("%.2f%s", v->val[i], (i<v->dim-1)?", ":"");
    printf("]\n");
}

void printSet(vectorSet set) {
    if(!set) return;
    vector curr = set->head;
    int i = 0;
    while(curr) {
        printf("%d: ", i++);
        printVector(curr);
        curr = curr->next;
    }
}