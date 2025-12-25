#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// --- Custom Library Dependencies ---
#include "vectorOps.h"
#include "csvHandler.h"
#include "testerFile.h"
#include "modular.h"      // Assumed existing module

// --- UI Utilities ---
void clear_screen(void);
void pause_screen(void);
int get_user_choice(void);

// --- Menus & Displays ---
void display_main_menu(void);
void display_about(void);

// --- Logic Controllers ---
void run_tester_module(void);
void run_single_calculation(void);

// --- Sub-Modules ---
void vector_operation_menu(void);
void volume_calculation_menu(void);
void modular_operation_menu(void);
void bit_operation_menu(void);
void plain_operation_menu(void);

#endif // UI_H