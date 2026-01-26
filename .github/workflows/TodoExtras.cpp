#include "TodoExtras.h"
#include <iostream>

using namespace std;

void TodoExtras::showStats(const TodoApp& app) {
    int done = 0, pending = 0;

    for (const auto& t : app.getTasks()) {
        (t.status == Status::Done) ? done++ : pending++;
    }

    cout << "Total: " << done + pending
         << " | Done: " << done
         << " | Pending: " << pending << '\n';
}

void TodoExtras::searchByTitle(const TodoApp& app, const string& keyword) {
    for (const auto& t : app.getTasks()) {
        if (t.title.find(keyword) != string::npos) {
            cout << t.id << ". " << t.title << '\n';
        }
    }
}

void TodoExtras::clearCompleted(TodoApp& app) {
    for (const auto& t : app.getTasks()) {
        if (t.status == Status::Done) {
            app.removeTask(t.id);
        }
    }
}
