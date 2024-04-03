// hash-include directive
#include <iostream>
#include <fstream>
#include <cstring>
#include <limits.h>

// Global declaration of constants
#define MAX_FILENAME_LENGTH 50
#define MAX_LINE_LENGTH 100
#define MAX_FLIGHTS 100
#define MAX_SOURCE_DEST_LENGTH 50

// user defined data type (struct)
struct Flight {
	// struct members
    char source[MAX_SOURCE_DEST_LENGTH];
    char destination[MAX_SOURCE_DEST_LENGTH];
    int fare;
    char airline[MAX_FILENAME_LENGTH - 4]; // as the filename without ".txt" extension doesn't exceed 50 characters
};

// parseLine function: extracting information from flight result
int parseLine(char* source, char* destination, int* fare, char* line) {
    char* token = strtok(line, "-,");
    if (token == nullptr)
        return -1; // Error: Missing dash or comma
    strcpy(source, token);

    token = strtok(nullptr, "-,");
    if (token == nullptr)
        return -1; // Error: Missing comma
    strcpy(destination, token);

    token = strtok(nullptr, "-,");
    if (token == nullptr)
        return -1; // Error: Missing fare
    *fare = atoi(token);

    return 0; // Success
}

// processFlight function: reads all text data files and extracts flight information using parseLine()
int processFlight(const char* filename, Flight flights[], int& totalCount) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return -1; // Error opening file
    }

    char line[MAX_LINE_LENGTH];
    while (file.getline(line, MAX_LINE_LENGTH)) {
        if (strlen(line) == 0) // Skip empty lines
            continue;

        char source[MAX_SOURCE_DEST_LENGTH];
        char destination[MAX_SOURCE_DEST_LENGTH];
        int fare;
        if (parseLine(source, destination, &fare, line) != 0) {
            std::cerr << "Error: Missing comma or dash in line: " << line << "\n";
            continue; // Skip malformed line
        }

        strcpy(flights[totalCount].source, source);
        strcpy(flights[totalCount].destination, destination);
        flights[totalCount].fare = fare;
        strcpy(flights[totalCount].airline, filename);
        totalCount++;
    }

    file.close();
    return 0; // Success
}

// function to display least fare details
void displayLeastFareDetails(Flight flights[], int totalCount) {
    int minFare = INT_MAX;
    int minIndex = -1;

    for (int i = 0; i < totalCount; ++i) {
        if (flights[i].fare < minFare) {
            minFare = flights[i].fare;
            minIndex = i;
        }
    }

    if (minIndex != -1) {
        std::cout << flights[minIndex].airline << ": " << flights[minIndex].source << " to " << flights[minIndex].destination
                  << ", Fare $" << flights[minIndex].fare << "\n";
    }
}

// driving block
int main() {
    Flight flights[MAX_FLIGHTS];
    int totalCount = 0;
	
	// file handling
    std::ifstream flightsFile("flights.txt");
    if (!flightsFile) {
        std::cerr << "Error: Could not open flights.txt\n";
        return 1;
    }

    char filename[MAX_FILENAME_LENGTH];
    while (flightsFile.getline(filename, MAX_FILENAME_LENGTH)) {
        if (strlen(filename) == 0) // Skip empty lines
            continue;

        if (processFlight(filename, flights, totalCount) != 0) {
            std::cerr << "Error processing file: " << filename << "\n";
        }
    }

    flightsFile.close();

    displayLeastFareDetails(flights, totalCount);

    return 0;
}