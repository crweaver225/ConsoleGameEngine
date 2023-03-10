#ifndef ConsoleGameEngine_H
#define ConsoleGameEngine_H

#include <curses.h>
#include <chrono>
#include <thread>

class ConsoleGameEngine {
    protected:
        int m_nScreenWidth;
        int m_nScreenHeight;
    public:
        void ConstructConsole(int height, int width);
};

#endif