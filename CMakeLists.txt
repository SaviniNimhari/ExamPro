cmake_minimum_required(VERSION 3.15)
project(ExamPro VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# Source files
set(SOURCES
    main.cpp
)

# Header files for IDE navigation
set(HEADERS
    models/Student.h
    models/Subject.h
    models/Examination.h
    models/Result.h
    services/StudentService.h
    services/SubjectService.h
    services/ExamService.h
    services/ResultService.h
    utils/FileHandler.h
    utils/Validation.h
)

add_executable(ExamPro ${SOURCES} ${HEADERS})
