#include <iostream>
#include "Todo.h"
#include "TodoExtras.h"

using namespace std;

int main() {
    TodoApp app;
    app.loadFromFile("todo.db");

    int choice{};
    while (true) {
        cout << "\n1.Add 2.Remove 3.Toggle 4.Show 5.Stats "
                "6.Search 7.Priority 8.Sort 9.ClearDone 0.Exit\n> ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        if (choice == 1) {
            string title;
            int p;
            cout << "Title: ";
            getline(cin, title);
            cout << "Priority (1-3): ";
            cin >> p;
            cin.ignore();
            app.addTask(title, p);
        }
        else if (choice == 7) {
            int id, p;
            cout << "ID: ";
            cin >> id;
            cout << "New priority: ";
            cin >> p;
            cin.ignore();
            app.changePriority(id, p);
        }
        else if (choice == 8) {
            app.showSortedByPriority();
        }
        else if (choice == 9) {
            TodoExtras::clearCompleted(app);
        }
        else if (choice == 2) {
            int id;
            cin >> id;
            app.removeTask(id);
        }
        else if (choice == 3) {
            int id;
            cin >> id;
            app.toggleTask(id);
        }
        else if (choice == 4) {
            app.showTasks();
        }
        else if (choice == 5) {
            TodoExtras::showStats(app);
        }
        else if (choice == 6) {
            string key;
            cout << "Keyword: ";
            getline(cin, key);
            TodoExtras::searchByTitle(app, key);
        }
    }

    app.saveToFile("todo.db");
    return 0;
}
