#ifndef SUBJECTSERVICE_H
#define SUBJECTSERVICE_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "../models/Subject.h"
#include "../utils/FileHandler.h"
#include "../utils/Validation.h"

class SubjectService {
private:
    std::vector<Subject> subjects;
    std::string filePath;

public:
    explicit SubjectService(const std::string& path = "data/subjects.txt") : filePath(path) {
        loadFromFile();
    }

    void loadFromFile() {
        subjects.clear();
        std::vector<std::string> lines = FileHandler::readAllLines(filePath);
        for (const auto& line : lines) {
            Subject s = Subject::fromCSV(line);
            if (!s.getSubjectID().empty()) {
                subjects.push_back(s);
            }
        }
    }

    void saveToFile() const {
        std::vector<std::string> lines;
        for (const auto& s : subjects) {
            lines.push_back(s.toCSV());
        }
        FileHandler::writeAllLines(filePath, lines);
    }

    const std::vector<Subject>& getAllSubjects() const {
        return subjects;
    }

    bool exists(const std::string& id) const {
        std::string target = Validation::toUpperCase(id);
        for (const auto& s : subjects) {
            if (Validation::toUpperCase(s.getSubjectID()) == target) {
                return true;
            }
        }
        return false;
    }

    const Subject* getSubject(const std::string& id) const {
        std::string target = Validation::toUpperCase(id);
        for (const auto& s : subjects) {
            if (Validation::toUpperCase(s.getSubjectID()) == target) {
                return &s;
            }
        }
        return nullptr;
    }

    std::string getSubjectName(const std::string& id) const {
        const Subject* s = getSubject(id);
        return s ? s->getSubjectName() : ("Unknown (" + id + ")");
    }

    void addSubject() {
        std::cout << "\n========================================\n";
        std::cout << "            ADD NEW SUBJECT             \n";
        std::cout << "========================================\n";

        std::string id;
        while (true) {
            id = Validation::getValidString("Enter Subject ID (e.g. SUB004): ", false);
            id = Validation::toUpperCase(id);
            if (exists(id)) {
                std::cout << " [Error] Subject ID '" << id << "' already exists! Please use a unique ID.\n";
            } else {
                break;
            }
        }

        std::string name = Validation::getValidString("Enter Subject Name: ", true);

        Subject newSubject(id, name);
        subjects.push_back(newSubject);
        saveToFile();

        std::cout << "\n [Success] Subject added successfully!\n";
        newSubject.displaySubject();
    }

    void viewAllSubjects() const {
        std::cout << "\n======================================================\n";
        std::cout << "                  ALL SUBJECTS LIST                   \n";
        std::cout << "======================================================\n";
        if (subjects.empty()) {
            std::cout << " No subjects registered yet.\n";
            std::cout << "======================================================\n";
            return;
        }

        std::cout << std::left 
                  << std::setw(15) << "Subject ID"
                  << std::setw(35) << "Subject Name"
                  << "\n";
        std::cout << "------------------------------------------------------\n";

        for (const auto& s : subjects) {
            std::cout << std::left
                      << std::setw(15) << s.getSubjectID()
                      << std::setw(35) << s.getSubjectName()
                      << "\n";
        }
        std::cout << "======================================================\n";
        std::cout << " Total Subjects: " << subjects.size() << "\n";
    }

    void searchSubject() const {
        std::cout << "\n========================================\n";
        std::cout << "             SEARCH SUBJECT             \n";
        std::cout << "========================================\n";
        std::string query = Validation::getValidString("Enter Subject ID or Name to search: ");
        std::string upperQuery = Validation::toUpperCase(query);

        std::vector<Subject> matches;
        for (const auto& s : subjects) {
            if (Validation::toUpperCase(s.getSubjectID()).find(upperQuery) != std::string::npos ||
                Validation::toUpperCase(s.getSubjectName()).find(upperQuery) != std::string::npos) {
                matches.push_back(s);
            }
        }

        if (matches.empty()) {
            std::cout << "\n [Info] No subjects matched '" << query << "'.\n";
        } else {
            std::cout << "\n Found " << matches.size() << " match(es):\n";
            for (const auto& m : matches) {
                m.displaySubject();
                std::cout << "----------------------------------------\n";
            }
        }
    }

    void updateSubject() {
        std::cout << "\n========================================\n";
        std::cout << "             UPDATE SUBJECT             \n";
        std::cout << "========================================\n";
        std::string id = Validation::getValidString("Enter Subject ID to update: ", false);
        id = Validation::toUpperCase(id);

        auto it = std::find_if(subjects.begin(), subjects.end(), [&](const Subject& s) {
            return Validation::toUpperCase(s.getSubjectID()) == id;
        });

        if (it == subjects.end()) {
            std::cout << " [Error] Subject with ID '" << id << "' not found.\n";
            return;
        }

        std::cout << "\nCurrent Details:";
        it->displaySubject();

        std::string newName = Validation::getValidString("Enter New Subject Name: ", true);
        it->setSubjectName(newName);
        saveToFile();

        std::cout << "\n [Success] Subject updated successfully!\n";
        it->displaySubject();
    }

    bool deleteSubject() {
        std::cout << "\n========================================\n";
        std::cout << "             DELETE SUBJECT             \n";
        std::cout << "========================================\n";
        std::string id = Validation::getValidString("Enter Subject ID to delete: ", false);
        id = Validation::toUpperCase(id);

        auto it = std::find_if(subjects.begin(), subjects.end(), [&](const Subject& s) {
            return Validation::toUpperCase(s.getSubjectID()) == id;
        });

        if (it == subjects.end()) {
            std::cout << " [Error] Subject with ID '" << id << "' not found.\n";
            return false;
        }

        std::cout << "\nTarget Subject:";
        it->displaySubject();

        if (Validation::confirmAction("\nAre you sure you want to permanently delete this subject?")) {
            subjects.erase(it);
            saveToFile();
            std::cout << "\n [Success] Subject '" << id << "' has been deleted.\n";
            return true;
        } else {
            std::cout << " [Info] Deletion cancelled.\n";
            return false;
        }
    }

    void seedDefaultSubjects() {
        if (subjects.empty()) {
            subjects.push_back(Subject("SUB001", "Mathematics"));
            subjects.push_back(Subject("SUB002", "Programming"));
            subjects.push_back(Subject("SUB003", "Database Systems"));
            saveToFile();
        }
    }
};

#endif // SUBJECTSERVICE_H
