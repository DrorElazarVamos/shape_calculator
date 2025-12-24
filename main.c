#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 

// --- Custom Library Headers ---
#include "vectorOps.h"    // Assumed header for vector operations
#include "csvHandler.h"   // Assumed header for CSV handling
#include "testerFile.h"   // Assumed header for automated testing
#include "modular.h"      // Assumed header for modular arithmetic
#include "bitOperation.h" // NEW: Header for Bit and Base Operations

// --- Forward Declarations ---
void display_main_menu(void);
void clear_screen(void);
void pause_screen(void);
int get_user_choice(void);
void run_tester_module(void);
void run_single_calculation(void);
void display_about(void);
void vector_operation(void);
void modular_operation(void);
void volume_calculation(void);
void bit_operation_menu(void);
void plain_operation_menu(void); // NEW: Plain/Plane Operations

// --- Helper for Reading Points/Vectors ---
// Reads a 3-element point/vector array from user
bool read_3d_input(const char *prompt, double *out_arr) {
    printf("%s", prompt);
    if (scanf("%lf %lf %lf", &out_arr[0], &out_arr[1], &out_arr[2]) != 3) {
        printf("Invalid input. Please enter three numbers separated by spaces.\n");
        while (getchar() != '\n');
        return false;
    }
    while (getchar() != '\n');
    return true;
}

// Helper to create a vector from an array (only fills direction array, magnitude is 0)
vector create_vector_from_array(double *arr) {
    vector v;
    v.direction[0] = arr[0];
    v.direction[1] = arr[1];
    v.direction[2] = arr[2];
    v.magnitude = sqrt(pow(v.direction[0], 2) + pow(v.direction[1], 2) + pow(v.direction[2], 2));
    return v;
}

// --- Main Function ---
int main() {
    int choice;
    bool running = true;

    while (running) {
        clear_screen();
        display_main_menu();
        choice = get_user_choice();

        switch (choice) {
            case -1:
                printf("\nThank you for using my calculator!\n");
                running = false;
                break;
            case 0:
                display_about();
                break;
            case 1:
                run_tester_module();
                break;
            case 2:
                run_single_calculation();
                break;
            case 3:
                modular_operation();
                break;
            case 4: 
                bit_operation_menu();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pause_screen();
        }
    }

    return 0;
}

// --- UI Helper Functions ---

void display_main_menu(void) {
    printf("=============================================\n");
    printf("||   Advanced Math Calculator - Main Menu  ||\n");
    printf("=============================================\n");
    printf("|                                            |\n");
    printf("|  1. Run Automated Test Suite               |\n");
    printf("|  2. Calculations (Vector/Volume/Plain)     |\n");
    printf("|  3. Modular operations                     |\n");
    printf("|  4. Bit & Base Operations (K < 11)         |\n");
    printf("|  0. About                                  |\n");
    printf("| -1. Exit                                   |\n");
    printf("|                                            |\n");
    printf("=============================================\n");
    printf("\nEnter your choice ((-1)-4): "); 
}

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause_screen(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n' && getchar() != EOF); 
    getchar();
}

int get_user_choice(void) {
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    return choice;
}

// --- Module: Automated Test Suite ---
void run_tester_module(void) {
    char filename[256];
    int test_choice;
    
    clear_screen();
    printf("=============================================\n");
    printf("|        Automated Test Suite Runner        |\n");
    printf("=============================================\n\n");
    
    printf("Enter the CSV test file path: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename.\n");
        pause_screen();
        return;
    }
    
    size_t len = strcspn(filename, "\n");
    if (len >= sizeof(filename) - 1) {
        fprintf(stderr, "Error: Filename too long (max %zu characters)\n", sizeof(filename) - 2);
        pause_screen();
        return;
    }
    filename[len] = '\0';
    
    if (len == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        pause_screen();
        return;
    }

    CsvFile *csv = csv_open(filename);
    if (csv == NULL) {
        printf("\nError: Could not open file '%s'\n", filename);
        printf("Please check:\n");
        printf("  - File exists in current directory\n");
        printf("  - File path is correct\n");
        printf("  - You have read permissions\n");
        pause_screen();
        return;
    }
    
    if (!csv_read_line(csv)) {
        printf("Error: File is empty or could not read header.\n");
        csv_close(csv);
        pause_screen();
        return;
    }

    printf("\n--- Test Options ---\n");
    printf("1. Test Parallelepiped Volume (k=1)\n");
    printf("2. Test Pyramid Volume (k=6)\n");
    printf("3. Test Cross Product\n");
    printf("4. Test Scalar Product\n");
    printf("5. Run All Tests\n");
    printf("0. Cancel and Return to Main Menu\n");
    printf("\nEnter choice (0-5): ");
    
    if (scanf("%d", &test_choice) != 1) {
        fprintf(stderr, "Invalid input.\n");
        csv_close(csv);
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');

    if (test_choice == 0) {
        csv_close(csv);
        return;
    }

    printf("\n");
    switch(test_choice) {
        case 1:
            run_volume_tests(csv, volumeParallelepiped, "Parallelepiped Volume", 1.0);
            break;
        case 2:
            run_volume_tests(csv, volumeParallelepiped, "Pyramid Volume", 6.0);
            break;
        case 3:
            run_cross_product_tests(csv, crossProduct);
            break;
        case 4:
            run_scalar_product_tests(csv, scalaricProduct);
            break;
        case 5:
            run_volume_tests(csv, volumeParallelepiped, "Parallelepiped Volume", 1.0);
            run_volume_tests(csv, volumeParallelepiped, "Pyramid Volume", 6.0);
            run_cross_product_tests(csv, crossProduct);
            run_scalar_product_tests(csv, scalaricProduct);
            break;
        default:
            printf("Invalid choice.\n");
    }
    
    csv_close(csv);
    pause_screen();
}

// --- Module: Vector Operation ---
void vector_operation(void) {
    vector *vectorsArg = NULL;
    int operation;
    int vectorCount = 0;

    clear_screen();
    printf("=============================================\n");
    printf("|            Vector Operation                |\n");
    printf("=============================================\n\n");
    printf("|                                            |\n");
    printf("|  1. Cross Product                          |\n");
    printf("|  2. Scalar Product                         |\n");
    printf("|  3. Addition/Subtraction                   |\n");
    printf("|  4. Get Normal Vector                      |\n");
    printf("|  5. Check if Vector is in Shape            |\n");
    printf("|  0. Cancel                                 |\n");
    printf("|                                            |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (0-5): ");

    if (scanf("%d", &operation) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');

    switch (operation) {
        case 1:
        case 2:
            vectorCount = 2;
            break;
        case 3:
            printf("\nEnter how many vectors you want to add/sub (min: 2, max: 10): ");
            if (scanf("%d", &vectorCount) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            if (vectorCount < 2 || vectorCount > 10) {
                printf("Invalid input. Must be between 2 and 10.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            break;
        case 4:
            vectorCount = 1;
            break;
        case 5:
            printf("\nEnter how many vectors (3 for shape + at least 1 to check) (min: 4, max: 10): ");
            if (scanf("%d", &vectorCount) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            if (vectorCount < 4 || vectorCount > 10) {
                printf("Invalid input. Must be between 4 and 10.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            break;
        case 0:
            return;
        default:
            printf("Invalid choice.\n");
            pause_screen();
            return;
    }

    vectorsArg = (vector*)malloc(vectorCount * sizeof(vector));
    if (vectorsArg == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        pause_screen();
        return;
    }

    while (getchar() != '\n');
    for (int i = 0; i < vectorCount; i++) {
        printf("Enter Vector %d (X Y Z): ", i + 1);
        if (scanf("%lf %lf %lf", &vectorsArg[i].direction[0], 
                     &vectorsArg[i].direction[1], &vectorsArg[i].direction[2]) != 3) {
            printf("Invalid input. Please enter three numbers separated by spaces.\n");
            while (getchar() != '\n');
            free(vectorsArg);
            pause_screen();
            return;
        }
        vectorsArg[i].magnitude = sqrt(
            pow(vectorsArg[i].direction[0], 2) + 
            pow(vectorsArg[i].direction[1], 2) + 
            pow(vectorsArg[i].direction[2], 2)
        );
    }

    printf("\n--- Results ---\n");
    
    switch(operation) {
        case 1: {
            vector result = crossProduct(vectorsArg[0], vectorsArg[1]);
            printf("V1 x V2 = [%.6lf, %.6lf, %.6lf]\n", 
                       result.direction[0], result.direction[1], result.direction[2]);
            printf("Magnitude: %.6lf\n", result.magnitude);
            
            double dot_v1 = scalaricProduct(result, vectorsArg[0]);
            double dot_v2 = scalaricProduct(result, vectorsArg[1]);
            printf("Verification: V1·result = %.6lf, V2·result = %.6lf (should be ~0)\n", 
                       dot_v1, dot_v2);
            break;
        }
        case 2: {
            double result = scalaricProduct(vectorsArg[0], vectorsArg[1]);
            printf("V1 · V2 = %.6lf\n", result);
            break;
        }
        case 3: {
            char operation_type;
            printf("Addition or Subtraction (+/-): ");
            while (getchar() != '\n');
            if (scanf("%c", &operation_type) != 1) {
                printf("Invalid input.\n");
                free(vectorsArg);
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            
            vector result = {0};
            if (operation_type == '+') {
                for (int i = 0; i < vectorCount; i++) {
                    for (int j = 0; j < 3; j++) {
                        result.direction[j] += vectorsArg[i].direction[j];
                    }
                }
                printf("Sum: [%.6lf, %.6lf, %.6lf]\n", 
                        result.direction[0], result.direction[1], result.direction[2]);
            } else if (operation_type == '-') {
                for (int j = 0; j < 3; j++) {
                    result.direction[j] = vectorsArg[0].direction[j];
                }
                for (int i = 1; i < vectorCount; i++) {
                    for (int j = 0; j < 3; j++) {
                        result.direction[j] -= vectorsArg[i].direction[j];
                    }
                }
                printf("Difference: [%.6lf, %.6lf, %.6lf]\n", 
                        result.direction[0], result.direction[1], result.direction[2]);
            } else {
                printf("Invalid operation.\n");
                free(vectorsArg);
                pause_screen();
                return;
            }
            
            result.magnitude = sqrt(
                pow(result.direction[0], 2) + 
                pow(result.direction[1], 2) + 
                pow(result.direction[2], 2)
            );
            printf("Magnitude: %.6lf\n", result.magnitude);
            break;
        }
        case 4: {
            if (vectorsArg[0].magnitude == 0) {
                printf("Cannot normalize zero vector.\n");
            } else {
                vector normal;
                for (int i = 0; i < 3; i++) {
                    normal.direction[i] = vectorsArg[0].direction[i] / vectorsArg[0].magnitude;
                }
                normal.magnitude = 1.0;
                printf("Normal (Unit) Vector: [%.6lf, %.6lf, %.6lf]\n", 
                        normal.direction[0], normal.direction[1], normal.direction[2]);
                printf("Magnitude: %.6lf\n", normal.magnitude);
            }
            break;
        }
        case 5: {
            vector shape[3] = {vectorsArg[0], vectorsArg[1], vectorsArg[2]};
            double volume = volumeParallelepiped(shape, 1.0); 
            
            printf("Shape Volume: %.6lf\n\n", volume);
            
            if (fabs(volume) < 0.001) {
                printf("Warning: Vectors are coplanar (volume ~0)\n");
            }
            
            for (int i = 3; i < vectorCount; i++) {
                printf("Checking Vector %d:\n", i + 1);
                
                vector test1[3] = {vectorsArg[i], vectorsArg[1], vectorsArg[2]};
                vector test2[3] = {vectorsArg[0], vectorsArg[i], vectorsArg[2]};
                vector test3[3] = {vectorsArg[0], vectorsArg[1], vectorsArg[i]};
                
                double vol1 = volumeParallelepiped(test1, 1.0);
                double vol2 = volumeParallelepiped(test2, 1.0);
                double vol3 = volumeParallelepiped(test3, 1.0);
                
                double total = fabs(vol1) + fabs(vol2) + fabs(vol3); 
                
                printf("  Sub-volumes: %.6lf + %.6lf + %.6lf = %.6lf\n", 
                        fabs(vol1), fabs(vol2), fabs(vol3), total);
                
                if (fabs(total - fabs(volume)) < 0.001) {
                    printf("  Result: Vector IS inside the parallelepiped\n\n");
                } else {
                    printf("  Result: Vector IS NOT inside the parallelepiped\n\n");
                }
            }
            break;
        }
        default:
            printf("Invalid choice.\n");
    }
    
    free(vectorsArg);
    pause_screen();
}

// --- Module: Volume Calculation ---
void volume_calculation(void) {
    vector v1, v2, v3;
    
    clear_screen();
    printf("=============================================\n");
    printf("|         Volume Calculation Mode            |\n");
    printf("=============================================\n\n");
    
    printf("Enter Vector 1 (X Y Z): ");
    if (scanf("%lf %lf %lf", &v1.direction[0], &v1.direction[1], &v1.direction[2]) != 3) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    
    printf("Enter Vector 2 (X Y Z): ");
    if (scanf("%lf %lf %lf", &v2.direction[0], &v2.direction[1], &v2.direction[2]) != 3) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    
    printf("Enter Vector 3 (X Y Z): ");
    if (scanf("%lf %lf %lf", &v3.direction[0], &v3.direction[1], &v3.direction[2]) != 3) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }

    unsigned int k;
    printf("Enter the type of shape(const k=? - 1 for Parallelepiped, 6 for Pyramid): ");
    if (scanf("%u",&k) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    
    vector vectors[3] = {v1, v2, v3};
    double vol_shape = volumeParallelepiped(vectors, (double)k);
    
    printf("\n--- Results ---\n");
    printf("Shape's Volume: %.6lf\n", vol_shape);
    
    if (fabs(vol_shape) < 0.001) {
        printf("\nWarning: Vectors are coplanar (parallel or in same plane)\n");
    }
    
    while (getchar() != '\n');
    pause_screen();
}

// --- Module: Single Calculation ---
void run_single_calculation(void) {
    int calc_choice;
    
    clear_screen();
    printf("=============================================\n");
    printf("|            Calculation Mode                |\n");
    printf("=============================================\n\n");
    printf("|                                            |\n");
    printf("|  1. Volume of a Shape                      |\n");
    printf("|  2. Vector Operations                      |\n");
    printf("|  3. Plane/Plain Geometry                   |\n");
    printf("|  0. Cancel                                 |\n");
    printf("|                                            |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (0-3): ");
    
    if (scanf("%d", &calc_choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');
    
    switch(calc_choice) {
        case 1:
            volume_calculation();
            break;
        case 2:
            vector_operation();
            break;
        case 3:
            plain_operation_menu();
            break;
        case 0:
            return;
        default:
            printf("Invalid choice.\n");
            pause_screen();
    }
}

// --- Module: Plane/Plain Geometry ---
void plain_operation_menu(void) {
    int op_choice;
    
    double p1_arr[3], p2_arr[3], p3_arr[3];
    double check_p_arr[3];
    vector v1, v2, n_vec;
    plain *Plain1 = NULL, *Plain2 = NULL;
    
    clear_screen();
    printf("=============================================\n");
    printf("|         Plane/Plain Geometry Mode          |\n");
    printf("=============================================\n\n");
    printf("|  1. Get Plain (Orthogonal V + Point)       |\n");
    printf("|  2. Get Plain (2 Vectors + Point)          |\n");
    printf("|  3. Get Plain (3 Points)                   |\n");
    printf("|  4. Check Point in Plain                   |\n");
    printf("|  5. Distance Point to Plain                |\n");
    printf("|  6. Check Plain Parallelism                |\n");
    printf("|  7. Check Plain Intersection               |\n");
    printf("|  8. Check Point in Plain (Vectoric Form)   |\n");
    printf("|  0. Cancel                                 |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (0-8): ");

    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');
    
    printf("\n--- Result ---\n");

    switch(op_choice) {
        case 1:
            if (!read_3d_input("Enter Orthogonal Vector N (X Y Z): ", p1_arr) ||
                !read_3d_input("Enter Point P (X Y Z): ", p2_arr)) break;
            
            n_vec = create_vector_from_array(p1_arr);
            Plain1 = getPlain(n_vec, p2_arr); 
            if (Plain1) {
                printf("Plain Created. N = [%.2lf, %.2lf, %.2lf], P = [%.2lf, %.2lf, %.2lf]\n",
                    Plain1->orthogonal_vector.direction[0], Plain1->orthogonal_vector.direction[1], Plain1->orthogonal_vector.direction[2],
                    Plain1->point[0], Plain1->point[1], Plain1->point[2]);
                free(Plain1);
            }
            break;

        case 2:
            if (!read_3d_input("Enter Vector V1 (X Y Z): ", p1_arr) ||
                !read_3d_input("Enter Vector V2 (X Y Z): ", p2_arr) ||
                !read_3d_input("Enter Point P (X Y Z): ", p3_arr)) break;
            
            v1 = create_vector_from_array(p1_arr);
            v2 = create_vector_from_array(p2_arr);
            Plain1 = getPlain2Vector(v1, v2, p3_arr); 
            if (Plain1) {
                printf("Plain Created. Orthogonal V (V1 x V2) = [%.2lf, %.2lf, %.2lf]\n",
                    Plain1->orthogonal_vector.direction[0], Plain1->orthogonal_vector.direction[1], Plain1->orthogonal_vector.direction[2]);
                free(Plain1);
            }
            break;

        case 3: {
            printf("--- NOTE: For this test, P1 will be stored in static memory. ---\n");
            double static_p1[3], static_p2[3], static_p3[3]; 
            if (!read_3d_input("Enter Point P1 (X Y Z): ", static_p1) ||
                !read_3d_input("Enter Point P2 (X Y Z): ", static_p2) ||
                !read_3d_input("Enter Point P3 (X Y Z): ", static_p3)) break;
            
            Plain1 = getPlain3point(static_p1, static_p2, static_p3); 
            if (Plain1) {
                printf("Plain Created. Orthogonal V = [%.2lf, %.2lf, %.2lf]\n",
                    Plain1->orthogonal_vector.direction[0], Plain1->orthogonal_vector.direction[1], Plain1->orthogonal_vector.direction[2]);
                free(Plain1);
            }
            break;
        }

        case 4:
            if (!read_3d_input("Enter Plain N Vector (X Y Z): ", p1_arr) ||
                !read_3d_input("Enter Plain Point P0 (X Y Z): ", p2_arr) ||
                !read_3d_input("Enter Point to Check P (X Y Z): ", check_p_arr)) break;
            
            Plain1 = getPlain(create_vector_from_array(p1_arr), p2_arr);
            if (checkPointInPlain(*Plain1, check_p_arr)) {
                printf("Result: The point IS IN the plain (Ax + By + Cz = D).\n");
            } else {
                printf("Result: The point IS NOT IN the plain.\n");
            }
            free(Plain1);
            break;

        case 5:
            if (!read_3d_input("Enter Plain N Vector (X Y Z): ", p1_arr) ||
                !read_3d_input("Enter Plain Point P0 (X Y Z): ", p2_arr) ||
                !read_3d_input("Enter Point P (X Y Z): ", check_p_arr)) break;
            
            Plain1 = getPlain(create_vector_from_array(p1_arr), p2_arr);
            double distance = distPointPlain(check_p_arr, *Plain1);
            printf("Distance: %.6lf\n", distance);
            free(Plain1);
            break;
        
        case 6:
            printf("--- Plain 1 ---\n");
            if (!read_3d_input("Enter N1 Vector (X Y Z): ", p1_arr) ||
                !read_3d_input("Enter P1 Point (X Y Z): ", p2_arr)) break;
            printf("--- Plain 2 ---\n");
            if (!read_3d_input("Enter N2 Vector (X Y Z): ", p3_arr) ||
                !read_3d_input("Enter P2 Point (X Y Z): ", check_p_arr)) break;
            
            Plain1 = getPlain(create_vector_from_array(p1_arr), p2_arr);
            Plain2 = getPlain(create_vector_from_array(p3_arr), check_p_arr);
            if (checkPlainParrallel(*Plain1, *Plain2)) {
                printf("Result: The Plains ARE parallel.\n");
            } else {
                printf("Result: The Plains ARE NOT parallel (they intersect).\n");
            }
            free(Plain1); free(Plain2);
            break;
            
        case 7:
            printf("Yet to be built");
            break;

        case 8:
            printf("--- CHECK POINT IN PLAIN (VECTORIC FORM) ---\n");
            printf("This function requires a plainVectoric struct (P0 + t*V1 + s*V2).\n");
            printf("Requires dedicated input function. Returning to menu.\n");
            break;

        case 0:
            return;
            
        default:
            printf("Invalid choice.\n");
    }

    pause_screen();
}

// --- Module: About ---
void display_about(void) {
    clear_screen();
    printf("=============================================\n");
    printf("|              About This Program           |\n");
    printf("=============================================\n\n");
    
    printf("Vector & Volume Calculator v1.2\n");
    printf("--------------------------------\n\n");
    
    printf("This program provides:\n");
    printf("  * Vector, Volume, and Plane Calculations\n");
    printf("  * Modular Arithmetic\n");
    printf("  * Base Conversion & Arithmetic (K < 11)\n");
    printf("  * Bitwise Operations\n");
    printf("  * Automated CSV-based testing\n\n");
    
    printf("Mathematical Operations:\n");
    printf("  Scalar Product:  V1 · V2 = x1*x2 + y1*y2 + z1*z2\n");
    printf("  Cross Product:   V1 x V2 = perpendicular vector\n");
    printf("  Volume:          |V1 · (V2 x V3)| / k\n\n");
    
    printf("Modules:\n");
    printf("  * mathUtil / vectorOps: Core vector/math operations\n");
    printf("  * modular: Modular arithmetic functions\n");
    printf("  * bitOperation: Base K arithmetic and bitwise ops\n");
    printf("  * csvHandler / testerFile: Testing framework\n\n");
    
    pause_screen();
}

// --- Module: Modular Operation ---
void modular_operation(void){
    int calc_choice;
    int numA, numB, numC;
    int tmp_cond; 

    clear_screen();
    printf("=============================================\n");
    printf("|          Modular Arithmetic Mode          |\n");
    printf("=============================================\n\n");
    printf("| 1. Calculate GCD (Greatest Common Divisor)  |\n");
    printf("| 2. Calculate LCM (Least Common Multiple)  |\n");
    printf("| 3. Get Modular Multiplicative Inverse (a^-1 mod n) |\n");
    printf("| 4. Modular Addition/Subtraction (a +/- b mod n) |\n");
    printf("| 5. Modular Multiplication/Division (a * b mod n) |\n");
    printf("| 6. Modular Exponentiation (b^p mod n) |\n");
    printf("| 7. Solve Linear Congruence (a*x = b mod n) |\n");
    printf("| 8. Check if a is divisible by b (General) |\n");
    printf("| 0. Cancel                                   |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (0-8): ");
    
    if (scanf("%d", &calc_choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');
    
    printf("\n--- Result ---\n");

    switch(calc_choice) {
        case 1:
            printf("Enter 2 numbers (A B): ");
            if (scanf("%d %d",&numA,&numB)!=2){
                printf("Invalid input.\n"); break;
            }
            printf("The GCD of %d and %d is %d\n", numA, numB, GCD(numA, numB));
            break;
        
        case 2:
            printf("Enter 2 numbers (A B): ");
            if (scanf("%d %d",&numA,&numB)!=2){
                printf("Invalid input.\n"); break;
            }
            if (numA == 0 || numB == 0) {
                printf("The LCM of %d and %d is 0 (since one or both are zero)\n", numA, numB);
            } else {
                long long product = (long long)numA * numB;
                int gcd = GCD(numA, numB);
                long long lcm = product / gcd;
                printf("The LCM of %d and %d is %lld\n", numA, numB, (lcm < 0) ? -lcm : lcm);
            }
            break;

        case 3:
            printf("Enter the number (A): ");
            if (scanf("%d",&numA)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter the modulo_n (N): ");
            if (scanf("%d",&numB)!=1){
                printf("Invalid input.\n"); break;
            }
            numC = invertible(numA, numB);
            if (numC == -1) {
                 printf("The inverse of %d in modulo %d does NOT exist (GCD is not 1).\n", numA, numB);
            } else {
                printf("The inverse of %d in modulo %d is %d\n", numA, numB, numC);
            }
            break;

        case 4:
            printf("Enter two numbers for the operation (A B): ");
            if (scanf("%d %d",&numA, &numB)!=2){
                printf("Invalid input.\n"); break;
            }
            printf("Enter the modulo_n (N): ");
            if (scanf("%d",&numC)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter 1 for addition (+) or 0 for subtraction (-): ");
            if (scanf("%d",&tmp_cond)!=1){
                printf("Invalid input.\n"); break;
            }
            if (tmp_cond == 1){
                int result = additionMudolar(numA, numB, numC, true);
                printf("The sum (%d + %d) in modulo %d is %d\n", numA, numB, numC, result);
            } else if (tmp_cond == 0){
                int result = additionMudolar(numA, numB, numC, false);
                printf("The difference (%d - %d) in modulo %d is %d\n", numA, numB, numC, result);
            } else {
                printf("Invalid input for operation type.\n");
            }
            break;

        case 5:
            printf("Enter two numbers for the operation (A B): ");
            if (scanf("%d %d",&numA, &numB)!=2){
                printf("Invalid input.\n"); break;
            }
            printf("Enter the modulo_n (N): ");
            if (scanf("%d",&numC)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter 1 for multiplication (*) or 0 for division (/): ");
            if (scanf("%d",&tmp_cond)!=1){
                printf("Invalid input.\n"); break;
            }
            if (tmp_cond == 1){
                int result = multiplicationModular(numA, numB, numC, false);
                printf("The product (%d * %d) in modulo %d is %d\n", numA, numB, numC, result);
            } else if (tmp_cond == 0){
                int result = multiplicationModular(numA, numB, numC, true);
                if (result == -1) {
                    printf("The modular division (%d / %d) in modulo %d FAILED (Inverse of %d does not exist).\n", numA, numB, numC, numB);
                } else {
                    printf("The quotient (%d / %d) in modulo %d is %d\n", numA, numB, numC, result);
                }
            } else {
                printf("Invalid input for operation type.\n");
            }
            break;
            
        case 6:
            printf("Enter the base (B): ");
            if (scanf("%d",&numA)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter the exponent (P, must be integer): ");
            if (scanf("%d",&numB)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter the modulo_n (N): ");
            if (scanf("%d",&numC)!=1){
                printf("Invalid input.\n"); break;
            }
            int result_exp = exponantialModular(numA, numB, numC);
            if (result_exp == -1) {
                printf("Error: Invalid input for modular exponentiation (N<=0 or P<0).\n");
            } else {
                printf("The result of (%d^%d) mod %d is %d\n", numA, numB, numC, result_exp);
            }
            break;
            
        case 7:
            printf("Solving A*X = B (mod N)\n");
            printf("Enter A: ");
            if (scanf("%d",&numA)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter B: ");
            if (scanf("%d",&numB)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter N (modulo): ");
            if (scanf("%d",&numC)!=1){
                printf("Invalid input.\n"); break;
            }
            int result_x = findTheX(numA, numB, numC);
            if (result_x == -1) {
                printf("No unique solution X exists for %d*X = %d (mod %d) (A must be coprime to N).\n", numA, numB, numC);
            } else {
                printf("A solution is X = %d (since %d * %d = %d mod %d)\n", result_x, numA, result_x, multiplicationModular(numA, result_x, numC, false), numC);
            }
            break;

        case 8:
            printf("Checking if A is divisible by B.\n");
            printf("Enter A (dividend): ");
            if (scanf("%d",&numA)!=1){
                printf("Invalid input.\n"); break;
            }
            printf("Enter B (divisor): ");
            if (scanf("%d",&numB)!=1){
                printf("Invalid input.\n"); break;
            }
            int quotient = dividableBy(numA, numB);
            if (quotient != -1) {
                printf("%d IS divisible by %d. The quotient is %d.\n", numA, numB, quotient);
            } else {
                printf("%d IS NOT evenly divisible by %d.\n", numA, numB);
            }
            break;

        case 0:
            return;
            
        default:
            printf("Invalid choice.\n");
    }

    pause_screen();
}

// --- Module: Bit and Base Operations ---
void bit_operation_menu(void) {
    int op_choice;
    int num1, num2, base, base_old;
    
    clear_screen();
    printf("=============================================\n");
    printf("|         Bit & Base Operations Mode         |\n");
    printf("|    (Base K Arithmetic assumes K < 11)     |\n");
    printf("=============================================\n\n");
    printf("| 1. Convert Base 10 Integer to Base K      |\n");
    printf("| 2. Base K Addition                        |\n");
    printf("| 3. Base K Multiplication                  |\n");
    printf("| 4. Base K Subtraction                     |\n");
    printf("| 5. Bitwise AND (&)                        |\n");
    printf("| 6. Bitwise OR (|)                         |\n");
    printf("| 7. Hex/Octal String to Binary String      |\n");
    printf("| 0. Cancel                                 |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (0-7): ");

    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');
    
    printf("\n--- Result ---\n");

    switch(op_choice) {
        case 1:
            printf("Enter Base 10 number: ");
            if (scanf("%d", &num1) != 1) { printf("Invalid input.\n"); break; }
            printf("Enter target Base K (2-10): ");
            if (scanf("%d", &base) != 1) { printf("Invalid input.\n"); break; }
            printf("Result in Base %d: %d\n", base, convertBase(num1, base));
            break;

        case 2:
            printf("Enter two numbers in Base K representation (A B): ");
            if (scanf("%d %d", &num1, &num2) != 2) { printf("Invalid input.\n"); break; }
            printf("Enter Base K (2-10): ");
            if (scanf("%d", &base) != 1) { printf("Invalid input.\n"); break; }
            printf("(%d + %d) in Base %d = %d\n", num1, num2, base, additionAnyBase(num1, num2, base, 0));
            break;
            
        case 3:
            printf("Enter two numbers in Base K representation (A B): ");
            if (scanf("%d %d", &num1, &num2) != 2) { printf("Invalid input.\n"); break; }
            printf("Enter Base K (2-10): ");
            if (scanf("%d", &base) != 1) { printf("Invalid input.\n"); break; }
            printf("(%d * %d) in Base %d = %d\n", num1, num2, base, multiplicationAnyBase(num1, num2, base));
            break;

        case 4:
            printf("Enter two numbers in Base K representation (A B, where A >= B): ");
            if (scanf("%d %d", &num1, &num2) != 2) { printf("Invalid input.\n"); break; }
            printf("Enter Base K (2-10): ");
            if (scanf("%d", &base) != 1) { printf("Invalid input.\n"); break; }
            printf("(%d - %d) in Base %d = %d\n", num1, num2, base, subtractionAnyBase(num1, num2, base));
            break;
            
        case 5:
            printf("Enter two Base 10 integers (A B): ");
            if (scanf("%d %d", &num1, &num2) != 2) { printf("Invalid input.\n"); break; }
            printf("(%d & %d) = %d\n", num1, num2, bitwiseAND(num1, num2));
            break;

        case 6:
            printf("Enter two Base 10 integers (A B): ");
            if (scanf("%d %d", &num1, &num2) != 2) { printf("Invalid input.\n"); break; }
            printf("(%d | %d) = %d\n", num1, num2, bitwiseOR(num1, num2));
            break;
            
        case 7: {
            char input_str[256];
            printf("Enter Hex or Octal number string: ");
            scanf("%s", input_str);
            printf("Enter original Base (8 or 16): ");
            if (scanf("%d", &base_old) != 1) { printf("Invalid input.\n"); break; }
            
            char* binary_result = convertToBase2(input_str, base_old);
            if (binary_result) {
                printf("(%s) in Base %d = (%s) in Base 2\n", input_str, base_old, binary_result);
                free(binary_result);
            } else {
                 printf("Conversion failed. Check input and base.\n");
            }
            break;
        }

        case 0:
            return;
            
        default:
            printf("Invalid choice.\n");
    }

    while (getchar() != '\n');
    pause_screen();
}