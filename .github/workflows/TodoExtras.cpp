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
            cout << "[" << (t.status == Status::Done ? "x" : " ") << "] "
                 << t.id << ". " << t.title << '\n';
        }
    }
}
