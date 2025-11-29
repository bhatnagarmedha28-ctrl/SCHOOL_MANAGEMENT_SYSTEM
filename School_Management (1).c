#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // For using bool type

// --- CONSTANTS ---
#define DATA_FILE "student_records.dat"
#define NAME_LENGTH 50
#define ADDRESS_LENGTH 100
#define COURSE_LENGTH 30

// --- STRUCTURE DEFINITION ---
// This structure holds all the essential details for a single student.
struct Student {
    int roll_number;
    char name[NAME_LENGTH];
    char student_class[COURSE_LENGTH]; // e.g., "10th Grade"
    char address[ADDRESS_LENGTH];
    float total_score;
    bool fee_paid; // 1 for Paid, 0 for Not Paid
};

// --- FUNCTION PROTOTYPES (Best Practice in C) ---
// Declaring all functions used in the program before main()
void show_menu();
void add_student();
void view_all_students();
void search_student();
void modify_student();
void delete_student();

// Utility functions
void clear_screen();
void clear_input_buffer();
void print_student_data(struct Student s);

// --- MAIN FUNCTION ---
int main() {
    int choice;

    // A loop to continuously display the menu until the user chooses to exit.
    do {
        show_menu();
        printf("Enter your choice: ");

        // Check for valid input
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer(); // Clear buffer on invalid input
            choice = 0; // Set choice to 0 to trigger the default case
        }
        clear_input_buffer();

        // Use a switch statement to handle the user's choice
        switch (choice) {
            case 1:
                add_student();
                break;
            case 2:
                view_all_students();
                break;
            case 3:
                search_student();
                break;
            case 4:
                modify_student();
                break;
            case 5:
                delete_student();
                break;
            case 6:
                printf("\n============================================\n");
                printf("  Exiting the School Management System. Goodbye!\n");
                printf("============================================\n");
                break;
            default:
                printf("\n[ERROR] Invalid choice. Please enter a number between 1 and 6.\n");
                // Pause execution
                printf("Press Enter to continue...");
                getchar();
        }
    } while (choice != 6);

    return 0;
}

// --- UTILITY FUNCTIONS ---

// Function to clear the screen
void clear_screen() {
    // Check if the OS is Windows or Unix-like (Linux/macOS)
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to clear the input buffer (crucial after using scanf and before fgets)
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to display the main menu
void show_menu() {
    clear_screen();
    printf("\n============================================\n");
    printf("     SCHOOL MANAGEMENT SYSTEM (C Project)     \n");
    printf("============================================\n");
    printf("1. Add New Student Record\n");
    printf("2. View All Student Records\n");
    printf("3. Search Student by Roll Number\n");
    printf("4. Modify Student Record\n");
    printf("5. Delete Student Record\n");
    printf("6. Exit Program\n");
    printf("============================================\n");
}

// Function to print a student's data in a formatted way
void print_student_data(struct Student s) {
    printf("\n--------------------------------------------\n");
    printf("   STUDENT DETAILS (Roll No: %d)\n", s.roll_number);
    printf("--------------------------------------------\n");
    printf("Name:             %s\n", s.name);
    printf("Class:            %s\n", s.student_class);
    printf("Address:          %s\n", s.address);
    printf("Total Score:      %.2f\n", s.total_score);
    printf("Fee Status:       %s\n", s.fee_paid ? "PAID" : "NOT PAID");
    printf("--------------------------------------------\n");
}

// --- CORE FUNCTIONALITIES ---

// 1. Add New Student Record
void add_student() {
    clear_screen();
    printf("============================================\n");
    printf("        ADD NEW STUDENT RECORD\n");
    printf("============================================\n");

    FILE *file = fopen(DATA_FILE, "ab"); // Open file in append binary mode
    if (file == NULL) {
        printf("[ERROR] Could not open file %s for writing.\n", DATA_FILE);
        return;
    }

    struct Student new_student;

    // Get Roll Number
    printf("Enter Roll Number (Integer): ");
    while (scanf("%d", &new_student.roll_number) != 1 || new_student.roll_number <= 0) {
        clear_input_buffer();
        printf("[ERROR] Invalid Roll Number. Please enter a positive integer: ");
    }
    clear_input_buffer();

    // Get Name
    printf("Enter Student Name: ");
    fgets(new_student.name, NAME_LENGTH, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = 0; // Remove newline

    // Get Class
    printf("Enter Class/Grade (e.g., 10th Grade): ");
    fgets(new_student.student_class, COURSE_LENGTH, stdin);
    new_student.student_class[strcspn(new_student.student_class, "\n")] = 0; // Remove newline

    // Get Score
    printf("Enter Total Score (e.g., 450.75): ");
    while (scanf("%f", &new_student.total_score) != 1 || new_student.total_score < 0) {
        clear_input_buffer();
        printf("[ERROR] Invalid score. Please enter a non-negative number: ");
    }
    clear_input_buffer();

    // Get Fee Status
    int fee_choice;
    printf("Fee Status (1=Paid, 0=Not Paid): ");
    while (scanf("%d", &fee_choice) != 1 || (fee_choice != 0 && fee_choice != 1)) {
        clear_input_buffer();
        printf("[ERROR] Invalid input. Enter 1 for Paid or 0 for Not Paid: ");
    }
    new_student.fee_paid = (bool)fee_choice;
    clear_input_buffer();

    // Get Address (optional detail)
    printf("Enter Address: ");
    fgets(new_student.address, ADDRESS_LENGTH, stdin);
    new_student.address[strcspn(new_student.address, "\n")] = 0; // Remove newline

    // Write the complete structure to the binary file
    fwrite(&new_student, sizeof(struct Student), 1, file);
    fclose(file);

    printf("\n[SUCCESS] Student record for %s (Roll No: %d) added successfully!\n",
           new_student.name, new_student.roll_number);

    printf("Press Enter to continue...");
    getchar();
}

// 2. View All Student Records
void view_all_students() {
    clear_screen();
    printf("============================================\n");
    printf("        ALL STUDENT RECORDS\n");
    printf("============================================\n");

    FILE *file = fopen(DATA_FILE, "rb"); // Open file in read binary mode
    if (file == NULL) {
        printf("[INFO] No student records found. The data file may not exist yet.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct Student current_student;
    int count = 0;

    // Read all records from the file until EOF
    while (fread(&current_student, sizeof(struct Student), 1, file) == 1) {
        print_student_data(current_student);
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("[INFO] The file is empty. No student records to display.\n");
    } else {
        printf("\nTotal records found: %d\n", count);
    }

    printf("Press Enter to continue...");
    getchar();
}

// 3. Search Student by Roll Number
void search_student() {
    clear_screen();
    printf("============================================\n");
    printf("        SEARCH STUDENT BY ROLL NUMBER\n");
    printf("============================================\n");

    int search_roll;
    printf("Enter Roll Number to search: ");
    if (scanf("%d", &search_roll) != 1) {
        clear_input_buffer();
        printf("[ERROR] Invalid input for Roll Number.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    clear_input_buffer(); // Clear buffer after scanf

    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) {
        printf("[INFO] No records exist to search.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct Student current_student;
    int found = 0;

    // Iterate through the file record by record
    while (fread(&current_student, sizeof(struct Student), 1, file) == 1) {
        if (current_student.roll_number == search_roll) {
            print_student_data(current_student);
            found = 1;
            break; // Found the student, no need to continue reading
        }
    }

    fclose(file);

    if (!found) {
        printf("\n[INFO] Student with Roll Number %d not found.\n", search_roll);
    }

    printf("Press Enter to continue...");
    getchar();
}

// 4. Modify Student Record
void modify_student() {
    clear_screen();
    printf("============================================\n");
    printf("        MODIFY STUDENT RECORD\n");
    printf("============================================\n");

    int modify_roll;
    printf("Enter Roll Number of the student to modify: ");
    if (scanf("%d", &modify_roll) != 1) {
        clear_input_buffer();
        printf("[ERROR] Invalid input for Roll Number.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    clear_input_buffer();

    // Open file in read/write binary mode (r+b)
    FILE *file = fopen(DATA_FILE, "r+b");
    if (file == NULL) {
        printf("[INFO] No records exist to modify.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct Student current_student;
    long record_size = sizeof(struct Student);
    int found = 0;

    // Loop through the file
    while (fread(&current_student, record_size, 1, file) == 1) {
        if (current_student.roll_number == modify_roll) {
            found = 1;
            printf("\n[FOUND] Record found. Displaying current details:\n");
            print_student_data(current_student);

            printf("\nEnter new Total Score: ");
            while (scanf("%f", &current_student.total_score) != 1 || current_student.total_score < 0) {
                clear_input_buffer();
                printf("[ERROR] Invalid score. Please enter a non-negative number: ");
            }
            clear_input_buffer();

            int fee_choice;
            printf("Enter new Fee Status (1=Paid, 0=Not Paid): ");
            while (scanf("%d", &fee_choice) != 1 || (fee_choice != 0 && fee_choice != 1)) {
                clear_input_buffer();
                printf("[ERROR] Invalid input. Enter 1 for Paid or 0 for Not Paid: ");
            }
            current_student.fee_paid = (bool)fee_choice;
            clear_input_buffer();

            // Move the file pointer back to the beginning of the record we just read
            fseek(file, -record_size, SEEK_CUR);

            // Overwrite the old record with the new, modified data
            fwrite(&current_student, record_size, 1, file);

            printf("\n[SUCCESS] Record for Roll No %d updated successfully!\n", modify_roll);
            break; // Modification complete
        }
    }

    fclose(file);

    if (!found) {
        printf("\n[INFO] Student with Roll Number %d not found.\n", modify_roll);
    }

    printf("Press Enter to continue...");
    getchar();
}

// 5. Delete Student Record
void delete_student() {
    clear_screen();
    printf("============================================\n");
    printf("        DELETE STUDENT RECORD\n");
    printf("============================================\n");

    int delete_roll;
    printf("Enter Roll Number of the student to delete: ");
    if (scanf("%d", &delete_roll) != 1) {
        clear_input_buffer();
        printf("[ERROR] Invalid input for Roll Number.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    clear_input_buffer();

    // Open original file for reading (rb) and a temporary file for writing (wb)
    FILE *original_file = fopen(DATA_FILE, "rb");
    FILE *temp_file = fopen("temp_records.dat", "wb");

    if (original_file == NULL || temp_file == NULL) {
        printf("[INFO] No records exist to delete or cannot create temporary file.\n");
        if (original_file) fclose(original_file);
        if (temp_file) fclose(temp_file);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct Student current_student;
    int found = 0;

    // Copy all records *except* the one to be deleted to the temporary file
    while (fread(&current_student, sizeof(struct Student), 1, original_file) == 1) {
        if (current_student.roll_number != delete_roll) {
            // Write to temp file if it's NOT the record we want to delete
            fwrite(&current_student, sizeof(struct Student), 1, temp_file);
        } else {
            // This is the record to be deleted
            found = 1;
        }
    }

    // Close both files
    fclose(original_file);
    fclose(temp_file);

    if (found) {
        // Delete the original file
        if (remove(DATA_FILE) != 0) {
            printf("[ERROR] Could not delete the original data file.\n");
        } else {
            // Rename the temporary file to the original file name
            if (rename("temp_records.dat", DATA_FILE) != 0) {
                printf("[ERROR] Could not rename the temporary file.\n");
            } else {
                printf("\n[SUCCESS] Student record for Roll No %d deleted successfully.\n", delete_roll);
            }
        }
    } else {
        // Clean up the temp file if no record was found
        remove("temp_records.dat");
        printf("\n[INFO] Student with Roll Number %d not found. No records deleted.\n", delete_roll);
    }

    printf("Press Enter to continue...");
    getchar();
}