#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include "UniversityManager.h"
#include "StudentManager2.h"
#include "DiscreteManager3.h"

using namespace std;

int toInt(const string& s, int def = 0) { try { return stoi(s); } catch (...) { return def; } }

void header(const string& t) {
    cout << "\n========================\n";
    cout << "   " << t << "\n";
    cout << "========================\n";
}

void topMenu() {
    header("FAST UNIVERSITY SYSTEM");
    cout << "1. Module 1 (Courses & Discrete Ops)\n";
    cout << "2. Module 2 (Student Academic Module)\n";
    cout << "3. Module 3 (Discrete Structures - Graphs, Sets)\n";
    cout << "0. Exit\n";
    cout << "Select: ";
}

int main() {
    UniversityManager M1;
    StudentManager2 M2;
    DiscreteManager3 M3;
    string line;

    while (true) {
        topMenu();
        getline(cin, line);
        int choice = toInt(line, -1);
        if (choice == 0) break;

        // ===== MODULE 1 =====
        if (choice == 1) {
            string sub;
            while (true) {
                cout << "\n===========================\n";
                cout << "     MODULE 1 — COURSES\n";
                cout << "===========================\n";
                cout << "1. Add course\n";
                cout << "2. Add prerequisite\n";
                cout << "3. Show all courses\n";
                cout << "4. Show prerequisites\n";
                cout << "5. Add student\n";
                cout << "6. Show all students\n";
                cout << "7. Check if student can take course\n";
                cout << "8. Load sample dataset\n";
                cout << "9. Show all valid sequences\n";
                cout << "10. Combinations of students\n";
                cout << "11. Lab session assignment\n";
                cout << "12. Elective assignment\n";
                cout << "13. Auto project group generator\n";
                cout << "0. Back\n";
                cout << "Select: ";
                getline(cin, sub);
                int x = toInt(sub, -1);
                if (x == 0) break;

                if (x == 1) {
                    cout << "Enter course id: ";
                    string c; getline(cin, c);
                    M1.addCourse(c);
                }
                else if (x == 2) {
                    cout << "Enter prerequisite: ";
                    string a; getline(cin, a);
                    cout << "Enter course: ";
                    string b; getline(cin, b);
                    M1.addPrerequisite(a, b);
                }
                else if (x == 3) M1.showCourses();
                else if (x == 4) M1.showPrereqs();
                else if (x == 5) {
                    cout << "Student id: ";
                    string s; getline(cin, s);
                    M1.addStudent(s);
                }
                else if (x == 6) M1.showStudents();
                else if (x == 7) {
                    cout << "Student: ";
                    string s; getline(cin, s);
                    cout << "Course: ";
                    string c; getline(cin, c);
                    vector<string> missing;
                    bool ok = M1.canStudentTake(s, c, &missing);
                    if (ok) cout << "YES\n";
                    else {
                        cout << "NO - Missing:";
                        for (auto& m : missing) cout << " " << m;
                        cout << "\n";
                    }
                }
                else if (x == 8) M1.loadSampleData();
                else if (x == 9) {
                    cout << "Cap: ";
                    string s; getline(cin, s);
                    M1.showAllValidSequences((size_t)toInt(s, 10));
                }
                else if (x == 10) {
                    int n, k;
                    cout << "Number of students: "; cin >> n; cin.ignore();
                    vector<string> studs(n);
                    for (int i = 0; i < n; i++) {
                        cout << "Student " << i + 1 << ": "; getline(cin, studs[i]);
                    }
                    cout << "Combination size k: "; cin >> k; cin.ignore();
                    auto combs = M1.combinationsOf(studs, k);
                    for (auto& v : combs) {
                        cout << "[";
                        for (size_t i = 0; i < v.size(); i++)
                            cout << v[i] << (i + 1 < v.size() ? ", " : "");
                        cout << "]\n";
                    }
                }
                else if (x == 11) {
                    cout << "Students n: "; string s; getline(cin, s);
                    int n = toInt(s, 0);
                    vector<string> list(n);
                    for (int i = 0; i < n; i++) {
                        cout << "Student " << i + 1 << ": ";
                        getline(cin, list[i]);
                    }
                    cout << "Labs L: "; getline(cin, s);
                    int L = toInt(s, 0);
                    map<string, int> labs;
                    for (int i = 0; i < L; i++) {
                        string name, cap;
                        cout << "Lab name: "; getline(cin, name);
                        cout << "Capacity: "; getline(cin, cap);
                        labs[name] = toInt(cap, 0);
                    }
                    auto res = M1.assignLabSessions(list, labs);
                    for (auto& p : res) {
                        cout << p.first << ": ";
                        for (auto& x : p.second) cout << x << " ";
                        cout << "\n";
                    }
                }
                else if (x == 12) {
                    cout << "Students n: "; string s; getline(cin, s);
                    int n = toInt(s, 0);
                    vector<string> ids(n);
                    vector<vector<string>> prefs(n);
                    for (int i = 0; i < n; i++) {
                        cout << "ID: "; getline(cin, ids[i]);
                        cout << "Prefs count: "; getline(cin, s);
                        int p = toInt(s, 0);
                        prefs[i].resize(p);
                        for (int j = 0; j < p; j++) {
                            cout << "Pref " << j + 1 << ": ";
                            getline(cin, prefs[i][j]);
                        }
                    }
                    cout << "Electives E: "; getline(cin, s);
                    int E = toInt(s, 0);
                    unordered_map<string, int> cap;
                    for (int i = 0; i < E; i++) {
                        string e, c;
                        cout << "Elective: "; getline(cin, e);
                        cout << "Cap: "; getline(cin, c);
                        cap[e] = toInt(c, 0);
                    }
                    auto ans = M1.assignElectives(ids, prefs, cap);
                    for (auto& id : ids) {
                        cout << id << " -> " << (ans[id].empty() ? "(none)" : ans[id]) << "\n";
                    }
                }
                else if (x == 13) {
                    cout << "Students n: "; string s; getline(cin, s);
                    int n = toInt(s, 0);
                    vector<string> v(n);
                    for (int i = 0; i < n; i++) {
                        cout << "Student " << i + 1 << ": ";
                        getline(cin, v[i]);
                    }
                    cout << "Group size: "; getline(cin, s);
                    int k = toInt(s, 2);
                    auto out = M1.autoPartitionGroups(v, k);
                    for (size_t i = 0; i < out.size(); i++) {
                        cout << "Group " << i + 1 << ": ";
                        for (auto& x : out[i]) cout << x << " ";
                        cout << "\n";
                    }
                }
            }
        }

        // ===== MODULE 2 =====
        else if (choice == 2) {
            header("Module 2 - Student Academic");
            string sub;
            while (true) {
                cout << "1. Open Module 2 menu\n2. Back\nSelect: ";
                getline(cin, sub);
                int s = toInt(sub, -1);
                if (s == 2) break;
                if (s == 1) {
                    string cmd;
                    while (true) {
                        cout << "\nModule 2 Menu:\n1.Add student\n2.Add course info\n3.Set course schedule\n4.Enroll student\n5.Show student\n6.Detect conflicts\n0.Back\nChoice: ";
                        getline(cin, cmd);
                        int c = toInt(cmd, -1);
                        if (c == 0) break;
                        if (c == 1) { cout << "ID: "; string id; getline(cin, id); cout << "Name: "; string name; getline(cin, name); M2.addStudent(id, name); cout << "Added\n"; }
                        else if (c == 2) { cout << "Course ID: "; string cid; getline(cin, cid); cout << "Credits: "; string cr; getline(cin, cr); M2.addCourseInfo(cid, toInt(cr, 3)); cout << "Added\n"; }
                        else if (c == 3) { cout << "Course ID: "; string cid; getline(cin, cid); cout << "Day: "; string day; getline(cin, day); cout << "Start: "; string st; getline(cin, st); cout << "End: "; string en; getline(cin, en); M2.setCourseSchedule(cid, day, toInt(st, 9), toInt(en, 10)); cout << "Scheduled\n"; }
                        else if (c == 4) { cout << "Student ID: "; string id; getline(cin, id); cout << "Course ID: "; string cid; getline(cin, cid); if (M2.enrollCourse(id, cid)) cout << "Enrolled\n"; else cout << "Failed\n"; }
                        else if (c == 5) { cout << "Student ID: "; string id; getline(cin, id); M2.showStudent(id); }
                        else if (c == 6) { cout << "Student ID: "; string id; getline(cin, id); auto v = M2.detectScheduleConflicts(id); if (v.empty()) cout << "No conflicts\n"; else for (auto& p : v) cout << p.first << " <-> " << p.second << "\n"; }
                    }
                }
            }
        }

        // ===== MODULE 3 =====
        else if (choice == 3) {
            header("Module 3 - Discrete Structures");
            string sub;
            while (true) {
                cout << "1. Open Module 3 menu\n2. Back\nSelect: ";
                getline(cin, sub);
                int s = toInt(sub, -1);
                if (s == 2) break;
                if (s == 1) {
                    string cmd;
                    while (true) {
                        cout << "\nModule 3 Menu:\n1.Add node\n2.Add edge\n3.BFS\n4.DFS\n5.Shortest path\n6.Check bipartite\n7.Store set\n8.Set ops\n9.Relation pairs\n10.Relation properties\n11.nPr/nCr\n12.Recurrences\n0.Back\nChoice: ";
                        getline(cin, cmd);
                        int c = toInt(cmd, -1);
                        if (c == 0) break;
                    }
                }
            }
        }
    }

    return 0;
}
