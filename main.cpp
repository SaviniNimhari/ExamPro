#include <iostream>
#include <string>
#include <memory>
#include "models/Student.h"
#include "models/Subject.h"
#include "models/Examination.h"
#include "models/Result.h"
#include "services/StudentService.h"
#include "services/SubjectService.h"
#include "services/ExamService.h"
#include "services/ResultService.h"
#include "utils/Validation.h"

// Submenu Handlers
void handleStudentMenu(StudentService& studentService) {
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "           STUDENT MANAGEMENT           \n";
        std::cout << "========================================\n";
        std::cout << " 1. Register Student\n";
        std::cout << " 2. View All Students\n";
        std::cout << " 3. Search Student\n";
        std::cout << " 4. Update Student\n";
        std::cout << " 5. Delete Student\n";
        std::cout << " 6. Back to Main Menu\n";
        std::cout << "========================================\n";
        int choice = Validation::getValidInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1:
                studentService.registerStudent();
                Validation::pauseConsole();
                break;
            case 2:
                studentService.viewAllStudents();
                Validation::pauseConsole();
                break;
            case 3:
                studentService.searchStudent();
                Validation::pauseConsole();
                break;
            case 4:
                studentService.updateStudent();
                Validation::pauseConsole();
                break;
            case 5:
                studentService.deleteStudent();
                Validation::pauseConsole();
                break;
            case 6:
                return;
        }
    }
}

void handleSubjectMenu(SubjectService& subjectService) {
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "           SUBJECT MANAGEMENT           \n";
        std::cout << "========================================\n";
        std::cout << " 1. Add Subject\n";
        std::cout << " 2. View All Subjects\n";
        std::cout << " 3. Search Subject\n";
        std::cout << " 4. Update Subject\n";
        std::cout << " 5. Delete Subject\n";
        std::cout << " 6. Back to Main Menu\n";
        std::cout << "========================================\n";
        int choice = Validation::getValidInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1:
                subjectService.addSubject();
                Validation::pauseConsole();
                break;
            case 2:
                subjectService.viewAllSubjects();
                Validation::pauseConsole();
                break;
            case 3:
                subjectService.searchSubject();
                Validation::pauseConsole();
                break;
            case 4:
                subjectService.updateSubject();
                Validation::pauseConsole();
                break;
            case 5:
                subjectService.deleteSubject();
                Validation::pauseConsole();
                break;
            case 6:
                return;
        }
    }
}

void handleExaminationMenu(ExamService& examService, const SubjectService& subjectService) {
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "         EXAMINATION MANAGEMENT         \n";
        std::cout << "========================================\n";
        std::cout << " 1. Create Examination\n";
        std::cout << " 2. View All Examinations\n";
        std::cout << " 3. Search Examination\n";
        std::cout << " 4. Assign Subjects to Examination\n";
        std::cout << " 5. View Examination Schedule\n";
        std::cout << " 6. Back to Main Menu\n";
        std::cout << "========================================\n";
        int choice = Validation::getValidInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1:
                examService.createExamination(subjectService);
                Validation::pauseConsole();
                break;
            case 2:
                examService.viewAllExaminations();
                Validation::pauseConsole();
                break;
            case 3:
                examService.searchExamination();
                Validation::pauseConsole();
                break;
            case 4:
                examService.assignSubjectsToExam(subjectService);
                Validation::pauseConsole();
                break;
            case 5:
                examService.viewExamSchedule(subjectService);
                Validation::pauseConsole();
                break;
            case 6:
                return;
        }
    }
}

void handleMarksMenu(ResultService& resultService,
                     const StudentService& studentService,
                     const SubjectService& subjectService,
                     const ExamService& examService) {
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "            MARKS MANAGEMENT            \n";
        std::cout << "========================================\n";
        std::cout << " 1. Enter Student Marks\n";
        std::cout << " 2. Update Student Marks\n";
        std::cout << " 3. View Marks by Subject\n";
        std::cout << " 4. View Marks by Student\n";
        std::cout << " 5. Back to Main Menu\n";
        std::cout << "========================================\n";
        int choice = Validation::getValidInt("Enter your choice (1-5): ", 1, 5);

        switch (choice) {
            case 1:
                resultService.enterStudentMarks(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 2:
                resultService.updateStudentMarks(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 3:
                resultService.viewMarksBySubject(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 4:
                resultService.viewMarksByStudent(studentService, subjectService);
                Validation::pauseConsole();
                break;
            case 5:
                return;
        }
    }
}

void handleResultsMenu(ResultService& resultService,
                       const StudentService& studentService,
                       const SubjectService& subjectService,
                       const ExamService& examService) {
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "           RESULTS MANAGEMENT           \n";
        std::cout << "========================================\n";
        std::cout << " 1. Calculate Results\n";
        std::cout << " 2. View Student Results\n";
        std::cout << " 3. View Examination Results\n";
        std::cout << " 4. View Top Performers\n";
        std::cout << " 5. View Failed Students\n";
        std::cout << " 6. Back to Main Menu\n";
        std::cout << "========================================\n";
        int choice = Validation::getValidInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1:
                resultService.viewExaminationResults(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 2:
                resultService.viewStudentResultsInteractive(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 3:
                resultService.viewExaminationResults(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 4:
                resultService.viewTopPerformers(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 5:
                resultService.viewFailedStudents(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 6:
                return;
        }
    }
}

void handleReportsMenu(ResultService& resultService,
                       const StudentService& studentService,
                       const SubjectService& subjectService,
                       const ExamService& examService) {
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "            GENERATE REPORTS            \n";
        std::cout << "========================================\n";
        std::cout << " 1. Export Student Marksheet / Report Card\n";
        std::cout << " 2. Export Examination Summary Report\n";
        std::cout << " 3. View Top Performers / Merit List\n";
        std::cout << " 4. Back to Main Menu\n";
        std::cout << "========================================\n";
        int choice = Validation::getValidInt("Enter your choice (1-4): ", 1, 4);

        switch (choice) {
            case 1:
                resultService.exportStudentReport(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 2:
                resultService.exportExamSummaryReport(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 3:
                resultService.viewTopPerformers(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 4:
                return;
        }
    }
}

int main() {
    // Initialize services & load data files
    StudentService studentService("data/students.txt");
    SubjectService subjectService("data/subjects.txt");
    ExamService examService("data/examinations.txt");
    ResultService resultService("data/results.txt");

    // Ensure sample data exists if first-time run
    studentService.seedDefaultStudents();
    subjectService.seedDefaultSubjects();
    examService.seedDefaultExams();
    resultService.seedDefaultMarks();

    while (true) {
        std::cout << "\n========================================\n";
        std::cout << " EXAMPRO EXAMINATION MANAGEMENT SYSTEM  \n";
        std::cout << "========================================\n";
        std::cout << " 1. Student Management\n";
        std::cout << " 2. Subject Management\n";
        std::cout << " 3. Examination Management\n";
        std::cout << " 4. Marks Management\n";
        std::cout << " 5. Results Management\n";
        std::cout << " 6. Search Student Results\n";
        std::cout << " 7. Generate Reports\n";
        std::cout << " 8. Exit\n";
        std::cout << "========================================\n";
        int choice = Validation::getValidInt("Enter your choice: ", 1, 8);

        switch (choice) {
            case 1:
                handleStudentMenu(studentService);
                break;
            case 2:
                handleSubjectMenu(subjectService);
                break;
            case 3:
                handleExaminationMenu(examService, subjectService);
                break;
            case 4:
                handleMarksMenu(resultService, studentService, subjectService, examService);
                break;
            case 5:
                handleResultsMenu(resultService, studentService, subjectService, examService);
                break;
            case 6:
                resultService.viewStudentResultsInteractive(studentService, subjectService, examService);
                Validation::pauseConsole();
                break;
            case 7:
                handleReportsMenu(resultService, studentService, subjectService, examService);
                break;
            case 8:
                std::cout << "\nSaving all records and exiting ExamPro. Goodbye!\n";
                studentService.saveToFile();
                subjectService.saveToFile();
                examService.saveToFile();
                resultService.saveToFile();
                return 0;
        }
    }
}
