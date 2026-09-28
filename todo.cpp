#include "Todo.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
namespace {
void validate(const std::string& title, int priority) {
    if (title.find_first_not_of(" \t\r\n") == std::string::npos || title.size() > 4096 || title.find_first_of("\r\n") != std::string::npos)
        throw std::invalid_argument("Title must be a nonempty single line (up to 4096 bytes)");
    if (priority < 1 || priority > 3) throw std::invalid_argument("Priority must be 1, 2 or 3");
}
void display(const Task& t) {
    std::cout << '[' << (t.status == Status::Done ? 'x' : ' ') << "] " << t.id << ". " << t.title << " (P:" << t.priority << ")\n";
}
}
Task* TodoApp::findTask(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) { return t.id == id; });
    if (it == tasks.end()) throw std::out_of_range("Task ID not found");
    return &*it;
}
void TodoApp::addTask(const std::string& title, int priority) {
    validate(title, priority);
    if (nextId == std::numeric_limits<int>::max()) throw std::overflow_error("Task ID limit reached");
    tasks.push_back({nextId, title, Status::Pending, priority});
    ++nextId;
}
void TodoApp::removeTask(int id) {
    findTask(id);
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [id](const Task& t){ return t.id == id; }), tasks.end());
}
void TodoApp::toggleTask(int id) {
    auto* task = findTask(id);
    task->status = task->status == Status::Done ? Status::Pending : Status::Done;
}
void TodoApp::changePriority(int id, int priority) {
    auto* task = findTask(id); validate(task->title, priority); task->priority = priority;
}
void TodoApp::clearCompleted() {
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](const Task& t){ return t.status == Status::Done; }), tasks.end());
}
void TodoApp::showTasks() const { for (const auto& t : tasks) display(t); }
void TodoApp::showSortedByPriority() const {
    auto copy = tasks;
    std::stable_sort(copy.begin(), copy.end(), [](const Task& a, const Task& b){ return a.priority > b.priority; });
    for (const auto& t : copy) display(t);
}
void TodoApp::saveToFile(const std::string& filename) const {
    const auto tmp = filename + ".tmp";
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out) throw std::runtime_error("Cannot open temporary database");
    out << "TODO2 " << nextId << '\n';
    for (const auto& t : tasks) out << t.id << ' ' << static_cast<int>(t.status) << ' ' << t.priority << ' ' << std::quoted(t.title) << '\n';
    out.flush();
    if (!out) throw std::runtime_error("Cannot write database");
    out.close();
    if (!out) throw std::runtime_error("Cannot close database");
#ifdef _WIN32
    const auto src = std::filesystem::path(tmp).wstring(), dst = std::filesystem::path(filename).wstring();
    if (!MoveFileExW(src.c_str(), dst.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        throw std::runtime_error("Cannot replace database; original preserved");
#else
    std::filesystem::rename(tmp, filename);
#endif
}
void TodoApp::loadFromFile(const std::string& filename) {
    if (!std::filesystem::exists(filename)) return;
    std::ifstream in(filename, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot read database");
    std::vector<Task> loaded;
    std::set<int> ids;
    std::string line;
    int loadedNext = 1;
    bool modern = false, first = true;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (first && line.rfind("TODO2 ", 0) == 0) {
            std::istringstream header(line.substr(6));
            if (!(header >> loadedNext) || loadedNext < 1 || !(header >> std::ws).eof()) throw std::runtime_error("Invalid database header");
            modern = true; first = false; continue;
        }
        first = false;
        if (line.empty()) throw std::runtime_error("Invalid empty database record");
        Task t; int status = -1;
        std::istringstream row(line);
        if (modern) {
            if (!(row >> t.id >> status >> t.priority >> std::quoted(t.title)) || !(row >> std::ws).eof()) throw std::runtime_error("Invalid database record");
        } else {
            char a, b;
            if (!(row >> t.id >> a) || a != '|' || !std::getline(row, t.title, '|') || !(row >> status >> b >> t.priority) || b != '|' || !(row >> std::ws).eof()) throw std::runtime_error("Invalid legacy database record");
        }
        validate(t.title, t.priority);
        if (t.id < 1 || t.id == std::numeric_limits<int>::max() || (status != 0 && status != 1) || !ids.insert(t.id).second) throw std::runtime_error("Invalid or duplicate task ID/status");
        t.status = static_cast<Status>(status);
        loadedNext = std::max(loadedNext, t.id + 1); loaded.push_back(t);
    }
    if (in.bad()) throw std::runtime_error("Database read failed");
    tasks.swap(loaded); nextId = loadedNext;
}
const std::vector<Task>& TodoApp::getTasks() const { return tasks; }
