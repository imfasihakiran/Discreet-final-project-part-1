# Discreet-final-project-part-1
Member 1 — Course & Prerequisite Management Module

As Member 1, my contribution to the project focuses on building the Course & Prerequisite Management System, which forms the foundation of the overall University Management application.
This module provides all essential academic-structure features required for organizing courses, tracking prerequisites, and supporting advanced discrete-structure operations.

🔹 Key Features I Developed
1. Course Management

Add new courses into the university system.

Maintain a clean list of all available courses.

2. Prerequisite Management

Link a course with its required prerequisites.

View prerequisite chains for each course.

Ensures valid dependencies and prevents invalid links.

3. Student Completion Tracking

Add students in the system (Module 1 part).

Mark or unmark courses that a student has completed.

Maintain a record of student academic progress.

4. Eligibility Checker

Automatically check whether a student is allowed to enroll in a particular course.

System returns:

YES → student has all prerequisites

NO → lists all missing prerequisite courses

5. Valid Course Sequences (Topological Ordering)

Generates all possible valid course-completion sequences based on prerequisites.

Prevents cycles and ensures correct prerequisite structure.

6. Additional Operations

I also implemented several helpful discrete-structure tools:

Power set generation

Combinations (k-group generation)

Lab session assignment based on capacity

Elective course assignment based on preferences

Automatic group project partitioning

Internal self-tests for verifying correctness of the module

🧠 Technical Highlights

Data structures used:
unordered_map, vector, map, recursion, backtracking, and topological sorting.

Fully modular code with clean separation between:

UniversityManager.cpp / .h (my module)

main menu system

Input-driven console interface to allow user interaction.

📌 Summary

In short, I implemented the complete Course & Prerequisite Engine of the university system, enabling course creation, prerequisite tracking, student progress management, eligibility checks, and several discrete-structure utilities that support the advanced modules of the project.
