#ifndef STUDENTSERVICE_H
#define STUDENTSERVICE_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "../models/Student.h"
#include "../utils/FileHandler.h"
#include "../utils/Validation.h"

class StudentService {
private:
    std::vector<Student> students;
    std::string filePath;

public:
    explicit StudentService(const std::string& path = "data/students.txt") : filePath(path) {
        loadFromFile();
    }

    void loadFromFile() {
        students.clear();
        std::vector<std::string> lines = FileHandler::readAllLines(filePath);
        for (const auto& line : lines) {
            Student s = Student::fromCSV(line);
            if (!s.getStudentID().empty()) {
                students.push_back(s);
            }
        }
    }

    void saveToFile() const {
        std::vector<std::string> lines;
        for (const auto& s : students) {
            lines.push_back(s.toCSV());
        }
        FileHandler::writeAllLines(filePath, lines);
    }

    const std::vector<Student>& getAllStudents() const {
        return students;
    }

    bool exists(const std::string& id) const {
        std::string target = Validation::toUpperCase(id);
        for (const auto& s : students) {
            if (Validation::toUpperCase(s.getStudentID()) == target) {
                return true;
            }
        }
        return false;
    }

    const Student* getStudent(const std::string& id) const {
        std::string target = Validation::toUpperCase(id);
        for (const auto& s : students) {
            if (Validation::toUpperCase(s.getStudentID()) == target) {
                return &s;
            }
        }
        return nullptr;
    }

    void registerStudent() {
        std::cout << "\n========================================\n";
        std::cout << "         REGISTER NEW STUDENT\n";
        std::cout << "========================================\n";

        std::string id;
        while (true) {
            id = Validation::getValidString("Enter Student ID (e.g. ST004): ", false);
            id = Validation::toUpperCase(id);
            if (exists(id)) {
                std::cout << " [Error] Student ID '" << id << "' already exists! Please use a unique ID.\n";
            } else {
                break;
            }
        }

        std::string firstName = Validation::getValidString("Enter First Name: ", false);
        std::string lastName = Validation::getValidString("Enter Last Name: ", false);
        int birthYear = Validation::getValidInt("Enter Birth Year (e.g. 2005): ", 1900, 2026);

        Student newStudent(id, firstName, lastName, birthYear);
        students.push_back(newStudent);
        saveToFile();

        std::cout << "\n [Success] Student registered successfully!\n";
        newStudent.displayStudent();
    }

    void viewAllStudents() const {
        std::cout << "\n====================================================================\n";
        std::cout << "                         ALL STUDENTS LIST                          \n";
        std::cout << "====================================================================\n";
        if (students.empty()) {
            std::cout << " No student records found.\n";
            std::cout << "====================================================================\n";
            return;
        }

        std::cout << std::left 
                  << std::setw(12) << "Student ID"
                  << std::setw(18) << "First Name"
                  << std::setw(18) << "Last Name"
                  << std::setw(12) << "Birth Year"
                  << "\n";
        std::cout << "--------------------------------------------------------------------\n";

        for (const auto& s : students) {
            std::cout << std::left
                      << std::setw(12) << s.getStudentID()
                      << std::setw(18) << s.getFirstName()
                      << std::setw(18) << s.getLastName()
                      << std::setw(12) << s.getBirthYear()
                      << "\n";
        }
        std::cout << "====================================================================\n";
        std::cout << " Total Students: " << students.size() << "\n";
    }

    void searchStudent() const {
        std::cout << "\n========================================\n";
        std::cout << "             SEARCH STUDENT             \n";
        std::cout << "========================================\n";
        std::string query = Validation::getValidString("Enter Student ID or Name to search: ");
        std::string upperQuery = Validation::toUpperCase(query);

        std::vector<Student> matches;
        for (const auto& s : students) {
            if (Validation::toUpperCase(s.getStudentID()).find(upperQuery) != std::string::npos ||
                Validation::toUpperCase(s.getFirstName()).find(upperQuery) != std::string::npos ||
                Validation::toUpperCase(s.getLastName()).find(upperQuery) != std::string::npos) {
                matches.push_back(s);
            }
        }

        if (matches.empty()) {
            std::cout << "\n [Info] No student records matched '" << query << "'.\n";
        } else {
            std::cout << "\n Found " << matches.size() << " match(es):\n";
            for (const auto& m : matches) {
                m.displayStudent();
                std::cout << "----------------------------------------\n";
            }
        }
    }

    void updateStudent() {
        std::cout << "\n========================================\n";
        std::cout << "             UPDATE STUDENT             \n";
        std::cout << "========================================\n";
        std::string id = Validation::getValidString("Enter Student ID to update: ", false);
        id = Validation::toUpperCase(id);

        auto it = std::find_if(students.begin(), students.end(), [&](const Student& s) {
            return Validation::toUpperCase(s.getStudentID()) == id;
        });

        if (it == students.end()) {
            std::cout << " [Error] Student with ID '" << id << "' not found.\n";
            return;
        }

        std::cout << "\nCurrent Details:";
        it->displayStudent();

        std::cout << "\nSelect detail to update:\n";
        std::cout << "1. First Name (" << it->getFirstName() << ")\n";
        std::cout << "2. Last Name (" << it->getLastName() << ")\n";
        std::cout << "3. Birth Year (" << it->getBirthYear() << ")\n";
        std::cout << "4. Update All Fields\n";
        std::cout << "5. Cancel\n";
        int choice = Validation::getValidInt("Enter your choice (1-5): ", 1, 5);

        if (choice == 1) {
            std::string first = Validation::getValidString("Enter New First Name: ", false);
            it->setFirstName(first);
        } else if (choice == 2) {
            std::string last = Validation::getValidString("Enter New Last Name: ", false);
            it->setLastName(last);
        } else if (choice == 3) {
            int year = Validation::getValidInt("Enter New Birth Year: ", 1900, 2026);
            it->setBirthYear(year);
        } else if (choice == 4) {
            std::string first = Validation::getValidString("Enter New First Name: ", false);
            std::string last = Validation::getValidString("Enter New Last Name: ", false);
            int year = Validation::getValidInt("Enter New Birth Year: ", 1900, 2026);
            it->setFirstName(first);
            it->setLastName(last);
            it->setBirthYear(year);
        } else {
            std::cout << " [Info] Update cancelled.\n";
            return;
        }

        saveToFile();
        std::cout << "\n [Success] Student updated successfully!\n";
        it->displayStudent();
    }

    bool deleteStudent() {
        std::cout << "\n========================================\n";
        std::cout << "             DELETE STUDENT             \n";
        std::cout << "========================================\n";
        std::string id = Validation::getValidString("Enter Student ID to delete: ", false);
        id = Validation::toUpperCase(id);

        auto it = std::find_if(students.begin(), students.end(), [&](const Student& s) {
            return Validation::toUpperCase(s.getStudentID()) == id;
        });

        if (it == students.end()) {
            std::cout << " [Error] Student with ID '" << id << "' not found.\n";
            return false;
        }

        std::cout << "\nTarget Student:";
        it->displayStudent();

        if (Validation::confirmAction("\nAre you sure you want to permanently delete this student?")) {
            students.erase(it);
            saveToFile();
            std::cout << "\n [Success] Student '" << id << "' has been deleted.\n";
            return true;
        } else {
            std::cout << " [Info] Deletion cancelled.\n";
            return false;
        }
    }

    void seedDefaultStudents() {
        if (students.empty()) {
            students.push_back(Student("ST001", "Savini", "Peramuna", 2005));
            students.push_back(Student("ST002", "Kamal", "Perera", 2004));
            students.push_back(Student("ST003", "Nimal", "Silva", 2005));
            saveToFile();
        }
    }
};

#endif // STUDENTSERVICE_H
