#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class Student {
private:
    std::string studentID;
    std::string firstName;
    std::string lastName;
    int birthYear;

public:
    Student() : studentID(""), firstName(""), lastName(""), birthYear(2000) {}

    Student(const std::string& id, const std::string& first, const std::string& last, int year)
        : studentID(id), firstName(first), lastName(last), birthYear(year) {}

    // Getters
    std::string getStudentID() const { return studentID; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getFullName() const { return firstName + " " + lastName; }
    int getBirthYear() const { return birthYear; }

    // Setters
    void setStudentID(const std::string& id) { studentID = id; }
    void setFirstName(const std::string& first) { firstName = first; }
    void setLastName(const std::string& last) { lastName = last; }
    void setBirthYear(int year) { birthYear = year; }

    // Display student information
    void displayStudent() const {
        std::cout << "\nStudent ID : " << studentID;
        std::cout << "\nFirst Name : " << firstName;
        std::cout << "\nLast Name  : " << lastName;
        std::cout << "\nFull Name  : " << getFullName();
        std::cout << "\nBirth Year : " << birthYear;
        std::cout << "\n";
    }

    // CSV format: ID,FirstName,LastName,BirthYear
    std::string toCSV() const {
        return studentID + "," + firstName + "," + lastName + "," + std::to_string(birthYear);
    }

    static Student fromCSV(const std::string& line) {
        std::stringstream ss(line);
        std::string id, first, last, yearStr;
        if (std::getline(ss, id, ',') &&
            std::getline(ss, first, ',') &&
            std::getline(ss, last, ',') &&
            std::getline(ss, yearStr, ',')) {
            int year = 2000;
            try {
                year = std::stoi(yearStr);
            } catch (...) {
                year = 2000;
            }
            return Student(id, first, last, year);
        }
        return Student();
    }
};

#endif // STUDENT_H
