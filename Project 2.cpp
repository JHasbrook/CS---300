/*
 * JACOB HASBROOK
 * CS-300 SNHU
 * 12/15/2024
 * 
 * This project uses `std::map` as the primary data structure to store and manage courses.
 * The decision to use `std::map` is based on the following reasons:
 *
 * 1. **Lexicographical (Alphanumeric +more) Order**:
 *    - `std::map` automatically maintains all keys (course IDs) in lexicographical order.
 *    - This ordering is crucial for this project as the courses need to be displayed
 *      in a sorted manner (e.g., CSCI100 comes before SCSI200).
 *    - By leveraging `std::map`, we eliminate the need for manual sorting, ensuring
 *      efficient insertion and retrieval while keeping the data always sorted.
 *
 * 2. **Efficient Operations**:
 *    - `std::map` provides logarithmic time complexity (O(log n)) for insertion, deletion,
 *      and lookup operations, making it suitable for managing a dynamic dataset of courses.
 *    - This efficiency allows us to handle large datasets while maintaining fast access
 *      and modification operations.
 *
 * 3. **Project Requirements**:
 *    - The lexicographical ordering of courses is necessary to ensure a structured
 *      and professional display of data, especially when listing all courses.
 *    - `std::map` simplifies the implementation, reducing the need for additional
 *      sorting logic and improving code maintainability.
 *
 * Why Lexicographical Order Matters:
 *    - For academic records, course lists are often expected to appear in ascending order
 *      by course ID. This makes it easier for users to locate
 *      specific courses visually.
 *    - The sorted order adds clarity and professionalism to the output, ensuring users
 *      can quickly identify prerequisites or related courses.
 *
 * Alternative Considerations:
 *    - While `std::unordered_map` provides O(1) average-case time complexity for lookups
 *      and insertions, it does not maintain any specific order. Thus, it was not selected
 *      as it cannot meet the project's lexicographical ordering method.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono> // For timing
#include <cstdlib> // For system()

using namespace std;
using namespace chrono; // For cleaner timing syntax

// ANSI escape code for green text
const string GREEN = "\033[32m";
const string RESET = "\033[0m"; // Reset color

// Struct to represent a course
struct Course {
    string courseID;
    string courseName;
    vector<string> prerequisites;

    Course() : courseID(""), courseName("") {}
};

// Map for lexicographical ordering
map<string, Course> courseMap;

// Function to clear the console
void ClearConsole() {
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux/macOS
#endif
}

// Function to validate the file path
string GetValidFilePath() {
    string filePath;

    while (true) {
        cout << "\nEnter the file name (press Enter to use default: 'coursesFile.csv'): ";
        getline(cin, filePath);

        // Use default file if input is empty
        if (filePath.empty()) {
            filePath = "coursesFile.csv";
        }

        // Check if the file exists and is accessible
        ifstream file(filePath);
        if (file.is_open()) {
            file.close();
            return filePath; // Return valid file path
        }
        else {
            cout << "\nError: Unable to open file at '" << filePath << "'. Please try again." << endl;
        }
    }
}

// Function to load courses from a file
void LoadCourses(const string& filePath) {
    ClearConsole(); // Clear screen before displaying results
    auto start = high_resolution_clock::now(); // Start timer

    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "\nError: Unable to open file at " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        stringstream ss(line);
        vector<string> tokens;
        string token;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            cout << "\nWarning: Skipping malformed line: " << line << endl;
            continue;
        }

        Course course;
        course.courseID = tokens[0];
        course.courseName = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]); // Add prerequisites
        }

        courseMap[course.courseID] = course; // Insert into map
    }

    file.close();
    auto end = high_resolution_clock::now(); // End timer
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "\nCourses successfully loaded into the system in " << duration.count() << " ms." << endl;
    cout << GREEN << "Time Complexity: O(n log n) due to map insertion for n courses." << RESET << endl;
}

// Function to search for a course
void PrintCourse(const string& courseID) {
    ClearConsole(); // Clear screen before displaying results
    auto start = high_resolution_clock::now(); // Start timer

    if (courseMap.find(courseID) == courseMap.end()) {
        cout << "\nError: Course with ID " << courseID << " not found." << endl;
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "\nCourse search completed in " << duration.count() << " ms." << endl;
        cout << GREEN << "Time Complexity: O(log n) for map lookup." << RESET << endl;
        return;
    }

    const Course& course = courseMap[courseID];
    cout << "Course ID: " << course.courseID << endl;
    cout << "Course Name: " << course.courseName << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    auto end = high_resolution_clock::now(); // End timer
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "\nCourse search completed in " << duration.count() << " ms." << endl;
    cout << GREEN << "Time Complexity: O(log n) for map lookup." << RESET << endl;
}

// Function to print all courses in lexicographical order
void PrintAllCourses() {
    ClearConsole(); // Clear screen before displaying results
    auto start = high_resolution_clock::now(); // Start timer

    if (courseMap.empty()) {
        cout << "\nNo courses available. Load data first." << endl;
        return;
    }

    cout << "All Courses (Lexicographical Order):" << endl;
    for (const auto& pair : courseMap) {
        const string& courseID = pair.first;
        const Course& course = pair.second;
        cout << courseID << ": " << course.courseName << endl;
    }

    auto end = high_resolution_clock::now(); // End timer
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "\nAll courses displayed in " << duration.count() << " ms." << endl;
    cout << GREEN << "Time Complexity: O(n) for in-order traversal of map." << RESET << endl;
}

// Main function
int main() {
    string filePath = GetValidFilePath(); // Get a valid file path
    int choice;

    do {
        ClearConsole(); // Clear screen before displaying the menu
        cout << "\nMenu:\n";
        cout << "1. Load Courses\n";
        cout << "2. Display All Courses\n";
        cout << "3. Find Course\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            LoadCourses(filePath);
            break;
        case 2:
            PrintAllCourses();
            break;
        case 3: {
            string courseID;
            cout << "\nEnter course ID: ";
            cin >> courseID;
            transform(courseID.begin(), courseID.end(), courseID.begin(), ::toupper);
            PrintCourse(courseID);
            break;
        }
        case 9:
            cout << "\nGoodbye!" << endl;
            break;
        default:
            cout << "\nInvalid choice. Try again. It’s really not that hard." << endl;
        }

        cout << "\nPress Enter to return to the menu...";
        cin.ignore();
        cin.get();
    } while (choice != 9);

    return 0;
}


