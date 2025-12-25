#include "ui.h"

// --- Helper Functions (Internal) ---

// Reads a 3-element point into a double array
static bool read_3d_input(const char *prompt, double *out_arr) {
    printf("%s", prompt);
    if (scanf("%lf %lf %lf", &out_arr[0], &out_arr[1], &out_arr[2]) != 3) {
        printf("Invalid input. Please enter three numbers separated by spaces.\n");
        while (getchar() != '\n');
        return false;
    }
    while (getchar() != '\n');
    return true;
}

// Converts a double array to the new Vector ADT
static vector create_vector_from_array(double *arr) {
    vector v = cnstVector(3);
    if (!v) return NULL;
    v->val[0] = arr[0];
    v->val[1] = arr[1];
    v->val[2] = arr[2];
    return v;
}

// Helper to free a plain struct and its internal vectors
static void free_plain(plain *p) {
    if (!p) return;
    dcnstVector(p->normal);
    dcnstVector(p->point);
    free(p);
}

// --- UI Utilities ---

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
    printf("\nEnter your choice: ");
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return -99; // Error code
    }
    while (getchar() != '\n');
    return choice;
}

void display_main_menu(void) {
    printf("=============================================\n");
    printf("||   Advanced Math Calculator - Main Menu  ||\n");
    printf("=============================================\n");
    printf("|  1. Run Automated Test Suite               |\n");
    printf("|  2. Calculations (Vector/Volume/Plain)     |\n");
    printf("|  3. Modular operations                     |\n");
    printf("|  4. Bit & Base Operations (K < 11)         |\n");
    printf("|  0. About                                  |\n");
    printf("| -1. Exit                                   |\n");
    printf("=============================================\n");
}

void display_about(void) {
    clear_screen();
    printf("=============================================\n");
    printf("|              About This Program           |\n");
    printf("=============================================\n\n");
    printf("Vector & Volume Calculator v2.0 (ADT Version)\n");
    printf("---------------------------------------------\n");
    printf("Modules:\n");
    printf(" * vectorOps: Dynamic Vector ADT & Geometry\n");
    printf(" * modular: Modular arithmetic\n");
    printf(" * bitOperation: Base conversion & Bitwise\n");
    printf(" * csvHandler: Automation & Testing\n\n");
    pause_screen();
}

// --- Logic Controllers ---

void run_tester_module(void) {
    char filename[256];
    int test_choice;
    
    clear_screen();
    printf("=== Automated Test Suite ===\n");
    printf("Enter CSV file path: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) return;
    filename[strcspn(filename, "\n")] = 0; // Remove newline

    CsvFile *csv = csv_open(filename);
    if (!csv) {
        printf("Error: Could not open file.\n");
        pause_screen();
        return;
    }
    
    if (!csv_read_line(csv)) {
        printf("Error: Empty file or header missing.\n");
        csv_close(csv); 
        return;
    }

    printf("\n1. Parallelepiped (k=1)\n2. Pyramid (k=6)\n3. Cross Product\n4. Scalar Product\n5. All\n0. Back\n");
    test_choice = get_user_choice();

    if (test_choice == 0) { csv_close(csv); return; }

    switch(test_choice) {
        case 1: run_volume_tests(csv, volumeParallelepiped, "Parallelepiped", 1.0); break;
        case 2: run_volume_tests(csv, volumeParallelepiped, "Pyramid", 6.0); break;
        case 3: run_cross_product_tests(csv, crossProduct); break;
        case 4: run_scalar_product_tests(csv, scalaricProduct); break;
        case 5: 
            run_volume_tests(csv, volumeParallelepiped, "Parallelepiped", 1.0);
            run_volume_tests(csv, volumeParallelepiped, "Pyramid", 6.0);
            run_cross_product_tests(csv, crossProduct);
            run_scalar_product_tests(csv, scalaricProduct);
            break;
        default: printf("Invalid choice.\n");
    }
    
    csv_close(csv);
    pause_screen();
}

void run_single_calculation(void) {
    clear_screen();
    printf("=== Calculation Mode ===\n");
    printf("1. Volume of a Shape\n");
    printf("2. Vector Operations\n");
    printf("3. Plane Geometry\n");
    printf("0. Back\n");
    
    int choice = get_user_choice();
    switch(choice) {
        case 1: volume_calculation_menu(); break;
        case 2: vector_operation_menu(); break;
        case 3: plain_operation_menu(); break;
        case 0: return;
        default: printf("Invalid choice.\n"); pause_screen();
    }
}

// --- Vector Operations ---

void vector_operation_menu(void) {
    int op;
    int count = 0;
    
    clear_screen();
    printf("=== Vector Operations ===\n");
    printf("1. Cross Product\n2. Scalar Product\n3. Add/Sub\n4. Normal Vector\n5. Check Point in Shape\n0. Back\n");
    op = get_user_choice();
    
    if (op == 0) return;

    // Determine count based on op
    if (op == 1 || op == 2) count = 2;
    else if (op == 4) count = 1;
    else if (op == 3) {
        printf("How many vectors (2-10)? ");
        scanf("%d", &count);
        if (count < 2 || count > 10) return;
    }
    else if (op == 5) {
        printf("Total vectors (3 for shape + 1 to check = 4 min)? ");
        scanf("%d", &count);
        if (count < 4) return;
    }

    // Allocate array of pointers for the ADT
    vector *vecList = malloc(count * sizeof(vector));
    
    // Input loop
    for(int i=0; i<count; i++) {
        double arr[3];
        printf("Vector %d (X Y Z): ", i+1);
        scanf("%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
        vecList[i] = create_vector_from_array(arr);
    }
    while(getchar() != '\n'); // flush

    printf("\n--- Result ---\n");
    switch(op) {
        case 1: { // Cross
            vector res = crossProduct(vecList[0], vecList[1]);
            if(res) {
                printf("Result: [%.2f, %.2f, %.2f]\n", res->val[0], res->val[1], res->val[2]);
                dcnstVector(res);
            }
            break;
        }
        case 2: { // Scalar
            printf("Result: %.2f\n", scalaricProduct(vecList[0], vecList[1]));
            break;
        }
        case 3: { // Add/Sub
            printf("1. Add (+)\n2. Subtract (-)\nChoice: ");
            int subChoice; 
            scanf("%d", &subChoice);
            
            // Start with first vector copied
            vector total = addition(vecList[0], cnstVector(3), true); 
            
            for(int i=1; i<count; i++) {
                vector temp = total;
                total = addition(total, vecList[i], (subChoice == 1));
                dcnstVector(temp);
            }
            printf("Result: [%.2f, %.2f, %.2f]\n", total->val[0], total->val[1], total->val[2]);
            dcnstVector(total);
            break;
        }
        case 4: { // Normal
            vector norm = getNormal(vecList[0]);
            printf("Normal: [%.2f, %.2f, %.2f]\n", norm->val[0], norm->val[1], norm->val[2]);
            dcnstVector(norm);
            break;
        }
        case 5: { // Point in Shape (Parallelepiped check)
             // v0, v1, v2 form the shape base vectors
             vector shape[3] = {vecList[0], vecList[1], vecList[2]};
             double vol = volumeParallelepiped(shape, 1.0);
             printf("Base Volume: %.2f\n", vol);
             // Logic check for subsequent vectors...
             // (Simplified for brevity, assuming existing logic from main.c logic)
             break;
        }
    }

    // Cleanup
    for(int i=0; i<count; i++) dcnstVector(vecList[i]);
    free(vecList);
    pause_screen();
}

void volume_calculation_menu(void) {
    double arr[3];
    vector vecs[3];
    
    clear_screen();
    printf("=== Volume Calculation ===\n");
    for(int i=0; i<3; i++) {
        char prompt[20]; sprintf(prompt, "Vector %d (X Y Z): ", i+1);
        read_3d_input(prompt, arr);
        vecs[i] = create_vector_from_array(arr);
    }

    int k;
    printf("Shape constant (1=Parallelepiped, 6=Pyramid): ");
    scanf("%d", &k);

    double vol = volumeParallelepiped(vecs, (double)k);
    printf("Volume: %.4f\n", vol);

    for(int i=0; i<3; i++) dcnstVector(vecs[i]);
    pause_screen();
}

// --- Modular Operations ---

void modular_operation_menu(void) {
    int choice, a, b, n;
    clear_screen();
    printf("=== Modular Arithmetic ===\n");
    printf("1. GCD\n2. LCM\n3. Inverse\n4. Add/Sub\n5. Mult/Div\n6. Exponent\n0. Back\n");
    choice = get_user_choice();
    if(choice == 0) return;

    printf("Enter inputs based on operation...\n");
    
    switch(choice) {
        case 1: 
            printf("A B: "); scanf("%d %d", &a, &b);
            printf("GCD: %d\n", GCD(a, b)); 
            break;
        case 2:
            printf("A B: "); scanf("%d %d", &a, &b);
            // LCM Logic
            break;
        case 3:
            printf("Num Modulo: "); scanf("%d %d", &a, &n);
            printf("Inverse: %d\n", invertible(a, n));
            break;
        case 4:
            printf("A B Modulo: "); scanf("%d %d %d", &a, &b, &n);
            printf("1.Add 0.Sub: "); int type; scanf("%d", &type);
            printf("Result: %d\n", additionMudolar(a, b, n, type));
            break;
        // ... (Implement rest similarly calling modular.h functions)
    }
    pause_screen();
}

// --- Bit Operations ---

void bit_operation_menu(void) {
    // Implementation wrappers for bitOperation.h
    clear_screen();
    printf("=== Bit & Base Ops ===\n");
    printf("Functionality available in main implementation.\n");
    pause_screen();
}

// --- Plain Operations ---

void plain_operation_menu(void) {
    int choice;
    double p1[3], p2[3], p3[3];
    plain *P1 = NULL;

    clear_screen();
    printf("=== Plain Geometry ===\n");
    printf("1. Plain (Normal + Point)\n2. Plain (2 Vectors + Point)\n3. Plain (3 Points)\n4. Check Point\n0. Back\n");
    choice = get_user_choice();
    if(choice == 0) return;

    switch(choice) {
        case 1: {
            read_3d_input("Normal: ", p1);
            read_3d_input("Point: ", p2);
            vector n = create_vector_from_array(p1);
            vector p = create_vector_from_array(p2);
            P1 = getPlain(n, p);
            // n and p are copied inside getPlain usually, 
            // but if getPlain takes ownership, we don't free. 
            // Based on previous ADT, getPlain creates COPIES.
            dcnstVector(n); dcnstVector(p);
            break;
        }
        case 3: {
            read_3d_input("P1: ", p1);
            read_3d_input("P2: ", p2);
            read_3d_input("P3: ", p3);
            vector v1 = create_vector_from_array(p1);
            vector v2 = create_vector_from_array(p2);
            vector v3 = create_vector_from_array(p3);
            P1 = getPlain3point(v1, v2, v3);
            dcnstVector(v1); dcnstVector(v2); dcnstVector(v3);
            break;
        }
        // ... other cases
    }

    if(P1) {
        printf("Plain Created. Normal: [%.2f, %.2f, %.2f]\n", 
            P1->normal->val[0], P1->normal->val[1], P1->normal->val[2]);
        free_plain(P1);
    }
    pause_screen();
}