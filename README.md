# ToDo Task Manager

A C++17 console task manager with stable IDs, priorities, completion status, search, statistics and persistent storage. No third-party libraries.

## Build and run

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Run `./build/todo` on Unix or `build\Release\todo.exe` with Visual Studio. MinGW creates `build\todo.exe`. An optional first argument selects the database path.

The menu supports add, remove, toggle, list, statistics, substring search, change priority, stable priority sorting and clearing completed tasks. Priority 3 is highest, 1 lowest. Invalid input reports an error; EOF exits cleanly.

## Storage

Each successful edit is saved using a temporary file and replacement; failed saves roll back the in-memory edit. The versioned TODO2 format escapes quotes, backslashes and separators. Original `id|title|status|priority` databases are read and migrated on the next save. Invalid files are rejected without overwriting them. IDs are not reused after deletion or restart.

Run only one process per database. Replacement protects against partial application writes but does not guarantee survival of power failure on every filesystem. Keep backups of important tasks.

`legacy/updated_Todoapp.cpp` preserves the earlier standalone date-based experiment and is deliberately excluded from the supported build. Its `tasks.txt` format differs from the modular application's database.

## Verification

Tests cover consecutive completed-task deletion, round trips with special characters, legacy migration, malformed and duplicate records, invalid priorities, unknown IDs, save errors and persistence of the ID counter. CI builds on Linux, Windows and macOS.
