#include "csvHandler.h"

// Helper function to read a single vector from the current line's tokens
// Note: Expects 'v_out' to be already allocated with dimension 3
static bool read_single_vector(CsvFile *csv, vector v_out) {
    char *field_str;

    if (!csv || !v_out || v_out->dim < 3) return false;

    // 1. Read X component
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    v_out->val[0] = atof(field_str);

    // 2. Read Y component
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    v_out->val[1] = atof(field_str);

    // 3. Read Z component
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    v_out->val[2] = atof(field_str);

    // 4. Read Magnitude (Ignored in new struct, but must be consumed from CSV)
    field_str = csv_get_field(csv);
    if (field_str == NULL) return false;
    // We do nothing with magnitude as it is calculated on demand in the new ADT

    return true;
}

// --- Core CSV Function Implementations ---

CsvFile* csv_open(const char *filename) {
    CsvFile *csv = (CsvFile*)malloc(sizeof(CsvFile));
    if (csv == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    csv->file_ptr = fopen(filename, "r");
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

// --- Vector Set Functionality Implementation ---

vectorSet csv_read_vector_set(const char *filename) {
    // 1. Create the Set
    vectorSet set = cnstVectorSet();
    if (!set) return NULL;

    CsvFile *file = csv_open(filename);
    if (file == NULL) {
        // Return empty set if file fails, or NULL depending on preference
        return set; 
    }

    csv_read_line(file); // Skip header 

    while (csv_read_line(file)) {
        // 2. Construct a new vector for this line
        vector current_vector = cnstVector(3); // 3D vector
        if (!current_vector) break; // Memory fail

        // Attempt to read the FIRST vector (4 fields: x, y, z, mag)
        if (read_single_vector(file, current_vector)) {
            
            // Skip the remaining 9 fields (V2, V3, and EXPECTED_VOLUME) if they exist
            // (Based on legacy logic)
            for (int i = 0; i < 9; i++) {
                if (csv_get_field(file) == NULL) break;
            }
            
            // 3. Add to Set
            addToSet(set, current_vector);

        } else {
            // If reading failed, free the unused vector
            dcnstVector(current_vector);
            fprintf(stderr, "Warning: Skipping badly formatted line %d.\n", file->current_line_number);
        }
    }

    csv_close(file);
    return set;
}