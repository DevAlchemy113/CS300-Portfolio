// David Silva
// CS-300: Analysis and Design
// Project Two – Course Planner
// Date: December 9, 2025
//
// This program implements a course planning and advising system using a hash
// table. It loads course data from a file, prints an alphabetical list of
// courses, and allows the user to view details and prerequisites for any
// single course.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

/*
    This function removes spaces, tabs, and newlines from the beginning
    and end of a string. The course data file sometimes has extra spaces,
    so trimming ensures we store clean and consistent course names and numbers.
*/
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

/*
    Converts any string to uppercase. This lets the user type a course like:
        csci300
        CsCi300
        CSCI300
    and still match the stored CSCI300 in the hash table.
*/
string toUpper(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c){ return toupper(c); });
    return result;
}

/*
    A course object stores everything the advising system needs:
    - courseNumber  (ex: "CSCI300")
    - courseName    (ex: "Introduction to Algorithms")
    - prereqList    (zero, one, or several prerequisites)
*/
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prereqList;
};

/*
    HashNode is one entry in the hash table. Instead of using arrays,
    the hash table uses linked lists (chaining) to resolve collisions.
*/
struct HashNode {
    Course course;
    HashNode* next;
    HashNode(const Course& c) : course(c), next(nullptr) {}
};

/*
    The HashTable stores all courses from the CSV file.
    Hash tables give very fast average lookup times, which is why the
    Week 6 assignment recommended this data structure for Project Two.

    The table uses:
    - A vector of bucket pointers
    - Each bucket holds a linked list of HashNodes
*/
class HashTable {
public:

    // Create a table with the requested number of buckets.
    HashTable(int size = 20) {
        tableSize = size;
        buckets.resize(tableSize, nullptr);
    }

    // Destructor frees all allocated nodes.
    ~HashTable() {
        for (int i = 0; i < tableSize; ++i) {
            HashNode* current = buckets[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    /*
        Insert a course using the hash of its course number.
        If the bucket is empty → place it there.
        If not → append to the end of the linked list.
    */
    void insertCourse(const Course& c) {
        int key = hashKey(c.courseNumber);
        HashNode* newNode = new HashNode(c);

        if (!buckets[key]) {
            buckets[key] = newNode;
        } else {
            HashNode* current = buckets[key];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    /*
        Quickly find a course by its number.
        Returns a pointer if found, or nullptr if the course does not exist.
    */
    Course* findCourse(const string& courseNumber) {
        int key = hashKey(courseNumber);
        HashNode* current = buckets[key];

        while (current != nullptr) {
            if (current->course.courseNumber == courseNumber) {
                return &(current->course);
            }
            current = current->next;
        }
        return nullptr;
    }

    /*
        Returns every course stored in the table. This allows us to sort
        them alphabetically when printing the full course list.
    */
    vector<Course> getAllCourses() const {
        vector<Course> all;

        for (int i = 0; i < tableSize; ++i) {
            HashNode* current = buckets[i];
            while (current != nullptr) {
                all.push_back(current->course);
                current = current->next;
            }
        }
        return all;
    }

private:
    vector<HashNode*> buckets;
    int tableSize;

    /*
        Simple hash function that converts the courseNumber (ex: "CSCI300")
        into a bucket index using the sum of ASCII values.

        Example:
           C + S + C + I + 3 + 0 + 0 → mod tableSize
    */
    int hashKey(const string& key) const {
        int sum = 0;
        for (char ch : key) sum += static_cast<unsigned char>(ch);
        return sum % tableSize;
    }
};

/*
    Reads the CSV input file and loads the hash table with courses.
    Each line in the file may look like:

        CSCI300,Introduction to Algorithms,CSCI200,MATH201

    This function:
        - Splits the line by commas
        - Extracts the course number and name
        - Stores zero or more prerequisites
*/
bool loadDataStructure(const string& fileName, HashTable& table) {
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        cout << "Error: Unable to open file \"" << fileName << "\"." << endl;
        return false;
    }

    string line;
    while (getline(inFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string token;

        Course c;

        // Course number
        if (!getline(ss, token, ',')) continue;
        c.courseNumber = toUpper(trim(token));

        // Course name
        if (!getline(ss, token, ',')) continue;
        c.courseName = trim(token);

        // Zero, one, or multiple prerequisites
        while (getline(ss, token, ',')) {
            string prereq = toUpper(trim(token));
            if (!prereq.empty()) c.prereqList.push_back(prereq);
        }

        table.insertCourse(c);
    }

    inFile.close();
    cout << "Data structure loaded." << endl;
    return true;
}

/*
    Prints all courses in alphabetical order by course number.
    Hash tables do not store data in order, so we collect all courses
    into a vector and sort them before printing.
*/
void printCourseList(const HashTable& table) {
    vector<Course> courses = table.getAllCourses();

    sort(courses.begin(), courses.end(),
         [](const Course& a, const Course& b) {
             return a.courseNumber < b.courseNumber;
         });

    for (const Course& c : courses) {
        cout << c.courseNumber << ", " << c.courseName << endl;
    }
}

/*
    Allows the user to type a course number and prints detailed information.
    - Shows course name
    - Lists prerequisites or says "None"
*/
void printSingleCourse(HashTable& table) {
    cout << "What course do you want to know about? ";

    string input;
    cin >> input;
    string courseNumber = toUpper(input);

    Course* c = table.findCourse(courseNumber);
    if (!c) {
        cout << "Course not found." << endl;
        return;
    }

    cout << c->courseNumber << ", " << c->courseName << endl;

    cout << "Prerequisites: ";
    if (c->prereqList.empty()) {
        cout << "None" << endl;
    } else {
        for (size_t i = 0; i < c->prereqList.size(); ++i) {
            cout << c->prereqList[i];
            if (i < c->prereqList.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

/*
    Main program: displays the menu and processes user selections.
    Uses a loop so the user can load files, print lists, and lookup
    courses without restarting the program.
*/
int main() {

    HashTable table(20);           // Stores all course data in memory
    bool dataLoaded = false;       // Tracks whether the file was successfully loaded

    cout << "Welcome to the course planner." << endl;

    int choice = 0;

    while (choice != 9) {

        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit." << endl;
        cout << endl;
        cout << "What would you like to do? ";

        // Validate numeric input
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "That is not a valid option." << endl;
            continue;
        }

        cout << endl;

        if (choice == 1) {
            /*
                Load the CSV file into the hash table.
                Use getline() to allow spaces in filenames like:
                    "CS 300 ABCU_Advising_Program_Input.csv"
            */
            string fileName;
            cout << "Enter the file name to load: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, fileName);

            dataLoaded = loadDataStructure(fileName, table);

        } else if (choice == 2) {
            // Print the full ordered course list
            if (!dataLoaded) {
                cout << "Please load the data structure first (option 1)." << endl;
            } else {
                cout << "Here is a sample schedule:" << endl;
                printCourseList(table);
            }

        } else if (choice == 3) {
            // Lookup a single course and display details
            if (!dataLoaded) {
                cout << "Please load the data structure first (option 1)." << endl;
            } else {
                printSingleCourse(table);
            }

        } else if (choice == 9) {
            // Gracefully exit program
            cout << "Thank you for using the course planner!" << endl;

        } else {
            // Any other number is invalid
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}