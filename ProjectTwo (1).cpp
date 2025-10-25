#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // for transform (case-insensitive search)

using namespace std;

// ---------------------------
// Course structure
// ---------------------------
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// ---------------------------
// BST Node
// ---------------------------
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// ---------------------------
// BST Class
// ---------------------------
class CourseBST {
private:
    Node* root;

    void insert(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseName << endl;
            inOrder(node->right);
        }
    }

    Node* search(Node* node, string courseNumber) {
        if (node == nullptr) return nullptr;
        if (node->course.courseNumber == courseNumber) return node;
        if (courseNumber < node->course.courseNumber) return search(node->left, courseNumber);
        return search(node->right, courseNumber);
    }

public:
    CourseBST() : root(nullptr) {}

    void insert(Course course) {
        insert(root, course);
    }

    void printInOrder() {
        inOrder(root);
    }

    Course* search(string courseNumber) {
        Node* result = search(root, courseNumber);
        if (result != nullptr) return &result->course;
        return nullptr;
    }
};

// ---------------------------
// Helper Functions
// ---------------------------
vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void loadDataStructure(string filename, CourseBST& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');
        if (tokens.size() >= 2) {
            Course course;
            course.courseNumber = tokens[0];
            course.courseName = tokens[1];
            for (size_t i = 2; i < tokens.size(); i++) {
                course.prerequisites.push_back(tokens[i]);
            }
            bst.insert(course);
        }
    }
    file.close();
    cout << "Data loaded successfully." << endl;
}

void printCourse(CourseBST& bst, string courseNumber) {
    // Normalize input to uppercase
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    Course* course = bst.search(courseNumber);
    if (course == nullptr) {
        cout << "Course not found: " << courseNumber << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseName << endl;
    if (!course->prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); i++) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    else {
        cout << "Prerequisites: None" << endl;
    }
}

// ---------------------------
// Main Program
// ---------------------------
int main() {
    CourseBST bst;
    int choice = 0;
    string filename;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter the filename to load: ";
            cin >> filename;
            loadDataStructure(filename, bst);
            break;
        case 2:
            cout << "Here is a sample schedule:" << endl;
            bst.printInOrder();
            break;
        case 3: {
            string courseNumber;
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            printCourse(bst, courseNumber);
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
