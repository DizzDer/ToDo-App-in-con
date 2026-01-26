#ifndef TODO_H
#define TODO_H

#include <string>
#include <vector>

using namespace std;

enum class Status {
    Pending,
    Done
};

struct Task {
    int id{};
    string title;
    Status status{Status::Pending};
    int priority{1}; 
};

class TodoApp {
private:
    vector<Task> tasks;
    int nextId{1};

    Task* findTask(int id);

public:
    void addTask(const string& title, int priority = 1);
    void removeTask(int id);
    void toggleTask(int id);
    void changePriority(int id, int priority);

    void showTasks() const;
    void showSortedByPriority() const;

    void loadFromFile(const string& filename);
    void saveToFile(const string& filename) const;

    const vector<Task>& getTasks() const;
};

#endif
