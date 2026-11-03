#include <iostream>
#include <string>
#include <vector>

using namespace std;

// =========================================================
// 1. ABSTRACTION
// Person defines what a person is, but leaves displayInfo() 
// as a pure virtual function to be defined later.
// =========================================================
class Person {
protected:
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {}
    
    // Virtual destructor is needed to properly delete derived objects
    virtual ~Person() {} 
    
    virtual void displayInfo() const = 0; 
};

// =========================================================
// 2. ENCAPSULATION & INHERITANCE
// Student inherits from Person. Data like marks are private 
// and protected by addMark() so invalid data cannot enter.
// =========================================================
class Student : public Person {
private:
    string studentId;
    vector<double> marks;
public:
    Student(string id, string n, int a) : Person(n, a), studentId(id) {}

    string getId() const { return studentId; }

    // Encapsulation: Prevents invalid marks from bypassing validation
    void addMark(double mark) {
        if (mark >= 0.0 && mark <= 100.0) {
            marks.push_back(mark);
        } else {
            cout << "Invalid mark!\n";
        }
    }

    double average() const {
        if (marks.empty()) return 0.0;
        double total = 0.0;
        for (double m : marks) total += m;
        return total / marks.size();
    }

    // Pure virtual functions force subclasses to define their own grading rules
    virtual string calculateGrade() const = 0;
};

// =========================================================
// 3. POLYMORPHISM
// UG and PG students calculate grades differently.
// =========================================================
class UndergraduateStudent : public Student {
public:
    UndergraduateStudent(string id, string n, int a) : Student(id, n, a) {}

    string calculateGrade() const override {
        double avg = average();
        if (avg >= 80) return "A";
        if (avg >= 60) return "B";
        if (avg >= 50) return "C";
        return "Fail";
    }

    void displayInfo() const override {
        cout << "[UG] ID: " << getId() << " | Name: " << name 
             << " | Avg: " << average() << " | Grade: " << calculateGrade() << "\n";
    }
};

class PostgraduateStudent : public Student {
public:
    PostgraduateStudent(string id, string n, int a) : Student(id, n, a) {}

    // Stricter grading scale for Postgraduates[cite: 1, 2]
    string calculateGrade() const override {
        double avg = average();
        if (avg >= 85) return "A";
        if (avg >= 75) return "B";
        if (avg >= 55) return "C";
        return "Fail";
    }

    void displayInfo() const override {
        cout << "[PG] ID: " << getId() << " | Name: " << name 
             << " | Avg: " << average() << " | Grade: " << calculateGrade() << "\n";
    }
};

// =========================================================
// SYSTEM MANAGER
// Handles the vector of pointers and clean memory management.
// =========================================================
class StudentManagementSystem {
private:
    vector<Student*> students; // Using raw pointers to avoid object slicing[cite: 1, 2]

public:
    ~StudentManagementSystem() {
        // Clean up raw pointers to prevent memory leaks
        for (Student* s : students) {
            delete s;
        }
    }

    void addStudent(Student* s) {
        students.push_back(s);
        cout << "Student added!\n";
    }

    void displayAll() const {
        if (students.empty()) {
            cout << "No students found.\n";
            return;
        }
        // Polymorphism in action: calls the correct displayInfo() automatically[cite: 1, 2]
        for (Student* s : students) {
            s->displayInfo(); 
        }
    }
};

// =========================================================
// MAIN ENTRY POINT
// =========================================================
int main() {
    StudentManagementSystem system;
    int choice;

    do {
        cout << "\n1. Add UG Student\n2. Add PG Student\n3. View All\n4. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            string id, name;
            int age;
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin >> name;
            cout << "Age: "; cin >> age;

            Student* newStudent = nullptr;
            if (choice == 1) {
                newStudent = new UndergraduateStudent(id, name, age);
            } else {
                newStudent = new PostgraduateStudent(id, name, age);
            }

            cout << "Enter a mark: ";
            double mark;
            cin >> mark;
            newStudent->addMark(mark);

            system.addStudent(newStudent);
        } 
        else if (choice == 3) {
            system.displayAll();
        }
    } while (choice != 4);

    return 0;
}
