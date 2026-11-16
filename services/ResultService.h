#ifndef RESULTSERVICE_H
#define RESULTSERVICE_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "../models/Result.h"
#include "../utils/FileHandler.h"
#include "../utils/Validation.h"
#include "StudentService.h"
#include "SubjectService.h"
#include "ExamService.h"

class ResultService {
private:
    std::vector<MarkRecord> records;
    std::string filePath;

public:
    explicit ResultService(const std::string& path = "data/results.txt") : filePath(path) {
        loadFromFile();
    }

    void loadFromFile() {
        records.clear();
        std::vector<std::string> lines = FileHandler::readAllLines(filePath);
        for (const auto& line : lines) {
            MarkRecord r = MarkRecord::fromCSV(line);
            if (!r.studentID.empty() && !r.subjectID.empty()) {
                records.push_back(r);
            }
        }
    }

    void saveToFile() const {
        std::vector<std::string> lines;
        for (const auto& r : records) {
            lines.push_back(r.toCSV());
        }
        FileHandler::writeAllLines(filePath, lines);
    }

    // Find record iterator
    auto findRecord(const std::string& studentId, const std::string& examId, const std::string& subjectId) {
        std::string sTarget = Validation::toUpperCase(studentId);
        std::string eTarget = Validation::toUpperCase(examId);
        std::string subTarget = Validation::toUpperCase(subjectId);

        return std::find_if(records.begin(), records.end(), [&](const MarkRecord& r) {
            return Validation::toUpperCase(r.studentID) == sTarget &&
                   Validation::toUpperCase(r.examID) == eTarget &&
                   Validation::toUpperCase(r.subjectID) == subTarget;
        });
    }

    // ==========================================
    // 4. MARKS MANAGEMENT
    // ==========================================

    void enterStudentMarks(const StudentService& studentService, 
                           const SubjectService& subjectService, 
                           const ExamService& examService) {
        std::cout << "\n========================================\n";
        std::cout << "           ENTER STUDENT MARKS          \n";
        std::cout << "========================================\n";

        // Student selection
        std::string studentId = Validation::getValidString("Enter Student ID: ", false);
        studentId = Validation::toUpperCase(studentId);
        const Student* student = studentService.getStudent(studentId);
        if (!student) {
            std::cout << " [Error] Student with ID '" << studentId << "' does not exist! Register student first.\n";
            return;
        }
        std::cout << " Selected Student: " << student->getFullName() << " (" << studentId << ")\n";

        // Exam selection
        std::string examId = Validation::getValidString("Enter Examination ID (e.g. EX001): ", false);
        examId = Validation::toUpperCase(examId);
        const Examination* exam = examService.getExam(examId);
        if (!exam) {
            std::cout << " [Error] Examination with ID '" << examId << "' does not exist! Create exam first.\n";
            return;
        }
        std::cout << " Selected Exam: " << exam->getExamName() << " (" << examId << ")\n";

        // Subject selection
        std::cout << "\nAssigned subjects for this examination:\n";
        const auto& assignedSubs = exam->getSubjectIDs();
        if (assignedSubs.empty()) {
            std::cout << " [Warning] No subjects assigned to this exam. Showing all system subjects.\n";
        } else {
            for (const auto& sId : assignedSubs) {
                std::cout << " - " << sId << ": " << subjectService.getSubjectName(sId) << "\n";
            }
        }

        std::string subId = Validation::getValidString("Enter Subject ID: ", false);
        subId = Validation::toUpperCase(subId);
        if (!subjectService.exists(subId)) {
            std::cout << " [Error] Subject with ID '" << subId << "' does not exist!\n";
            return;
        }

        double marks = Validation::getValidDouble("Enter Marks (0 - 100): ", 0.0, 100.0);

        auto it = findRecord(studentId, examId, subId);
        if (it != records.end()) {
            std::cout << " [Notice] Record already exists with " << it->marks << " marks.\n";
            if (Validation::confirmAction("Do you want to update/overwrite with " + std::to_string(marks) + "?")) {
                it->marks = marks;
                saveToFile();
                std::cout << " [Success] Marks updated successfully!\n";
            } else {
                std::cout << " [Info] Operation cancelled.\n";
            }
        } else {
            records.push_back(MarkRecord(studentId, examId, subId, marks));
            saveToFile();
            std::cout << " [Success] Marks saved successfully!\n";
        }
    }

    void updateStudentMarks(const StudentService& studentService, 
                            const SubjectService& subjectService, 
                            const ExamService& examService) {
        std::cout << "\n========================================\n";
        std::cout << "          UPDATE STUDENT MARKS          \n";
        std::cout << "========================================\n";

        std::string studentId = Validation::getValidString("Enter Student ID: ", false);
        studentId = Validation::toUpperCase(studentId);

        std::string examId = Validation::getValidString("Enter Examination ID: ", false);
        examId = Validation::toUpperCase(examId);

        std::string subId = Validation::getValidString("Enter Subject ID: ", false);
        subId = Validation::toUpperCase(subId);

        auto it = findRecord(studentId, examId, subId);
        if (it == records.end()) {
            std::cout << " [Error] No existing marks found for Student '" << studentId 
                      << "' in Exam '" << examId << "' for Subject '" << subId << "'.\n";
            return;
        }

        std::cout << "\nCurrent Marks: " << std::fixed << std::setprecision(2) << it->marks << " / 100\n";
        double newMarks = Validation::getValidDouble("Enter New Marks (0 - 100): ", 0.0, 100.0);

        it->marks = newMarks;
        saveToFile();
        std::cout << "\n [Success] Marks updated successfully to " << newMarks << "!\n";
    }

    void viewMarksBySubject(const StudentService& studentService, 
                            const SubjectService& subjectService,
                            const ExamService& examService) const {
        std::cout << "\n========================================\n";
        std::cout << "         VIEW MARKS BY SUBJECT          \n";
        std::cout << "========================================\n";
        std::string subId = Validation::getValidString("Enter Subject ID: ", false);
        subId = Validation::toUpperCase(subId);

        if (!subjectService.exists(subId)) {
            std::cout << " [Error] Subject with ID '" << subId << "' does not exist.\n";
            return;
        }

        std::string examId = Validation::getValidString("Enter Examination ID (or press Enter for all): ", true);
        examId = Validation::trim(examId);
        examId = Validation::toUpperCase(examId);

        std::cout << "\n========================================================================================\n";
        std::cout << " Subject: " << subjectService.getSubjectName(subId) << " (" << subId << ")\n";
        if (!examId.empty()) {
            std::cout << " Examination: " << examService.getExamName(examId) << " (" << examId << ")\n";
        }
        std::cout << "========================================================================================\n";
        std::cout << std::left
                  << std::setw(12) << "Student ID"
                  << std::setw(24) << "Student Name"
                  << std::setw(12) << "Exam ID"
                  << std::setw(10) << "Marks"
                  << std::setw(8)  << "Grade"
                  << std::setw(10) << "Status"
                  << "\n";
        std::cout << "----------------------------------------------------------------------------------------\n";

        int count = 0;
        double sum = 0.0;
        for (const auto& r : records) {
            if (Validation::toUpperCase(r.subjectID) == subId) {
                if (!examId.empty() && Validation::toUpperCase(r.examID) != examId) {
                    continue;
                }
                const Student* s = studentService.getStudent(r.studentID);
                std::string sName = s ? s->getFullName() : "Unknown";
                std::string grade = GradingPolicy::calculateGrade(r.marks);
                std::string status = GradingPolicy::calculateStatus(r.marks);

                std::cout << std::left
                          << std::setw(12) << r.studentID
                          << std::setw(24) << sName
                          << std::setw(12) << r.examID
                          << std::setw(10) << std::fixed << std::setprecision(2) << r.marks
                          << std::setw(8)  << grade
                          << std::setw(10) << status
                          << "\n";
                count++;
                sum += r.marks;
            }
        }

        std::cout << "========================================================================================\n";
        if (count == 0) {
            std::cout << " No student marks recorded for this subject.\n";
        } else {
            std::cout << " Total Students: " << count << " | Subject Average: " 
                      << std::fixed << std::setprecision(2) << (sum / count) << "\n";
        }
    }

    void viewMarksByStudent(const StudentService& studentService, 
                            const SubjectService& subjectService) const {
        std::cout << "\n========================================\n";
        std::cout << "         VIEW MARKS BY STUDENT          \n";
        std::cout << "========================================\n";
        std::string studentId = Validation::getValidString("Enter Student ID: ", false);
        studentId = Validation::toUpperCase(studentId);

        const Student* s = studentService.getStudent(studentId);
        if (!s) {
            std::cout << " [Error] Student with ID '" << studentId << "' not found.\n";
            return;
        }

        std::cout << "\n Student: " << s->getFullName() << " (" << studentId << ")\n";
        std::cout << "========================================================================================\n";
        std::cout << std::left
                  << std::setw(12) << "Exam ID"
                  << std::setw(12) << "Subject ID"
                  << std::setw(28) << "Subject Name"
                  << std::setw(10) << "Marks"
                  << std::setw(8)  << "Grade"
                  << std::setw(10) << "Status"
                  << "\n";
        std::cout << "----------------------------------------------------------------------------------------\n";

        int count = 0;
        for (const auto& r : records) {
            if (Validation::toUpperCase(r.studentID) == studentId) {
                std::string subName = subjectService.getSubjectName(r.subjectID);
                std::string grade = GradingPolicy::calculateGrade(r.marks);
                std::string status = GradingPolicy::calculateStatus(r.marks);

                std::cout << std::left
                          << std::setw(12) << r.examID
                          << std::setw(12) << r.subjectID
                          << std::setw(28) << subName
                          << std::setw(10) << std::fixed << std::setprecision(2) << r.marks
                          << std::setw(8)  << grade
                          << std::setw(10) << status
                          << "\n";
                count++;
            }
        }
        std::cout << "========================================================================================\n";
        if (count == 0) {
            std::cout << " No marks found for this student.\n";
        }
    }

    // ==========================================
    // 5. RESULTS MANAGEMENT & CALCULATIONS
    // ==========================================

    StudentResultReport calculateStudentReport(const std::string& studentId, 
                                              const std::string& examId,
                                              const StudentService& studentService, 
                                              const SubjectService& subjectService, 
                                              const ExamService& examService) const {
        StudentResultReport report;
        report.studentID = Validation::toUpperCase(studentId);
        report.examID = Validation::toUpperCase(examId);

        const Student* s = studentService.getStudent(report.studentID);
        report.studentName = s ? s->getFullName() : "Unknown Student";

        const Examination* e = examService.getExam(report.examID);
        report.examName = e ? e->getExamName() : "Unknown Examination";

        double total = 0.0;
        bool hasFailedSubject = false;

        for (const auto& r : records) {
            if (Validation::toUpperCase(r.studentID) == report.studentID &&
                Validation::toUpperCase(r.examID) == report.examID) {
                SubjectResultDetail detail;
                detail.subjectID = r.subjectID;
                detail.subjectName = subjectService.getSubjectName(r.subjectID);
                detail.marks = r.marks;
                detail.grade = GradingPolicy::calculateGrade(r.marks);
                detail.status = GradingPolicy::calculateStatus(r.marks);

                if (detail.grade == "F") {
                    hasFailedSubject = true;
                }

                report.subjects.push_back(detail);
                total += r.marks;
            }
        }

        report.totalMarks = total;
        report.maxPossibleMarks = report.subjects.size() * 100.0;
        if (!report.subjects.empty()) {
            report.averageMarks = total / report.subjects.size();
            report.overallGrade = GradingPolicy::calculateGrade(report.averageMarks);
            if (hasFailedSubject || report.overallGrade == "F") {
                report.finalStatus = "FAIL";
            } else {
                report.finalStatus = "PASS";
            }
        } else {
            report.averageMarks = 0.0;
            report.overallGrade = "N/A";
            report.finalStatus = "N/A";
        }

        return report;
    }

    std::vector<StudentResultReport> calculateAllReportsForExam(const std::string& examId,
                                                              const StudentService& studentService,
                                                              const SubjectService& subjectService,
                                                              const ExamService& examService) const {
        std::vector<StudentResultReport> reports;
        std::string eTarget = Validation::toUpperCase(examId);

        // Gather all distinct students who have marks in this exam
        std::vector<std::string> studentIds;
        for (const auto& r : records) {
            if (Validation::toUpperCase(r.examID) == eTarget) {
                std::string sId = Validation::toUpperCase(r.studentID);
                if (std::find(studentIds.begin(), studentIds.end(), sId) == studentIds.end()) {
                    studentIds.push_back(sId);
                }
            }
        }

        for (const auto& sId : studentIds) {
            reports.push_back(calculateStudentReport(sId, examId, studentService, subjectService, examService));
        }

        // Sort descending by average marks for ranking
        std::sort(reports.begin(), reports.end(), [](const StudentResultReport& a, const StudentResultReport& b) {
            return a.averageMarks > b.averageMarks;
        });

        for (size_t i = 0; i < reports.size(); ++i) {
            reports[i].rank = static_cast<int>(i + 1);
        }

        return reports;
    }

    std::string formatReportCardString(const StudentResultReport& report) const {
        std::ostringstream oss;
        oss << "============================================================\n";
        oss << "              EXAMPRO - STUDENT RESULT REPORT               \n";
        oss << "============================================================\n";
        oss << " Student ID   : " << report.studentID << "\n";
        oss << " Student Name : " << report.studentName << "\n";
        oss << " Examination  : " << report.examName << " (" << report.examID << ")\n";
        if (report.rank > 0) {
            oss << " Class Rank   : " << report.rank << "\n";
        }
        oss << "------------------------------------------------------------\n";
        oss << std::left 
            << std::setw(28) << "Subject"
            << std::setw(12) << "Marks"
            << std::setw(10) << "Grade"
            << std::setw(10) << "Status"
            << "\n";
        oss << "------------------------------------------------------------\n";

        for (const auto& sub : report.subjects) {
            oss << std::left
                << std::setw(28) << sub.subjectName
                << std::setw(12) << std::fixed << std::setprecision(2) << sub.marks
                << std::setw(10) << sub.grade
                << std::setw(10) << sub.status
                << "\n";
        }

        oss << "------------------------------------------------------------\n";
        oss << " Total Marks   : " << std::fixed << std::setprecision(2) << report.totalMarks 
            << " / " << static_cast<int>(report.maxPossibleMarks) << "\n";
        oss << " Average Marks : " << std::fixed << std::setprecision(2) << report.averageMarks << "%\n";
        oss << " Overall Grade : " << report.overallGrade << "\n";
        oss << " Final Status  : " << report.finalStatus << "\n";
        oss << "============================================================\n";
        return oss.str();
    }

    void displayReportCard(const StudentResultReport& report) const {
        std::cout << "\n" << formatReportCardString(report);
    }

    void viewStudentResultsInteractive(const StudentService& studentService,
                                       const SubjectService& subjectService,
                                       const ExamService& examService) const {
        std::cout << "\n========================================\n";
        std::cout << "          VIEW STUDENT RESULTS          \n";
        std::cout << "========================================\n";

        std::string studentId = Validation::getValidString("Enter Student ID: ", false);
        studentId = Validation::toUpperCase(studentId);

        const Student* s = studentService.getStudent(studentId);
        if (!s) {
            std::cout << " [Error] Student with ID '" << studentId << "' does not exist.\n";
            return;
        }

        std::string examId = Validation::getValidString("Enter Examination ID (e.g. EX001): ", false);
        examId = Validation::toUpperCase(examId);

        StudentResultReport report = calculateStudentReport(studentId, examId, studentService, subjectService, examService);
        if (report.subjects.empty()) {
            std::cout << " [Info] No marks recorded for Student '" << studentId << "' in Exam '" << examId << "'.\n";
            return;
        }

        // Rank the student in the exam
        auto allReports = calculateAllReportsForExam(examId, studentService, subjectService, examService);
        for (const auto& r : allReports) {
            if (r.studentID == studentId) {
                report.rank = r.rank;
                break;
            }
        }

        displayReportCard(report);
    }

    void viewExaminationResults(const StudentService& studentService,
                                const SubjectService& subjectService,
                                const ExamService& examService) const {
        std::cout << "\n========================================\n";
        std::cout << "        VIEW EXAMINATION RESULTS        \n";
        std::cout << "========================================\n";
        std::string examId = Validation::getValidString("Enter Examination ID: ", false);
        examId = Validation::toUpperCase(examId);

        auto reports = calculateAllReportsForExam(examId, studentService, subjectService, examService);
        if (reports.empty()) {
            std::cout << " [Info] No results found for Examination '" << examId << "'.\n";
            return;
        }

        std::cout << "\n====================================================================================================\n";
        std::cout << " RESULTS OVERVIEW: " << examService.getExamName(examId) << " (" << examId << ")\n";
        std::cout << "====================================================================================================\n";
        std::cout << std::left
                  << std::setw(6)  << "Rank"
                  << std::setw(12) << "Student ID"
                  << std::setw(24) << "Name"
                  << std::setw(14) << "Total Marks"
                  << std::setw(12) << "Average"
                  << std::setw(8)  << "Grade"
                  << std::setw(10) << "Status"
                  << "\n";
        std::cout << "----------------------------------------------------------------------------------------------------\n";

        int passCount = 0;
        int failCount = 0;
        double classAvgSum = 0.0;

        for (const auto& r : reports) {
            std::cout << std::left
                      << std::setw(6)  << r.rank
                      << std::setw(12) << r.studentID
                      << std::setw(24) << r.studentName
                      << std::setw(14) << (std::to_string(static_cast<int>(r.totalMarks)) + " / " + std::to_string(static_cast<int>(r.maxPossibleMarks)))
                      << std::setw(12) << std::fixed << std::setprecision(2) << r.averageMarks
                      << std::setw(8)  << r.overallGrade
                      << std::setw(10) << r.finalStatus
                      << "\n";

            if (r.finalStatus == "PASS") passCount++;
            else failCount++;
            classAvgSum += r.averageMarks;
        }

        std::cout << "====================================================================================================\n";
        std::cout << " Total Students: " << reports.size() 
                  << " | Passed: " << passCount 
                  << " | Failed: " << failCount 
                  << " | Pass Rate: " << std::fixed << std::setprecision(1) << (passCount * 100.0 / reports.size()) << "%"
                  << " | Class Average: " << std::fixed << std::setprecision(2) << (classAvgSum / reports.size()) << "%\n";
    }

    void viewTopPerformers(const StudentService& studentService,
                           const SubjectService& subjectService,
                           const ExamService& examService) const {
        std::cout << "\n========================================\n";
        std::cout << "          VIEW TOP PERFORMERS           \n";
        std::cout << "========================================\n";
        std::string examId = Validation::getValidString("Enter Examination ID: ", false);
        examId = Validation::toUpperCase(examId);

        auto reports = calculateAllReportsForExam(examId, studentService, subjectService, examService);
        if (reports.empty()) {
            std::cout << " [Info] No results found for Examination '" << examId << "'.\n";
            return;
        }

        int count = Validation::getValidInt("How many top performers to view? (1-10): ", 1, 10);
        int limit = std::min(count, static_cast<int>(reports.size()));

        std::cout << "\n====================================================================================\n";
        std::cout << "                     TOP " << limit << " PERFORMERS: " << examService.getExamName(examId) << "\n";
        std::cout << "====================================================================================\n";
        std::cout << std::left
                  << std::setw(6)  << "Rank"
                  << std::setw(12) << "Student ID"
                  << std::setw(26) << "Name"
                  << std::setw(14) << "Average"
                  << std::setw(10) << "Grade"
                  << std::setw(10) << "Status"
                  << "\n";
        std::cout << "------------------------------------------------------------------------------------\n";

        for (int i = 0; i < limit; ++i) {
            const auto& r = reports[i];
            std::cout << std::left
                      << std::setw(6)  << r.rank
                      << std::setw(12) << r.studentID
                      << std::setw(26) << r.studentName
                      << std::setw(14) << std::fixed << std::setprecision(2) << r.averageMarks
                      << std::setw(10) << r.overallGrade
                      << std::setw(10) << r.finalStatus
                      << "\n";
        }
        std::cout << "====================================================================================\n";
    }

    void viewFailedStudents(const StudentService& studentService,
                            const SubjectService& subjectService,
                            const ExamService& examService) const {
        std::cout << "\n========================================\n";
        std::cout << "          VIEW FAILED STUDENTS          \n";
        std::cout << "========================================\n";
        std::string examId = Validation::getValidString("Enter Examination ID: ", false);
        examId = Validation::toUpperCase(examId);

        auto reports = calculateAllReportsForExam(examId, studentService, subjectService, examService);
        if (reports.empty()) {
            std::cout << " [Info] No results found for Examination '" << examId << "'.\n";
            return;
        }

        std::vector<StudentResultReport> failedList;
        for (const auto& r : reports) {
            if (r.finalStatus == "FAIL") {
                failedList.push_back(r);
            }
        }

        std::cout << "\n====================================================================================\n";
        std::cout << "                     FAILED STUDENTS: " << examService.getExamName(examId) << "\n";
        std::cout << "====================================================================================\n";
        if (failedList.empty()) {
            std::cout << " Great news! No students failed in this examination.\n";
            std::cout << "====================================================================================\n";
            return;
        }

        std::cout << std::left
                  << std::setw(12) << "Student ID"
                  << std::setw(24) << "Name"
                  << std::setw(12) << "Average"
                  << std::setw(10) << "Grade"
                  << std::setw(26) << "Failed Subjects"
                  << "\n";
        std::cout << "------------------------------------------------------------------------------------\n";

        for (const auto& r : failedList) {
            std::string failedSubs = "";
            for (const auto& s : r.subjects) {
                if (s.grade == "F") {
                    if (!failedSubs.empty()) failedSubs += ", ";
                    failedSubs += s.subjectID + "(" + std::to_string(static_cast<int>(s.marks)) + ")";
                }
            }
            if (failedSubs.empty() && r.averageMarks < 35.0) {
                failedSubs = "Low Avg (" + std::to_string(static_cast<int>(r.averageMarks)) + "%)";
            }

            std::cout << std::left
                      << std::setw(12) << r.studentID
                      << std::setw(24) << r.studentName
                      << std::setw(12) << std::fixed << std::setprecision(2) << r.averageMarks
                      << std::setw(10) << r.overallGrade
                      << std::setw(26) << failedSubs
                      << "\n";
        }
        std::cout << "====================================================================================\n";
        std::cout << " Total Failed Students: " << failedList.size() << "\n";
    }

    // ==========================================
    // 7. REPORT GENERATION / EXPORT
    // ==========================================

    void exportStudentReport(const StudentService& studentService,
                            const SubjectService& subjectService,
                            const ExamService& examService) const {
        std::cout << "\n========================================\n";
        std::cout << "      EXPORT STUDENT RESULT REPORT      \n";
        std::cout << "========================================\n";

        std::string studentId = Validation::getValidString("Enter Student ID: ", false);
        studentId = Validation::toUpperCase(studentId);

        std::string examId = Validation::getValidString("Enter Examination ID: ", false);
        examId = Validation::toUpperCase(examId);

        StudentResultReport report = calculateStudentReport(studentId, examId, studentService, subjectService, examService);
        if (report.subjects.empty()) {
            std::cout << " [Error] No records found for Student '" << studentId << "' in Exam '" << examId << "'.\n";
            return;
        }

        auto allReports = calculateAllReportsForExam(examId, studentService, subjectService, examService);
        for (const auto& r : allReports) {
            if (r.studentID == studentId) {
                report.rank = r.rank;
                break;
            }
        }

        std::string content = formatReportCardString(report);
        std::string outPath = "data/report_" + studentId + "_" + examId + ".txt";

        std::ofstream outFile(outPath);
        if (outFile.is_open()) {
            outFile << content;
            outFile.close();
            std::cout << "\n [Success] Report card generated and saved to: " << outPath << "\n";
            std::cout << content << "\n";
        } else {
            std::cout << " [Error] Could not save report file to " << outPath << "\n";
        }
    }

    void exportExamSummaryReport(const StudentService& studentService,
                                const SubjectService& subjectService,
                                const ExamService& examService) const {
        std::cout << "\n========================================\n";
        std::cout << "      EXPORT EXAM SUMMARY REPORT        \n";
        std::cout << "========================================\n";

        std::string examId = Validation::getValidString("Enter Examination ID: ", false);
        examId = Validation::toUpperCase(examId);

        auto reports = calculateAllReportsForExam(examId, studentService, subjectService, examService);
        if (reports.empty()) {
            std::cout << " [Error] No results found for Examination '" << examId << "'.\n";
            return;
        }

        std::ostringstream oss;
        oss << "====================================================================================================\n";
        oss << "                             EXAMPRO - EXAMINATION SUMMARY REPORT                                   \n";
        oss << "====================================================================================================\n";
        oss << " Examination : " << examService.getExamName(examId) << " (" << examId << ")\n";
        oss << " Exam Date   : " << (examService.getExam(examId) ? examService.getExam(examId)->getExamDate() : "N/A") << "\n";
        oss << " Total Candidates : " << reports.size() << "\n";
        oss << "----------------------------------------------------------------------------------------------------\n";
        oss << std::left
            << std::setw(6)  << "Rank"
            << std::setw(12) << "Student ID"
            << std::setw(24) << "Name"
            << std::setw(14) << "Total Marks"
            << std::setw(12) << "Average"
            << std::setw(8)  << "Grade"
            << std::setw(10) << "Status"
            << "\n";
        oss << "----------------------------------------------------------------------------------------------------\n";

        int passCount = 0;
        int failCount = 0;
        double classAvgSum = 0.0;

        for (const auto& r : reports) {
            oss << std::left
                << std::setw(6)  << r.rank
                << std::setw(12) << r.studentID
                << std::setw(24) << r.studentName
                << std::setw(14) << (std::to_string(static_cast<int>(r.totalMarks)) + " / " + std::to_string(static_cast<int>(r.maxPossibleMarks)))
                << std::setw(12) << std::fixed << std::setprecision(2) << r.averageMarks
                << std::setw(8)  << r.overallGrade
                << std::setw(10) << r.finalStatus
                << "\n";

            if (r.finalStatus == "PASS") passCount++;
            else failCount++;
            classAvgSum += r.averageMarks;
        }

        oss << "----------------------------------------------------------------------------------------------------\n";
        oss << " SUMMARY STATISTICS:\n";
        oss << " - Total Students Evaluated: " << reports.size() << "\n";
        oss << " - Total Passed: " << passCount << " (" << std::fixed << std::setprecision(1) << (passCount * 100.0 / reports.size()) << "%)\n";
        oss << " - Total Failed: " << failCount << " (" << std::fixed << std::setprecision(1) << (failCount * 100.0 / reports.size()) << "%)\n";
        oss << " - Overall Class Average: " << std::fixed << std::setprecision(2) << (classAvgSum / reports.size()) << "%\n";
        oss << "====================================================================================================\n";

        std::string outPath = "data/exam_summary_" + examId + ".txt";
        std::ofstream outFile(outPath);
        if (outFile.is_open()) {
            outFile << oss.str();
            outFile.close();
            std::cout << "\n [Success] Summary report generated and saved to: " << outPath << "\n";
            std::cout << oss.str() << "\n";
        } else {
            std::cout << " [Error] Could not save report file to " << outPath << "\n";
        }
    }

    void seedDefaultMarks() {
        if (records.empty()) {
            // ST001: Savini Peramuna (Sample from section 10: 85, 78, 92 -> Total 255, Avg 85.00)
            records.push_back(MarkRecord("ST001", "EX001", "SUB001", 85.0));
            records.push_back(MarkRecord("ST001", "EX001", "SUB002", 78.0));
            records.push_back(MarkRecord("ST001", "EX001", "SUB003", 92.0));

            // ST002: Kamal Perera
            records.push_back(MarkRecord("ST002", "EX001", "SUB001", 68.0));
            records.push_back(MarkRecord("ST002", "EX001", "SUB002", 72.0));
            records.push_back(MarkRecord("ST002", "EX001", "SUB003", 64.0));

            // ST003: Nimal Silva
            records.push_back(MarkRecord("ST003", "EX001", "SUB001", 42.0));
            records.push_back(MarkRecord("ST003", "EX001", "SUB002", 30.0)); // Fail in SUB002
            records.push_back(MarkRecord("ST003", "EX001", "SUB003", 55.0));

            saveToFile();
        }
    }
};

#endif // RESULTSERVICE_H
