#include "csvHandler.h"
#include <math.h> 

// Helper function to read a single vector from the current line's tokens
static bool read_single_vector(CsvFile *csv, vector *v_out) {
    char *field_str;

    if (!csv || !v_out) return false;

    // 1. Read X component
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    v_out->direction[0] = atof(field_str);

    // 2. Read Y component
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    v_out->direction[1] = atof(field_str);

    // 3. Read Z component
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    v_out->direction[2] = atof(field_str);

    // 4. Read Magnitude
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    v_out->magnitude = atof(field_str);

    return true;
}

// --- Core CSV Function Implementations ---

CsvFile* csv_open(const char *filename) {
    CsvFile *csv = (CsvFile*)malloc(sizeof(CsvFile));
    if (csv == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    csv->file_ptr =  fopen(filename, "r");
    if (csv->file_ptr == NULL) {
        perror("Error opening CSV file");
        free(csv); 
        return NULL;
    }

    csv->current_line_number = 0;

    return csv;
}

bool csv_read_line(CsvFile *csv) {
    if (fgets(csv->line_buffer, MAX_LINE_LENGTH, csv->file_ptr) != NULL) {
        csv->line_buffer[strcspn(csv->line_buffer, "\n")] = 0; 
        csv->current_line_number++;
        return true;
    }
    return false;
}

char* csv_get_field(CsvFile *csv) {
    static int last_line_number = -1;
    
    // If we're on a new line, reinitialize strtok
    if (last_line_number != csv->current_line_number) {
        last_line_number = csv->current_line_number;
        return strtok(csv->line_buffer, ",");
    }
    
    // Continue tokenizing the current line
    return strtok(NULL, ",");
}

void csv_close(CsvFile *csv) {
    if (csv == NULL) return;
    if (csv->file_ptr != NULL) {
        fclose(csv->file_ptr);
    }
    free(csv);
}

// --- Vector List Functionality Implementation (Used to satisfy linkage, but not by main) ---

VectorList csv_read_vector_list(const char *filename) {
    VectorList list = { .vectors = NULL, .count = 0 };
    CsvFile *file = csv_open(filename);

    if (file == NULL) return list;

    csv_read_line(file); // Skip header 

    while (csv_read_line(file)) {
        vector current_vector;
        
        // Attempt to read the FIRST vector (4 fields)
        if (read_single_vector(file, &current_vector)) {
            
            // Skip the remaining 9 fields (V2, V3, and EXPECTED_VOLUME)
            for (int i = 0; i < 9; i++) {
                if (csv_get_field(file) == NULL) break;
            }
            
            list.count++;
            vector *new_vectors = realloc(list.vectors, list.count * sizeof(vector));
            
            if (new_vectors == NULL) {
                fprintf(stderr, "Error: Memory reallocation failed.\n");
                free_vector_list(&list);
                csv_close(file);
                return (VectorList){ .vectors = NULL, .count = 0 };
            }
            
            list.vectors = new_vectors;
            list.vectors[list.count - 1] = current_vector;

        } else {
            fprintf(stderr, "Warning: Skipping badly formatted line (could not read V1 data).\n");
        }
    }

    csv_close(file);
    return list;
}