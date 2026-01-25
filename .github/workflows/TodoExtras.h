#ifndef TODO_EXTRAS_H
#define TODO_EXTRAS_H

#include "Todo.h"

using namespace std;

class TodoExtras {
public:
    static void showStats(const TodoApp& app);
    static void searchByTitle(const TodoApp& app, const string& keyword);
};

#endif
