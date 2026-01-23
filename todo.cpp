#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Task {
private:
    string title;
    bool completed;

public:
    Task(string t) {
        title = t;
        completed = false;
    }

    string getTitle() const { return title; }
    bool isCompleted() const { return completed; }

    void markComplete() { completed = true; }

    string serialize() const {
        return title + "|" + (completed ? "1" : "0");
    }

    static Task deserialize(const string &line) {
        size_t sep = line.find('|');
        Task t(line.substr(0, sep));
        if(line.substr(sep+1) == "1") t.markComplete();
        return t;
    }
};

class TaskManager {
private:
    vector<Task> tasks;
    string filename;

public:
    TaskManager(const string &file) : filename(file) {
        load();
    }

    void addTask(const string &title) {
        tasks.push_back(Task(title));
        save();
    }

    void completeTask(int index) {
        if(index < 1 || index > tasks.size()) {
            cout << "Invalid task number!" << endl;
            return;
        }
        tasks[index-1].markComplete();
        save();
    }

    void listTasks() {
        cout << "\n=== Task List ===" << endl;
        if(tasks.empty()) cout << "No tasks yet!" << endl;
        for(size_t i=0; i<tasks.size(); i++) {
            cout << i+1 << ". [" << (tasks[i].isCompleted() ? "x" : " ") << "] " << tasks[i].getTitle() << endl;
        }
        cout << "================\n" << endl;
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
    string title;

    do {
        cout << "1. List tasks\n2. Add task\n3. Complete task\n0. Exit\nChoose: ";
        cin >> choice;
        cin.ignore(); // clear newline

        switch(choice) {
            case 1:
                manager.listTasks();
                break;
            case 2:
                cout << "Enter task title: ";
                getline(cin, title);
                manager.addTask(title);
                break;
            case 3:
                cout << "Enter task number to complete: ";
                int num;
                cin >> num;
                manager.completeTask(num);
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while(choice != 0);

    return 0;
}
