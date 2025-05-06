#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>   // WinApi header

HANDLE  hConsole;
int colorattribute = 0; // color attribute

#define yellow_zone 1
#define red_zone 1.5
#define green_zone 2

// Function to validate a number plate
int validateNumberPlate(const char *plate) {
    // Check if the plate length is exactly 7 characters
    if (strlen(plate) != 7) {
        return 0; // Invalid
    }

    // Check the format [A-Z A-Z 0-9 0-9 A-Z A-Z A-Z]
    if (isupper(plate[0]) && isupper(plate[1]) && // First two characters are uppercase letters
        isdigit(plate[2]) && isdigit(plate[3]) && // Next two characters are digits
        isupper(plate[4]) && isupper(plate[5]) && isupper(plate[6])) { // Last three characters are uppercase letters
        return 1; // Valid
    }

    return 0; // Invalid
}

// Function to validate a zone
int validateZone(const char *zone) {
    // Check if the zone is one of the valid options
    if (strcmp(zone, "yellow") == 0 || strcmp(zone, "red") == 0 || strcmp(zone, "green") == 0) {
        return 1; // Valid
    }
    return 0; // Invalid
}

// Function to validate time
int validateTime(int time) {
    // Check if the time is a positive integer
    if (time > 0) {
        return 1; // Valid
    }
    return 0; // Invalid
}

int validateStreet(const char *street) {
    if (street == NULL || strlen(street) == 0) {
        return 0; // Invalid if null or empty
    }

    for (int i = 0; street[i] != '\0'; i++) {
        char c = street[i];
        if (!isalnum(c) && c != ' ' && c != '-') {
            return 0; // Invalid character found
        }
    }

    return 1; // All characters are valid
}

int calculatePrice(const char *zone, int time) {
    // Calculate the price based on the zone and time
    if (strcmp(zone, "yellow") == 0) {
        return yellow_zone * time;
    } else if (strcmp(zone, "red") == 0) {
        return red_zone * time;
    } else if (strcmp(zone, "green") == 0) {
        return green_zone * time;
    }
    return 0; // Invalid zone
}

typedef struct payments {
    char plate[8];
    char street[100];
    char zone[100];
    int time;
    float price;
} payment;

void header(int param) {
    if(param == 1){
        printf("Main menu\n");
        printf("--------------\n");
        printf("1 - Pay for parking\n");
        printf("2 - View Parking history\n");
        printf("0 - Exit\n");
        printf("--------------\n");
    }
    else if (param == 2){
        printf("--------------\n");
        printf("1 - Display all\n");
        printf("2 - Search parking\n");
        printf("0 - Return\n");
        printf("--------------\n");
    }
}

void displayparking(payment p) {
    printf("--------------\n");
    printf("Plate: ");
    SetConsoleTextAttribute(hConsole, 240); // Plate in color 240
    printf("%s\n", p.plate);
    SetConsoleTextAttribute(hConsole, 7); // Reset to default

    printf("Street: %s\n", p.street);

    // Zone in respective color
    printf("Zone: ");
    if (strcmp(p.zone, "yellow") == 0) {
        SetConsoleTextAttribute(hConsole, 6); // Yellow
    } else if (strcmp(p.zone, "red") == 0) {
        SetConsoleTextAttribute(hConsole, 4); // Red
    } else if (strcmp(p.zone, "green") == 0) {
        SetConsoleTextAttribute(hConsole, 2); // Green
    }
    printf("%s\n", p.zone);
    SetConsoleTextAttribute(hConsole, 7); // Reset to default

    printf("Time: %d hours\n", p.time);

    SetConsoleTextAttribute(hConsole, 6); // Price in yellow
    printf("Price: %.2f\n", p.price);
    SetConsoleTextAttribute(hConsole, 7); // Reset to default

    printf("--------------\n");
}

// Function to add a person to the file
void add_parking(char path[]) {
    payment p; // Move to local function scope
    FILE *filePointer;
    filePointer = fopen(path, "a");  // Open the file in append mode

    printf("--------------\n");
    printf("Pay for parking\n");
    printf("--------------\n");

    // Validate plate
    do {
        printf("Enter your plate number (format: XY01XYZ): ");
        scanf("%s", p.plate);
        if (!validateNumberPlate(p.plate)) {
            printf("Invalid data, input again.\n");
        }
    } while (!validateNumberPlate(p.plate));

    do {
        printf("Enter the street (letters, numbers, spaces, hyphens only): ");
        scanf(" %[^\n]%*c", p.street); // Read line with spaces
        if (!validateStreet(p.street)) {
            printf("Invalid data, input again.\n");
        }
    } while (!validateStreet(p.street));

    // Validate zone
    do {
        printf("Enter the zone (yellow/red/green): ");
        scanf("%s", p.zone);
        if (!validateZone(p.zone)) {
            printf("Invalid data, input again.\n");
        }
    } while (!validateZone(p.zone));

    // Validate time
    do {
        printf("Enter the time (in hours): ");
        scanf("%d", &p.time);
        if (!validateTime(p.time)) {
            printf("Invalid data, input again.\n");
        }
    } while (!validateTime(p.time));

    p.price = calculatePrice(p.zone, p.time); // Calculate the price based on the zone

    if (filePointer == NULL) {
        printf("The file cannot be accessed.\n");
    } else {
        // Write the data to the file
        fprintf(filePointer, "%s,%s,%s,%d,%.2f\n", p.plate, p.street, p.zone, p.time, p.price);
        fclose(filePointer);
    }
    displayparking(p); // Display the parking details
    printf("--------------\n");
}

// Function to display the content of the file
void simpleDisplay(char path[]) {
    FILE *filePointer;
    filePointer = fopen(path, "r");

    printf("--------------\n");
    printf("Parking Display\n");
    printf("--------------\n");

    if (filePointer == NULL) {
        printf("The file cannot be accessed.\n");
    } else {
        char line[200];
        while (fgets(line, sizeof(line), filePointer)) {
            // Remove the newline character from the line
            line[strcspn(line, "\n")] = '\0';

            // Tokenize the line to extract fields
            char tempLine[200];
            strcpy(tempLine, line); // Copy the line to avoid modifying the original
            char *plate = strtok(tempLine, ",");
            char *street = strtok(NULL, ",");
            char *zone = strtok(NULL, ",");
            char *time = strtok(NULL, ",");
            char *price = strtok(NULL, ",");

            // Display the extracted fields with appropriate colors
            if (plate && street && zone && time && price) {
                printf("Plate: ");
                SetConsoleTextAttribute(hConsole, 240); // Plate in color 240
                printf("%s", plate);
                SetConsoleTextAttribute(hConsole, 7); // Reset to default
                printf("\n");

                printf("Street: %s\n", street);

                // Zone in respective color
                printf("Zone: ");
                if (strcmp(zone, "yellow") == 0) {
                    SetConsoleTextAttribute(hConsole, 6); // Yellow
                } else if (strcmp(zone, "red") == 0) {
                    SetConsoleTextAttribute(hConsole, 4); // Red
                } else if (strcmp(zone, "green") == 0) {
                    SetConsoleTextAttribute(hConsole, 2); // Green
                }
                printf("%s\n", zone);
                SetConsoleTextAttribute(hConsole, 7); // Reset to default

                printf("Time: %s hours\n", time);

                SetConsoleTextAttribute(hConsole, 6); // Price in yellow
                printf("Price: %s\n", price);
                SetConsoleTextAttribute(hConsole, 7); // Reset to default

                printf("--------------\n");
            }
        }
        fclose(filePointer);
    }

    printf("--------------\n");
    system("pause");
}

void search_by_plate(char path[]) {
    FILE *filePointer;
    filePointer = fopen(path, "r");

    char plate[255];
    printf("Enter the plate number: ");
    scanf("%s", plate);

    if (filePointer == NULL) {
        printf("The file cannot be accessed.\n");
        return;
    }

    char line[200];
    int found = 0;

    printf("--------------\n");
    printf("Search Results for Plate: %s\n", plate);
    printf("--------------\n");

    while (fgets(line, sizeof(line), filePointer)) {
        // Remove the newline character from the line
        line[strcspn(line, "\n")] = '\0';

        // Tokenize the line to extract fields
        char tempLine[200];
        strcpy(tempLine, line); // Copy the line to avoid modifying the original
        char *filePlate = strtok(tempLine, ",");
        char *street = strtok(NULL, ",");
        char *zone = strtok(NULL, ",");
        char *time = strtok(NULL, ",");
        char *price = strtok(NULL, ",");

        if (filePlate != NULL && strcmp(filePlate, plate) == 0) {
            // If the plate matches, display the details with appropriate colors
            printf("Plate: ");
            SetConsoleTextAttribute(hConsole, 240); // Plate in color 240
            printf("%s\n", filePlate);
            SetConsoleTextAttribute(hConsole, 7); // Reset to default

            printf("Street: %s\n", street);
            SetConsoleTextAttribute(hConsole, 7); // Reset to default
            
            printf("Zone: ");
            if (strcmp(zone, "yellow") == 0) {
                SetConsoleTextAttribute(hConsole, 6); // Yellow
            } else if (strcmp(zone, "red") == 0) {
                SetConsoleTextAttribute(hConsole, 4); // Red
            } else if (strcmp(zone, "green") == 0) {
                SetConsoleTextAttribute(hConsole, 2); // Green
            }
            printf("%s\n", zone);
            SetConsoleTextAttribute(hConsole, 7); // Reset to default

            printf("Time: %s hours\n", time);

            printf("Price: ");
            SetConsoleTextAttribute(hConsole, 6); // Yellow for price
            printf("%s\n", price);
            SetConsoleTextAttribute(hConsole, 7); // Reset to default

            printf("--------------\n");
            found = 1;
        }
    }

    fclose(filePointer);

    if (!found) {
        printf("No parking records found for plate: %s\n", plate);
    }

    printf("--------------\n");
    system("pause");
}

void submenu(char path[]) {
    system("cls");
    int option;
    printf("Parking History\n");
    do {
        header(2);
        printf("Enter option: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                simpleDisplay(path);
                break;
            case 2:
                printf("Search parking by plate\n");
                search_by_plate(path);
                break;
            case 0:
                system("cls");
                break;
            default:
                printf("Invalid option\n");
                system("pause");
                system("cls");
                break;
        }
    } while (option > 0 && option <= 3);
}

void menu(int op, char path[]) {
    switch (op) {
        case 1:
            add_parking(path);
            break;
        case 2:
            submenu(path);
            break;
        default:
            printf("Goodbye!\n");
    }
}


int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    colorattribute = 7; // default color is 7 (light gray on black)
    int option;
    printf("Welcome to \n");
    
    SetConsoleTextAttribute(hConsole, 1); // blue
    printf("   ____   ____               _    \n");
    printf("  / ___| |  _ \\  __ _  _ __ | | __\n");
    printf(" | |     | |_) |/ _` || '__|| |/ /\n");
    printf(" | |___  |  __/| (_| || |   |   < \n");
    printf("  \\____| |_|    \\__,_||_|   |_|\\_\\\n");
    SetConsoleTextAttribute(hConsole, 7); // default color is 7 (light gray on black)
    

    char path[] = "C:\\Users\\Lenovo\\Desktop\\UVT\\Sem 2\\Programming Project\\c-project-write-to-file-TeoBulboacaUVT\\file.txt";

    do {
        header(1);
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option, path);
    } while (option > 0 && option <= 3);

    return 0;
}
