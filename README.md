# ExamPro – Examination Management System

**ExamPro** is an Object-Oriented C++ Examination Management System designed for educational institutions to manage students, subjects, examinations, marks, and academic results.

---

## 📌 Project Overview

- **Project Name:** ExamPro
- **Project Type:** Desktop / Console Application
- **Language:** C++ (C++17)
- **Architecture:** Layered Object-Oriented Architecture (Models, Services, Utils, Data)
- **Data Storage:** Structured Text Files (CSV), with persistent automatic synchronization
- **Supported Compilers / IDEs:** Visual Studio 2022 / 2019, Code::Blocks, MinGW GCC, Clang, CMake

---

## 📂 Project Structure

```text
ExamPro/
│
├── main.cpp                 # Main entry point & interactive menu routing
├── build.bat                # Automated Windows build script (auto-detects MSVC/MinGW)
├── run.bat                  # One-click build and execution script
├── CMakeLists.txt           # Cross-platform CMake configuration
├── README.md                # Project documentation and user manual
│
├── models/                  # Core domain entities
│   ├── Student.h            # Student class (ID, First Name, Last Name, Birth Year)
│   ├── Subject.h            # Subject class (ID, Name)
│   ├── Examination.h        # Examination class (ID, Name, Date, Assigned Subjects)
│   └── Result.h             # Result records, grading logic, and report cards
│
├── services/                # Business logic and operations
│   ├── StudentService.h     # Student registration, search, update, delete
│   ├── SubjectService.h     # Subject catalog management
│   ├── ExamService.h        # Exam schedules & subject assignments
│   └── ResultService.h      # Marks recording, grade calculations & analytics
│
├── utils/                   # Reusable utilities
│   ├── FileHandler.h        # File I/O and CSV serialization/deserialization
│   └── Validation.h         # Robust input sanitization and range checks
│
└── data/                    # Persistent text storage
    ├── students.txt         # Student database
    ├── subjects.txt         # Subject catalog
    ├── examinations.txt     # Examination schedules
    └── results.txt          # Student marks records
```

---

## 🎓 Grading System

The system evaluates student performance according to institutional grading guidelines:

| Marks Range | Grade | Status | Description |
| :---: | :---: | :---: | :--- |
| **75 – 100** | **A** | **Pass** | Distinction |
| **65 – 74**  | **B** | **Pass** | Very Good |
| **55 – 64**  | **C** | **Pass** | Credit Pass |
| **35 – 54**  | **S** | **Pass** | Simple Pass |
| **0 – 34**   | **F** | **Fail** | Failure / Retake Required |

> **Note:** A student passes overall if their average score is $\ge 35\%$ and they have passed all individual subjects. If any individual subject receives grade **F**, the overall status is **FAIL**.

---

## ⚙️ Core Modules & Features

### 1. Student Management
- **Register Student:** Adds a new student record (with uniqueness check on ID).
- **View All Students:** Displays an aligned tabular view of all registered students.
- **Search Student:** Find students by ID or name keyword.
- **Update Student:** Edit first name, last name, or birth year.
- **Delete Student:** Permanently removes a student after user confirmation.

### 2. Subject Management
- **Add Subject:** Creates unique subject entries (e.g., `SUB001`, `Mathematics`).
- **View All Subjects:** Displays subject lists.
- **Search & Update Subjects:** Modify subject details or search by keyword.
- **Delete Subject:** Remove subjects from the institution's curriculum.

### 3. Examination Management
- **Create Examination:** Create exams with IDs, titles, and dates (`YYYY-MM-DD`).
- **Assign Subjects:** Link one or multiple subjects to specific examinations.
- **View Schedules:** Comprehensive view of upcoming examinations and their subjects.

### 4. Marks Management
- **Enter Student Marks:** Record marks (0.0 to 100.0) with validation against existing students, exams, and subjects.
- **Update Marks:** Correct marks for existing records.
- **View Marks by Subject:** Audit all student scores for a given subject.
- **View Marks by Student:** Display a student's marks across all examinations.

### 5. Results & Analytics
- **Calculate Results:** Generates overall marks, class averages, and individual grades.
- **View Examination Results:** Ranked leaderboard of all candidates in an examination, with class average and pass percentage.
- **View Top Performers:** Filter and inspect the highest-ranking students.
- **View Failed Students:** Identify students who scored below 35% or failed any subject.

### 6. Search Student Results
- Instant report card lookup by Student ID and Exam ID.

### 7. Report Generation & Export
- **Export Marksheet:** Generates and saves a formatted student result report to `data/report_<StudentID>_<ExamID>.txt`.
- **Export Exam Summary:** Generates and saves an analytical class report to `data/exam_summary_<ExamID>.txt`.

---

## 📊 Sample Calculation & Output

Given marks:
- **Mathematics:** 85.0
- **Programming:** 78.0
- **Database Systems:** 92.0

$$\text{Total Marks} = 85.0 + 78.0 + 92.0 = 255.0$$
$$\text{Average Marks} = \frac{255.0}{3} = 85.0\%$$

```text
============================================================
              EXAMPRO - STUDENT RESULT REPORT               
============================================================
 Student ID   : ST001
 Student Name : Savini Peramuna
 Examination  : Midterm Examination 2026 (EX001)
 Class Rank   : 1
------------------------------------------------------------
Subject                     Marks       Grade     Status    
------------------------------------------------------------
Mathematics                 85.00       A         PASS      
Programming                 78.00       A         PASS      
Database Systems            92.00       A         PASS      
------------------------------------------------------------
 Total Marks   : 255.00 / 300
 Average Marks : 85.00%
 Overall Grade : A
 Final Status  : PASS
============================================================
```

---

## 🛠️ How to Build and Run

### Option 1: Quick Run (Batch Script - Recommended on Windows)
Simply double-click or run:
```cmd
run.bat
```
*(If `ExamPro.exe` does not exist yet, `run.bat` automatically invokes `build.bat` to compile it using Visual Studio MSVC or MinGW GCC, and then launches the application).*

### Option 2: Build with `build.bat`
```cmd
build.bat
```

### Option 3: Compile via Command Line

**Using Microsoft Visual C++ (MSVC / Developer Command Prompt):**
```cmd
cl.exe /EHsc /std:c++17 main.cpp /Fe:ExamPro.exe
ExamPro.exe
```

**Using MinGW GCC (g++):**
```bash
g++ -std=c++17 -Wall -O2 main.cpp -o ExamPro.exe
./ExamPro.exe
```

### Option 4: Visual Studio 2022 IDE
1. Open Visual Studio 2022.
2. Select **Open a local folder** and choose `ExamPro`.
3. Visual Studio will automatically detect `CMakeLists.txt` or `main.cpp`.
4. Click **Run** (`F5` or `Ctrl + F5`).

### Option 5: Code::Blocks
1. Open Code::Blocks.
2. Go to **File -> New -> Project -> Console Application -> C++**.
3. Choose the `ExamPro` folder.
4. Add `main.cpp` and header files under `models/`, `services/`, and `utils/`.
5. Ensure compiler flag `-std=c++17` is enabled under **Project Build Options**.
6. Click **Build and Run** (`F9`).

---

## 💡 C++ Concepts Demonstrated

- **Encapsulation:** Private data members with public getters, setters, and operational methods.
- **Class Composition:** High-level services orchestrating models and utilities.
- **Vectors & Standard Library Algorithms:** Dynamic collections, `std::find_if`, `std::sort`, lambda predicates.
- **Persistent File I/O:** Reading, writing, and CSV parsing via `std::ifstream`, `std::ofstream`, and `std::stringstream`.
- **Defensive Programming & Validation:** Stream error recovery, range checks, regex validation for dates, and input sanitization to eliminate crashes or infinite loops.
- **Memory Safety & Best Practices:** Pass-by-const-reference, `const`-correctness on inquiry methods.
