#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <string>
#include <sstream>

class Subject {
private:
    std::string subjectID;
    std::string subjectName;

public:
    Subject() : subjectID(""), subjectName("") {}

    Subject(const std::string& id, const std::string& name)
        : subjectID(id), subjectName(name) {}

    // Getters
    std::string getSubjectID() const { return subjectID; }
    std::string getSubjectName() const { return subjectName; }

    // Setters
    void setSubjectID(const std::string& id) { subjectID = id; }
    void setSubjectName(const std::string& name) { subjectName = name; }

    // Display subject information
    void displaySubject() const {
        std::cout << "\nSubject ID   : " << subjectID;
        std::cout << "\nSubject Name : " << subjectName;
        std::cout << "\n";
    }

    // CSV format: ID,SubjectName
    std::string toCSV() const {
        return subjectID + "," + subjectName;
    }

    static Subject fromCSV(const std::string& line) {
        std::stringstream ss(line);
        std::string id, name;
        if (std::getline(ss, id, ',') && std::getline(ss, name)) {
            // Trim trailing \r if any
            if (!name.empty() && name.back() == '\r') {
                name.pop_back();
            }
            return Subject(id, name);
        }
        return Subject();
    }
};

#endif // SUBJECT_H
