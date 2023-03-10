#include "ConsoleGameEngine.h"

void ConsoleGameEngine::ConstructConsole(int height, int width) {
    
    // start ncurses
    initscr(); 
    // do not print input values
    noecho();
    // terminate ncurses on ctrl + c
    cbreak();
    //enable color
    start_color();

    int x_max{getmaxx(stdscr)};
    WINDOW* system_window = newwin(9, x_max - 1, 0, 0);

    int row{0};

    while (1) {
        wrefresh(system_window);
        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        mvwprintw(system_window, ++row, 9, "PID");

    }
    endwin();
}