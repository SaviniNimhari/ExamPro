#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

struct MarkRecord {
    std::string studentID;
    std::string examID;
    std::string subjectID;
    double marks;

    MarkRecord() : studentID(""), examID("EX001"), subjectID(""), marks(0.0) {}
    MarkRecord(const std::string& sId, const std::string& eId, const std::string& subId, double m)
        : studentID(sId), examID(eId), subjectID(subId), marks(m) {}

    std::string toCSV() const {
        std::stringstream ss;
        ss << studentID << "," << examID << "," << subjectID << "," << std::fixed << std::setprecision(1) << marks;
        return ss.str();
    }

    static MarkRecord fromCSV(const std::string& line) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            if (!token.empty() && token.back() == '\r') {
                token.pop_back();
            }
            tokens.push_back(token);
        }

        if (tokens.size() == 3) {
            // format: studentID, subjectID, marks
            double m = 0.0;
            try { m = std::stod(tokens[2]); } catch (...) {}
            return MarkRecord(tokens[0], "EX001", tokens[1], m);
        } else if (tokens.size() >= 4) {
            // format: studentID, examID, subjectID, marks
            double m = 0.0;
            try { m = std::stod(tokens[3]); } catch (...) {}
            return MarkRecord(tokens[0], tokens[1], tokens[2], m);
        }
        return MarkRecord();
    }
};

struct SubjectResultDetail {
    std::string subjectID;
    std::string subjectName;
    double marks;
    std::string grade;
    std::string status; // PASS / FAIL
};

class GradingPolicy {
public:
    static std::string calculateGrade(double marks) {
        if (marks >= 75.0) return "A";
        if (marks >= 65.0) return "B";
        if (marks >= 55.0) return "C";
        if (marks >= 35.0) return "S";
        return "F";
    }

    static std::string calculateStatus(double marks) {
        return (marks >= 35.0) ? "PASS" : "FAIL";
    }

    static std::string calculateStatusFromGrade(const std::string& grade) {
        return (grade == "F") ? "FAIL" : "PASS";
    }
};

struct StudentResultReport {
    std::string studentID;
    std::string studentName;
    std::string examID;
    std::string examName;
    std::vector<SubjectResultDetail> subjects;
    double totalMarks;
    double maxPossibleMarks;
    double averageMarks;
    std::string overallGrade;
    std::string finalStatus; // PASS or FAIL
    int rank;

    StudentResultReport()
        : totalMarks(0.0), maxPossibleMarks(0.0), averageMarks(0.0),
          overallGrade("F"), finalStatus("FAIL"), rank(0) {}
};

#endif // RESULT_H
