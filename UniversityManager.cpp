#include "UniversityManager.h"
#include <algorithm>
#include <iostream>

using namespace std;

UniversityManager::UniversityManager() {}

void UniversityManager::invalidatePrereqMemo() {
    prereqMemo.clear();
}

bool UniversityManager::addCourse(const string& courseId) {
    if (courseId.empty()) return false;
    courses.insert(courseId);
    adj[courseId];
    revAdj[courseId];
    invalidatePrereqMemo();
    return true;
}

bool UniversityManager::addPrerequisite(const string& prereq, const string& course) {
    if (prereq.empty() || course.empty()) return false;
    addCourse(prereq);
    addCourse(course);
    auto& v = adj[prereq];
    if (find(v.begin(), v.end(), course) != v.end()) return false;
    v.push_back(course);
    revAdj[course].push_back(prereq);
    invalidatePrereqMemo();
    return true;
}

vector<string> UniversityManager::getAllCourses() const {
    vector<string> out(courses.begin(), courses.end());
    sort(out.begin(), out.end());
    return out;
}

bool UniversityManager::hasCycleUtil(const string& u, unordered_map<string, int>& color) {
    color[u] = 1;
    auto it = adj.find(u);
    if (it != adj.end()) {
        for (const string& v : it->second) {
            if (color[v] == 1) return true;
            if (color[v] == 0 && hasCycleUtil(v, color)) return true;
        }
    }
    color[u] = 2;
    return false;
}

bool UniversityManager::hasCycle() {
    unordered_map<string, int> color;
    for (const auto& c : courses) color[c] = 0;
    for (const auto& c : courses)
        if (color[c] == 0)
            if (hasCycleUtil(c, color)) return true;
    return false;
}

void UniversityManager::allTopologicalSorts(vector<vector<string>>& results, size_t cap) {
    unordered_map<string, int> indeg;
    for (const auto& c : courses) indeg[c] = 0;
    for (const auto& p : adj)
        for (const auto& to : p.second)
            indeg[to]++;
    vector<string> curr;
    unordered_set<string> used;
    auto dfs = [&](auto&& self) -> void {
        if (results.size() >= cap) return;
        vector<string> zeros;
        for (const auto& x : indeg)
            if (x.second == 0 && used.find(x.first) == used.end())
                zeros.push_back(x.first);
        sort(zeros.begin(), zeros.end());
        bool progressed = false;
        for (const string& z : zeros) {
            progressed = true;
            used.insert(z);
            curr.push_back(z);
            vector<string> changed;
            auto it = adj.find(z);
            if (it != adj.end()) {
                for (const auto& nbr : it->second) {
                    indeg[nbr]--;
                    changed.push_back(nbr);
                }
            }
            int old = indeg[z];
            indeg[z] = -1;
            self(self);
            indeg[z] = old;
            for (const auto& m : changed) indeg[m]++;
            curr.pop_back();
            used.erase(z);
            if (results.size() >= cap) return;
        }
        if (!progressed) {
            if (curr.size() == courses.size()) results.push_back(curr);
        }
        };
    dfs(dfs);
}

void UniversityManager::showAllValidSequences(size_t cap) {
    if (hasCycle()) {
        cout << "ERROR: prerequisite graph has a cycle.\n";
        return;
    }
    vector<vector<string>> results;
    allTopologicalSorts(results, cap);
    if (results.empty()) {
        cout << "No valid sequences found.\n";
        return;
    }
    size_t show = min(results.size(), cap);
    cout << "Showing up to " << show << " valid sequences:\n";
    for (size_t i = 0;i < show;i++) {
        cout << i + 1 << ". ";
        for (size_t j = 0;j < results[i].size();j++) {
            cout << results[i][j];
            if (j + 1 < results[i].size()) cout << " -> ";
        }
        cout << "\n";
    }
    if (results.size() > cap) cout << "... and more (cap reached)\n";
}

bool UniversityManager::addStudent(const string& sid) {
    if (sid.empty()) return false;
    if (!students.count(sid)) students[sid] = Student(sid);
    return true;
}

bool UniversityManager::markCompleted(const string& sid, const string& course) {
    if (sid.empty() || course.empty()) return false;
    addStudent(sid);
    addCourse(course);
    students[sid].completedCourses.insert(course);
    return true;
}

bool UniversityManager::unmarkCompleted(const string& sid, const string& course) {
    if (!students.count(sid)) return false;
    students[sid].completedCourses.erase(course);
    return true;
}

void UniversityManager::collectAllPrereqsUtil(const string& course, unordered_set<string>& accum, unordered_set<string>& vis) {
    if (prereqMemo.find(course) != prereqMemo.end()) {
        for (const auto& x : prereqMemo[course]) accum.insert(x);
        return;
    }
    if (vis.count(course)) return;
    vis.insert(course);
    auto it = revAdj.find(course);
    unordered_set<string> local;
    if (it != revAdj.end()) {
        for (const string& p : it->second) {
            local.insert(p);
            collectAllPrereqsUtil(p, local, vis);
        }
    }
    prereqMemo[course] = local;
    for (const auto& x : local) accum.insert(x);
    vis.erase(course);
}

unordered_set<string> UniversityManager::collectAllPrereqs(const string& course) {
    unordered_set<string> result;
    unordered_set<string> vis;
    collectAllPrereqsUtil(course, result, vis);
    return result;
}

bool UniversityManager::canStudentTake(const string& sid, const string& course, vector<string>* missingOut) {
    if (!students.count(sid) || !courses.count(course)) return false;
    unordered_set<string> required = collectAllPrereqs(course);
    vector<string> missing;
    for (const string& r : required) {
        if (students[sid].completedCourses.find(r) == students[sid].completedCourses.end())
            missing.push_back(r);
    }
    sort(missing.begin(), missing.end());
    if (missingOut) *missingOut = missing;
    return missing.empty();
}

vector<vector<string>> UniversityManager::combinationsOf(const vector<string>& items, int k) {
    vector<vector<string>> out;
    int n = (int)items.size();
    if (k < 0 || k > n) return out;
    vector<int> bit(n, 0);
    for (int i = 0;i < k;i++) bit[n - 1 - i] = 1;
    do {
        vector<string> g;
        for (int i = 0;i < n;i++) if (bit[i]) g.push_back(items[i]);
        out.push_back(g);
    } while (next_permutation(bit.begin(), bit.end()));
    return out;
}

vector<vector<string>> UniversityManager::powerSet(const vector<string>& items, size_t cap) {
    vector<vector<string>> out;
    int n = (int)items.size();
    if (n >= 31) {
        unsigned long long limit = cap;
        for (unsigned long long m = 0;m < limit;m++) {
            vector<string> sub;
            out.push_back(sub);
        }
        return out;
    }
    unsigned long long total = (n == 0) ? 1ULL : (1ULL << n);
    unsigned long long limit = min(total, (unsigned long long)cap);
    for (unsigned long long m = 0;m < limit;m++) {
        vector<string> sub;
        for (int i = 0;i < n;i++) if (m & (1ULL << i)) sub.push_back(items[i]);
        out.push_back(sub);
    }
    return out;
}

void UniversityManager::showCourses() {
    auto v = getAllCourses();
    if (v.empty()) { cout << "No courses.\n"; return; }
    cout << "Courses:\n";
    for (const auto& c : v) cout << " - " << c << "\n";
}

void UniversityManager::showPrereqs() {
    if (courses.empty()) { cout << "No prerequisites.\n"; return; }
    map<string, vector<string>> ordered;
    for (const auto& c : courses) {
        auto it = adj.find(c);
        if (it != adj.end() && !it->second.empty()) ordered[c] = it->second;
    }
    cout << "Prerequisite relations:\n";
    for (const auto& p : ordered)
        for (const auto& to : p.second)
            cout << " " << p.first << " -> " << to << "\n";
}

void UniversityManager::showStudents() {
    if (students.empty()) { cout << "No students.\n"; return; }
    cout << "Students:\n";
    for (const auto& p : students) {
        cout << " - " << p.first << " : ";
        vector<string> cc(p.second.completedCourses.begin(), p.second.completedCourses.end());
        sort(cc.begin(), cc.end());
        for (size_t i = 0;i < cc.size();i++) cout << cc[i] << (i + 1 < cc.size() ? ", " : "");
        cout << "\n";
    }
}

void UniversityManager::loadSampleData() {
    adj.clear();
    revAdj.clear();
    courses.clear();
    students.clear();
    invalidatePrereqMemo();
    addCourse("IntroCS"); addCourse("BasicCS"); addCourse("DataStruct"); addCourse("Algorithms"); addCourse("Databases"); addCourse("AdvancedCS");
    addPrerequisite("IntroCS", "BasicCS");
    addPrerequisite("BasicCS", "DataStruct");
    addPrerequisite("DataStruct", "Algorithms");
    addPrerequisite("Algorithms", "AdvancedCS");
    addPrerequisite("Databases", "AdvancedCS");
    addStudent("S1"); markCompleted("S1", "IntroCS"); markCompleted("S1", "BasicCS");
    addStudent("S2"); markCompleted("S2", "IntroCS"); markCompleted("S2", "BasicCS"); markCompleted("S2", "DataStruct"); markCompleted("S2", "Algorithms");
    addStudent("S3"); markCompleted("S3", "IntroCS");
    cout << "Sample data loaded.\n";
}

void UniversityManager::runSelfTests() {
    loadSampleData();
    cout << "Running self-tests...\n";
    vector<string> missing;
    bool ok = canStudentTake("S1", "AdvancedCS", &missing);
    cout << "S1 -> AdvancedCS : " << (ok ? "CAN" : "CANNOT") << "\n";
    if (!ok) { cout << "Missing:"; for (auto& m : missing) cout << " " << m; cout << "\n"; }
    ok = canStudentTake("S2", "AdvancedCS", &missing);
    cout << "S2 -> AdvancedCS : " << (ok ? "CAN" : "CANNOT") << "\n";
    cout << "Valid sequences (cap 10):\n";
    showAllValidSequences(10);
    vector<string> studs = { "S1","S2","S3" };
    auto groups = combinationsOf(studs, 2);
    cout << "Groups of 2:\n";
    for (auto& g : groups) {
        cout << "[";
        for (size_t i = 0;i < g.size();i++) cout << g[i] << (i + 1 < g.size() ? ", " : "");
        cout << "]\n";
    }
}

unordered_map<string, vector<string>> UniversityManager::assignLabSessions(const vector<string>& studentsList, const map<string, int>& labCapacities) {
    unordered_map<string, vector<string>> assignment;
    vector<pair<string, int>> labs;
    labs.reserve(labCapacities.size());
    for (const auto& p : labCapacities) labs.emplace_back(p.first, p.second);
    if (labs.empty()) return assignment;
    int n = (int)studentsList.size();
    int idx = 0;
    vector<int> remaining;
    remaining.reserve(labs.size());
    for (auto& l : labs) remaining.push_back(l.second);
    for (int i = 0;i < n;i++) {
        bool placed = false;
        int attempts = 0;
        while (attempts < (int)labs.size()) {
            int j = (idx + attempts) % labs.size();
            if (remaining[j] > 0) {
                assignment[labs[j].first].push_back(studentsList[i]);
                remaining[j]--;
                idx = (j + 1) % labs.size();
                placed = true;
                break;
            }
            attempts++;
        }
        if (!placed) break;
    }
    return assignment;
}

unordered_map<string, string> UniversityManager::assignElectives(const vector<string>& studentsList, const vector<vector<string>>& preferences, unordered_map<string, int> capacities) {
    unordered_map<string, string> result;
    int n = (int)studentsList.size();
    vector<int> assigned(n, 0);
    int maxPref = 0;
    for (const auto& pref : preferences) if ((int)pref.size() > maxPref) maxPref = (int)pref.size();
    for (int round = 0; round < maxPref; ++round) {
        for (int i = 0;i < n;i++) {
            if (assigned[i]) continue;
            if (i >= (int)preferences.size()) continue;
            const auto& prefs = preferences[i];
            if (round >= (int)prefs.size()) continue;
            const string& choice = prefs[round];
            auto it = capacities.find(choice);
            if (it != capacities.end() && it->second > 0) {
                result[studentsList[i]] = choice;
                it->second--;
                assigned[i] = 1;
            }
        }
    }
    for (int i = 0;i < n;i++) {
        if (!assigned[i]) result[studentsList[i]] = string();
    }
    return result;
}

vector<vector<string>> UniversityManager::autoPartitionGroups(const vector<string>& studentsList, int groupSize) {
    vector<vector<string>> out;
    if (groupSize <= 0) return out;
    vector<string> copy = studentsList;
    unsigned long long seed = 88172645463325252ULL;
    int n = (int)copy.size();
    for (int i = n - 1;i > 0;i--) {
        seed = seed * 1103515245ULL + 12345ULL;
        unsigned long long r = seed % (unsigned long long)(i + 1);
        int j = (int)r;
        string tmp = copy[i];
        copy[i] = copy[j];
        copy[j] = tmp;
    }
    for (int i = 0;i < n;i += groupSize) {
        vector<string> grp;
        for (int j = i;j < min(n, i + groupSize); ++j) grp.push_back(copy[j]);
        out.push_back(grp);
    }
    return out;
}
