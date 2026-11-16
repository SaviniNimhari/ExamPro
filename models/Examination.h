#ifndef EXAMINATION_H
#define EXAMINATION_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class Examination {
private:
    std::string examID;
    std::string examName;
    std::string examDate; // YYYY-MM-DD
    std::vector<std::string> subjectIDs;

public:
    Examination() : examID(""), examName(""), examDate("") {}

    Examination(const std::string& id, const std::string& name, const std::string& date, const std::vector<std::string>& subjects = {})
        : examID(id), examName(name), examDate(date), subjectIDs(subjects) {}

    // Getters
    std::string getExamID() const { return examID; }
    std::string getExamName() const { return examName; }
    std::string getExamDate() const { return examDate; }
    const std::vector<std::string>& getSubjectIDs() const { return subjectIDs; }

    // Setters
    void setExamID(const std::string& id) { examID = id; }
    void setExamName(const std::string& name) { examName = name; }
    void setExamDate(const std::string& date) { examDate = date; }
    void setSubjectIDs(const std::vector<std::string>& subs) { subjectIDs = subs; }

    // Subject operations
    bool assignSubject(const std::string& subjectID) {
        if (hasSubject(subjectID)) return false;
        subjectIDs.push_back(subjectID);
        return true;
    }

    bool removeSubject(const std::string& subjectID) {
        auto it = std::find(subjectIDs.begin(), subjectIDs.end(), subjectID);
        if (it != subjectIDs.end()) {
            subjectIDs.erase(it);
            return true;
        }
        return false;
    }

    bool hasSubject(const std::string& subjectID) const {
        return std::find(subjectIDs.begin(), subjectIDs.end(), subjectID) != subjectIDs.end();
    }

    void displayExam() const {
        std::cout << "\nExam ID   : " << examID;
        std::cout << "\nExam Name : " << examName;
        std::cout << "\nExam Date : " << examDate;
        std::cout << "\nAssigned Subjects (" << subjectIDs.size() << "): ";
        if (subjectIDs.empty()) {
            std::cout << "None";
        } else {
            for (size_t i = 0; i < subjectIDs.size(); ++i) {
                std::cout << subjectIDs[i] << (i + 1 < subjectIDs.size() ? ", " : "");
            }
        }
        std::cout << "\n";
    }

    // CSV format: examID,examName,examDate,sub1;sub2;sub3
    std::string toCSV() const {
        std::string subjectsJoined = "";
        for (size_t i = 0; i < subjectIDs.size(); ++i) {
            subjectsJoined += subjectIDs[i];
            if (i + 1 < subjectIDs.size()) {
                subjectsJoined += ";";
            }
        }
        return examID + "," + examName + "," + examDate + "," + subjectsJoined;
    }

    static Examination fromCSV(const std::string& line) {
        std::stringstream ss(line);
        std::string id, name, date, subsPart;
        if (std::getline(ss, id, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, date, ',')) {
            
            std::vector<std::string> subs;
            if (std::getline(ss, subsPart)) {
                if (!subsPart.empty() && subsPart.back() == '\r') {
                    subsPart.pop_back();
                }
                std::stringstream subsStream(subsPart);
                std::string sub;
                while (std::getline(subsStream, sub, ';')) {
                    if (!sub.empty()) {
                        subs.push_back(sub);
                    }
                }
            }
            return Examination(id, name, date, subs);
        }
        return Examination();
    }
};

#endif // EXAMINATION_H
