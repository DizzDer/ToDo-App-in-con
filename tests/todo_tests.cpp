#include "Todo.h"
#include "TodoExtras.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
void check(bool ok) { if (!ok) throw std::runtime_error("Check failed"); }
template<class F> void rejects(F f) { bool failed=false; try { f(); } catch (const std::exception&) { failed=true; } check(failed); }
int main() {
    try {
        TodoApp app;
        app.addTask("first", 3); app.addTask("second", 2); app.addTask("third", 1);
        app.toggleTask(1); app.toggleTask(2); TodoExtras::clearCompleted(app);
        check(app.getTasks().size() == 1 && app.getTasks()[0].id == 3);
        rejects([&]{app.addTask(" ", 1);}); rejects([&]{app.addTask("x", 4);}); rejects([&]{app.toggleTask(100);});
        app.addTask("pipes | quotes \" and \\ slash", 2);
        const auto path = "todo-test.db";
        app.saveToFile(path); app.saveToFile(path);
        TodoApp loaded; loaded.loadFromFile(path);
        check(loaded.getTasks().size()==2 && loaded.getTasks()[1].title==app.getTasks()[1].title);
        loaded.removeTask(4); loaded.saveToFile(path); app.loadFromFile(path); app.addTask("new", 1);
        check(app.getTasks().back().id==5);
        { std::ofstream out(path); out << "1|old|0|3\n2|done|1|2\n"; }
        loaded.loadFromFile(path); check(loaded.getTasks().size()==2); check(loaded.getTasks()[1].status==Status::Done);
        for (const auto* invalid : {"1|x|8|1\n", "1|x|0|1\n1|dup|0|1\n", "broken\n", "1|x|0|1\ntruncated"}) {
            { std::ofstream out(path); out << invalid; }
            rejects([&]{loaded.loadFromFile(path);}); check(loaded.getTasks().size()==2);
        }
        rejects([&]{loaded.saveToFile("missing-parent/todo.db");});
        std::filesystem::remove(path);
        std::cout << "Persistence, validation, migration, deletion and stable IDs passed\n";
    } catch (const std::exception& e) { std::cerr << e.what(); return 1; }
}
