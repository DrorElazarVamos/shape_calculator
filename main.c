#include "UI.h" 

int main() {
    int choice;
    bool running = true;

    while (running) {
        clear_screen();
        display_main_menu();
        choice = get_user_choice();

        switch (choice) {
            case -1:
                printf("\nExiting...\n");
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
                modular_operation_menu();
                break;
            case 4: 
                bit_operation_menu();
                break;
            default:
                printf("\nInvalid choice.\n");
                pause_screen();
        }
    }
    return 0;
}