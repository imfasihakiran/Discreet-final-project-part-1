#ifndef UNIVERSITYMANAGER_H
#define UNIVERSITYMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

using namespace std;

class Student {
public:
    string id;
    unordered_set<string> completedCourses;
    Student() {}
    Student(const string& s) :id(s) {}
};

class UniversityManager {
private:
    unordered_map<string, vector<string>> adj;
    unordered_map<string, vector<string>> revAdj;
    unordered_set<string> courses;
    unordered_map<string, Student> students;
    unordered_map<string, unordered_set<string>> prereqMemo;
    bool hasCycleUtil(const string& u, unordered_map<string, int>& color);
    void collectAllPrereqsUtil(const string& course, unordered_set<string>& accum, unordered_set<string>& vis);

public:
    UniversityManager();
    bool addCourse(const string& courseId);
    bool addPrerequisite(const string& prereq, const string& course);
    vector<string> getAllCourses() const;
    bool hasCycle();
    void allTopologicalSorts(vector<vector<string>>& results, size_t cap);
    void showAllValidSequences(size_t cap);
    bool addStudent(const string& sid);
    bool markCompleted(const string& sid, const string& course);
    bool unmarkCompleted(const string& sid, const string& course);
    bool canStudentTake(const string& sid, const string& course, vector<string>* missingOut = nullptr);
    vector<vector<string>> combinationsOf(const vector<string>& items, int k);
    vector<vector<string>> powerSet(const vector<string>& items, size_t cap);
    void showCourses();
    void showPrereqs();
    void showStudents();
    void loadSampleData();
    void runSelfTests();
    unordered_map<string, vector<string>> assignLabSessions(const vector<string>& studentsList, const map<string, int>& labCapacities);
    unordered_map<string, string> assignElectives(const vector<string>& studentsList, const vector<vector<string>>& preferences, unordered_map<string, int> capacities);
    vector<vector<string>> autoPartitionGroups(const vector<string>& studentsList, int groupSize);
    void invalidatePrereqMemo();
    unordered_set<string> collectAllPrereqs(const string& course);
};

#endif
