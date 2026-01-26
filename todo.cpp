#include "Todo.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

Task* TodoApp::findTask(int id) {
    auto it = find_if(tasks.begin(), tasks.end(),
        [id](const Task& t) { return t.id == id; });
    return (it != tasks.end()) ? &(*it) : nullptr;
}

void TodoApp::addTask(const string& title, int priority) {
    tasks.push_back({nextId++, title, Status::Pending, priority});
}

void TodoApp::removeTask(int id) {
    tasks.erase(
        remove_if(tasks.begin(), tasks.end(),
            [id](const Task& t) { return t.id == id; }),
        tasks.end()
    );
}

void TodoApp::toggleTask(int id) {
    if (auto* task = findTask(id)) {
        task->status =
            (task->status == Status::Pending) ? Status::Done : Status::Pending;
    }
}

void TodoApp::changePriority(int id, int priority) {
    if (auto* task = findTask(id)) {
        task->priority = priority;
    }
}

void TodoApp::showTasks() const {
    for (const auto& t : tasks) {
        cout << "[" << (t.status == Status::Done ? "x" : " ") << "] "
             << t.id << ". " << t.title
             << " (P:" << t.priority << ")\n";
    }
}

void TodoApp::showSortedByPriority() const {
    vector<Task> copy = tasks;
    sort(copy.begin(), copy.end(),
         [](const Task& a, const Task& b) {
             return a.priority > b.priority;
         });

    for (const auto& t : copy) {
        cout << "[" << (t.status == Status::Done ? "x" : " ") << "] "
             << t.id << ". " << t.title
             << " (P:" << t.priority << ")\n";
    }
}

void TodoApp::saveToFile(const string& filename) const {
    ofstream out(filename);
    for (const auto& t : tasks) {
        out << t.id << "|" << t.title << "|"
            << static_cast<int>(t.status) << "|"
            << t.priority << '\n';
    }
}

void TodoApp::loadFromFile(const string& filename) {
    ifstream in(filename);
    if (!in) return;

    tasks.clear();
    string title;
    int id, status, priority;
    char sep;

    while (in >> id >> sep && sep == '|') {
        getline(in, title, '|');
        in >> status >> sep >> priority;
        in.ignore();

        tasks.push_back(
            {id, title, static_cast<Status>(status), priority});
        nextId = max(nextId, id + 1);
    }
}

const vector<Task>& TodoApp::getTasks() const {
    return tasks;
}

    return 0;
}
