#ifndef TODO_H
#define TODO_H

#include <string>
#include <vector>



enum class Status {
    Pending,
    Done
};

struct Task {
    int id{};
    std::string title;
    Status status{Status::Pending};
    int priority{1}; 
};

class TodoApp {
private:
    std::vector<Task> tasks;
    int nextId{1};

    Task* findTask(int id);

public:
    void addTask(const std::string& title, int priority = 1);
    void removeTask(int id);
    void toggleTask(int id);
    void changePriority(int id, int priority);
    void clearCompleted();

    void showTasks() const;
    void showSortedByPriority() const;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    const std::vector<Task>& getTasks() const;
};

#endif
