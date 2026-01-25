#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> 
using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

class Task {
private:
    string title;
    string dueDate;
    int priority; 
    bool completed;

public:
    Task(string t, string d, int p) : title(t), dueDate(d), priority(p), completed(false) {}

    string getTitle() const { return title; }
    string getDueDate() const { return dueDate; }
    int getPriority() const { return priority; }
    bool isCompleted() const { return completed; }

    void markComplete() { completed = true; }

    string serialize() const {
        return title + "|" + dueDate + "|" + to_string(priority) + "|" + (completed ? "1" : "0");
    }

    static Task deserialize(const string &line) {
        stringstream ss(line);
        string t, d, p_str, c_str;
        getline(ss, t, '|');
        getline(ss, d, '|');
        getline(ss, p_str, '|');
        getline(ss, c_str, '|');
        Task task(t, d, stoi(p_str));
        if(c_str == "1") task.markComplete();
        return task;
    }
};

class TaskManager {
private:
    vector<Task> tasks;
    string filename;

public:
    TaskManager(const string &file) : filename(file) { load(); }

    void addTask(const string &title, const string &dueDate, int priority) {
        tasks.push_back(Task(title, dueDate, priority));
        save();
    }

    void completeTask(int index) {
        if(index < 1 || index > tasks.size()) {
            cout << RED << "Invalid task number!" << RESET << endl;
            return;
        }
        tasks[index-1].markComplete();
        save();
    }

    void deleteTask(int index) {
        if(index < 1 || index > tasks.size()) {
            cout << RED << "Invalid task number!" << RESET << endl;
            return;
        }
        tasks.erase(tasks.begin() + index - 1);
        save();
    }

    void listTasks(bool showAll = true) {
        cout << CYAN << "\n=== Task List ===" << RESET << endl;
        if(tasks.empty()) {
            cout << "No tasks yet!" << endl;
            return;
        }
        for(size_t i = 0; i < tasks.size(); i++) {
            if(!showAll && tasks[i].isCompleted()) continue;

            string color = tasks[i].isCompleted() ? GREEN : (tasks[i].getPriority() == 1 ? RED : tasks[i].getPriority() == 2 ? YELLOW : RESET);
            cout << i+1 << ". [" << (tasks[i].isCompleted() ? "x" : " ") << "] "
                 << color << tasks[i].getTitle() << RESET
                 << " (Due: " << tasks[i].getDueDate() << ", Priority: " << tasks[i].getPriority() << ")" << endl;
        }
        cout << CYAN << "================\n" << RESET << endl;
    }

    void save() {
        ofstream file(filename);
        for(auto &t : tasks) file << t.serialize() << endl;
    }

    void load() {
        tasks.clear();
        ifstream file(filename);
        string line;
        while(getline(file, line)) {
            tasks.push_back(Task::deserialize(line));
        }
    }
};

int main() {
    TaskManager manager("tasks.txt");
    int choice;

    do {
        cout << CYAN
             << "1. List all tasks\n"
             << "2. List pending tasks\n"
             << "3. Add task\n"
             << "4. Complete task\n"
             << "5. Delete task\n"
             << "0. Exit\n"
             << "Choose: " << RESET;
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                manager.listTasks();
                break;
            case 2:
                manager.listTasks(false);
                break;
            case 3: {
                string title, dueDate;
                int priority;
                cout << "Enter task title: ";
                getline(cin, title);
                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, dueDate);
                cout << "Enter priority (1=High, 2=Medium, 3=Low): ";
                cin >> priority;
                cin.ignore();
                manager.addTask(title, dueDate, priority);
                break;
            }
            case 4: {
                int num;
                cout << "Enter task number to complete: ";
                cin >> num;
                cin.ignore();
                manager.completeTask(num);
                break;
            }
            case 5: {
                int num;
                cout << "Enter task number to delete: ";
                cin >> num;
                cin.ignore();
                manager.deleteTask(num);
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << RED << "Invalid choice!" << RESET << endl;
        }
    } while(choice != 0);

    return 0;
}
 
