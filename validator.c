#include "validator.h"
#include <string.h>
#include <ctype.h>

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