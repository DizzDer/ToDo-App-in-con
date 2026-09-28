#include "Todo.h"
#include "TodoExtras.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
namespace {
struct EndInput {};
std::string line(const char* prompt) {
    std::cout << prompt;
    std::string value;
    if (!std::getline(std::cin, value)) throw EndInput{};
    return value;
}
int number(const char* prompt) {
    std::istringstream in(line(prompt)); int n;
    if (!(in >> n) || !(in >> std::ws).eof()) throw std::invalid_argument("Enter a whole number");
    return n;
}
}
int main(int argc, char** argv) {
    if (argc > 2) { std::cerr << "Usage: todo [database-path]\n"; return 2; }
    const std::string path = argc == 2 ? argv[1] : "todo.db";
    TodoApp app;
    try { app.loadFromFile(path); }
    catch (const std::exception& e) { std::cerr << "Load failed: " << e.what() << "\nDatabase was not modified.\n"; return 1; }
    for (;;) {
        auto before = app;
        try {
            int choice = number("\n1.Add 2.Remove 3.Toggle 4.Show 5.Stats 6.Search 7.Priority 8.Sort 9.ClearDone 0.Exit\n> ");
            if (choice == 0) break;
            bool changed = true;
            switch (choice) {
            case 1: { auto title = line("Title: "); auto priority = number("Priority (1=low, 3=high): "); app.addTask(title, priority); break; }
            case 2: app.removeTask(number("ID: ")); break;
            case 3: app.toggleTask(number("ID: ")); break;
            case 4: app.showTasks(); changed = false; break;
            case 5: TodoExtras::showStats(app); changed = false; break;
            case 6: TodoExtras::searchByTitle(app, line("Keyword: ")); changed = false; break;
            case 7: { auto id = number("ID: "); auto priority = number("New priority: "); app.changePriority(id, priority); break; }
            case 8: app.showSortedByPriority(); changed = false; break;
            case 9: app.clearCompleted(); break;
            default: throw std::invalid_argument("Unknown command");
            }
            if (changed) app.saveToFile(path);
        } catch (const EndInput&) { break; }
        catch (const std::exception& e) { app = before; std::cerr << "Error: " << e.what() << '\n'; }
    }
}
