#ifndef EXAMSERVICE_H
#define EXAMSERVICE_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "../models/Examination.h"
#include "../utils/FileHandler.h"
#include "../utils/Validation.h"
#include "SubjectService.h"

class ExamService {
private:
    std::vector<Examination> examinations;
    std::string filePath;

public:
    explicit ExamService(const std::string& path = "data/examinations.txt") : filePath(path) {
        loadFromFile();
    }

    void loadFromFile() {
        examinations.clear();
        std::vector<std::string> lines = FileHandler::readAllLines(filePath);
        for (const auto& line : lines) {
            Examination e = Examination::fromCSV(line);
            if (!e.getExamID().empty()) {
                examinations.push_back(e);
            }
        }
    }

    void saveToFile() const {
        std::vector<std::string> lines;
        for (const auto& e : examinations) {
            lines.push_back(e.toCSV());
        }
        FileHandler::writeAllLines(filePath, lines);
    }

    const std::vector<Examination>& getAllExams() const {
        return examinations;
    }

    bool exists(const std::string& id) const {
        std::string target = Validation::toUpperCase(id);
        for (const auto& e : examinations) {
            if (Validation::toUpperCase(e.getExamID()) == target) {
                return true;
            }
        }
        return false;
    }

    const Examination* getExam(const std::string& id) const {
        std::string target = Validation::toUpperCase(id);
        for (const auto& e : examinations) {
            if (Validation::toUpperCase(e.getExamID()) == target) {
                return &e;
            }
        }
        return nullptr;
    }

    std::string getExamName(const std::string& id) const {
        const Examination* e = getExam(id);
        return e ? e->getExamName() : ("Unknown Exam (" + id + ")");
    }

    void createExamination(const SubjectService& subjectService) {
        std::cout << "\n========================================\n";
        std::cout << "          CREATE EXAMINATION            \n";
        std::cout << "========================================\n";

        std::string id;
        while (true) {
            id = Validation::getValidString("Enter Examination ID (e.g. EX002): ", false);
            id = Validation::toUpperCase(id);
            if (exists(id)) {
                std::cout << " [Error] Examination ID '" << id << "' already exists! Please use a unique ID.\n";
            } else {
                break;
            }
        }

        std::string name = Validation::getValidString("Enter Examination Name: ", true);
        std::string date = Validation::getValidDate("Enter Examination Date (YYYY-MM-DD): ");

        Examination newExam(id, name, date);

        std::cout << "\nAssign subjects to this examination now?\n";
        if (Validation::confirmAction("Do you want to assign subjects?")) {
            const auto& allSubs = subjectService.getAllSubjects();
            if (allSubs.empty()) {
                std::cout << " [Warning] No subjects found in the system. Add subjects first in Subject Management.\n";
            } else {
                std::cout << "\nAvailable Subjects:\n";
                for (const auto& s : allSubs) {
                    std::cout << " - " << s.getSubjectID() << ": " << s.getSubjectName() << "\n";
                }

                while (true) {
                    std::string subId = Validation::getValidString("Enter Subject ID to assign (or 'done' to finish): ", false);
                    if (Validation::toUpperCase(subId) == "DONE") break;
                    subId = Validation::toUpperCase(subId);

                    if (!subjectService.exists(subId)) {
                        std::cout << " [Error] Subject '" << subId << "' does not exist in system!\n";
                        continue;
                    }

                    if (newExam.assignSubject(subId)) {
                        std::cout << " [Success] Subject '" << subId << "' assigned.\n";
                    } else {
                        std::cout << " [Notice] Subject is already assigned to this examination.\n";
                    }
                }
            }
        }

        examinations.push_back(newExam);
        saveToFile();

        std::cout << "\n [Success] Examination created successfully!\n";
        newExam.displayExam();
    }

    void viewAllExaminations() const {
        std::cout << "\n====================================================================================\n";
        std::cout << "                              EXAMINATION SCHEDULES                                 \n";
        std::cout << "====================================================================================\n";
        if (examinations.empty()) {
            std::cout << " No examinations scheduled.\n";
            std::cout << "====================================================================================\n";
            return;
        }

        std::cout << std::left 
                  << std::setw(12) << "Exam ID"
                  << std::setw(32) << "Examination Name"
                  << std::setw(15) << "Date"
                  << std::setw(20) << "Assigned Subjects"
                  << "\n";
        std::cout << "------------------------------------------------------------------------------------\n";

        for (const auto& e : examinations) {
            std::string subsJoined = "";
            const auto& subs = e.getSubjectIDs();
            for (size_t i = 0; i < subs.size(); ++i) {
                subsJoined += subs[i];
                if (i + 1 < subs.size()) subsJoined += ", ";
            }
            if (subsJoined.empty()) subsJoined = "None";

            std::cout << std::left
                      << std::setw(12) << e.getExamID()
                      << std::setw(32) << e.getExamName()
                      << std::setw(15) << e.getExamDate()
                      << std::setw(20) << subsJoined
                      << "\n";
        }
        std::cout << "====================================================================================\n";
        std::cout << " Total Examinations: " << examinations.size() << "\n";
    }

    void searchExamination() const {
        std::cout << "\n========================================\n";
        std::cout << "           SEARCH EXAMINATION           \n";
        std::cout << "========================================\n";
        std::string query = Validation::getValidString("Enter Examination ID or Name to search: ");
        std::string upperQuery = Validation::toUpperCase(query);

        std::vector<Examination> matches;
        for (const auto& e : examinations) {
            if (Validation::toUpperCase(e.getExamID()).find(upperQuery) != std::string::npos ||
                Validation::toUpperCase(e.getExamName()).find(upperQuery) != std::string::npos) {
                matches.push_back(e);
            }
        }

        if (matches.empty()) {
            std::cout << "\n [Info] No examinations matched '" << query << "'.\n";
        } else {
            std::cout << "\n Found " << matches.size() << " match(es):\n";
            for (const auto& m : matches) {
                m.displayExam();
                std::cout << "----------------------------------------\n";
            }
        }
    }

    void assignSubjectsToExam(const SubjectService& subjectService) {
        std::cout << "\n========================================\n";
        std::cout << "    ASSIGN SUBJECTS TO EXAMINATION      \n";
        std::cout << "========================================\n";
        std::string id = Validation::getValidString("Enter Examination ID: ", false);
        id = Validation::toUpperCase(id);

        auto it = std::find_if(examinations.begin(), examinations.end(), [&](const Examination& e) {
            return Validation::toUpperCase(e.getExamID()) == id;
        });

        if (it == examinations.end()) {
            std::cout << " [Error] Examination with ID '" << id << "' not found.\n";
            return;
        }

        std::cout << "\nCurrent Details:";
        it->displayExam();

        const auto& allSubs = subjectService.getAllSubjects();
        if (allSubs.empty()) {
            std::cout << " [Warning] No subjects found in system. Please add subjects first.\n";
            return;
        }

        std::cout << "\nAvailable Subjects in System:\n";
        for (const auto& s : allSubs) {
            bool alreadyAssigned = it->hasSubject(s.getSubjectID());
            std::cout << " - " << s.getSubjectID() << ": " << s.getSubjectName()
                      << (alreadyAssigned ? " [ASSIGNED]" : "") << "\n";
        }

        while (true) {
            std::string subId = Validation::getValidString("Enter Subject ID to assign (or 'done' to stop): ", false);
            if (Validation::toUpperCase(subId) == "DONE") break;
            subId = Validation::toUpperCase(subId);

            if (!subjectService.exists(subId)) {
                std::cout << " [Error] Subject ID '" << subId << "' does not exist!\n";
                continue;
            }

            if (it->assignSubject(subId)) {
                std::cout << " [Success] Subject '" << subId << "' assigned to " << it->getExamName() << ".\n";
            } else {
                std::cout << " [Notice] Subject is already assigned.\n";
            }
        }

        saveToFile();
        std::cout << "\nUpdated Examination Details:";
        it->displayExam();
    }

    void viewExamSchedule(const SubjectService& subjectService) const {
        std::cout << "\n====================================================================\n";
        std::cout << "                   DETAILED EXAMINATION SCHEDULES                   \n";
        std::cout << "====================================================================\n";
        if (examinations.empty()) {
            std::cout << " No examinations scheduled.\n";
            std::cout << "====================================================================\n";
            return;
        }

        for (const auto& e : examinations) {
            std::cout << " Examination: " << e.getExamName() << " (" << e.getExamID() << ")\n";
            std::cout << " Date       : " << e.getExamDate() << "\n";
            std::cout << " Subjects   :\n";
            const auto& subs = e.getSubjectIDs();
            if (subs.empty()) {
                std::cout << "   (No subjects assigned yet)\n";
            } else {
                for (const auto& subId : subs) {
                    std::cout << "   * " << std::left << std::setw(10) << subId 
                              << " - " << subjectService.getSubjectName(subId) << "\n";
                }
            }
            std::cout << "--------------------------------------------------------------------\n";
        }
    }

    void seedDefaultExams() {
        if (examinations.empty()) {
            std::vector<std::string> defaultSubs = {"SUB001", "SUB002", "SUB003"};
            examinations.push_back(Examination("EX001", "Midterm Examination 2026", "2026-06-15", defaultSubs));
            examinations.push_back(Examination("EX002", "Final Examination 2026", "2026-11-20", defaultSubs));
            saveToFile();
        }
    }
};

#endif // EXAMSERVICE_H
