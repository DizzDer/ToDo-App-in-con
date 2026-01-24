
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
};

class TodoApp {
private:
    vector<Task> tasks;
    int nextId{1};

    Task* findTask(int id);

public:
    void addTask(const string& title);
    void removeTask(int id);
    void toggleTask(int id);
    void showTasks() const;

    void loadFromFile(const string& filename);
    void saveToFile(const string& filename) const;
};

#endif
