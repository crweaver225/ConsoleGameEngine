#include "ConsoleGameEngine.h"

void ConsoleGameEngine::ConstructConsole(int height, int width) {

    m_nScreenHeight = height;
    m_nScreenWidth = width;
    
    // start ncurses
    initscr(); 
    // do not print input values
    noecho();
    // terminate ncurses on ctrl + c
    cbreak();
    //enable color
    start_color();

    int x_max{getmaxx(stdscr)};

    this->system_window = newwin(500, 500, 0, 0);


    endwin();
}

void ConsoleGameEngine::Start() {

    m_bAtomActive = true;

    OnUserCreate();

    std::thread t = std::thread(&ConsoleGameEngine::GameThread, this);
    t.join();
}

void ConsoleGameEngine::GameThread() {

    auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

    keypad(system_window, TRUE);

    while (m_bAtomActive) {

        // Handle Timing
	    tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

        // Handle keyboard import
        std::memset(sKeyState, 0, 262 * sizeof(short));

        int c = wgetch(system_window);
        sKeyState[c] = 1;
        
        // Refresh frame
        OnUserUpdate(fElapsedTime);

    }
}

void ConsoleGameEngine::Draw(float x, float y, short c, short col) {
   // mvaddch(x, y, ACS_CKBOARD);
    mvaddch(x, y, ACS_DIAMOND);
  // mvaddch(x, y, '.');
}

void ConsoleGameEngine::DrawLine(int x1, int y1, int x2, int y2, short c, short col) {
    /*
    Draw(21.0,21.0,c,col);
    Draw(22.0,22.0,c,col);
    Draw(23.0,23.0,c,col);
    Draw(24.0,24.0,c,col);
    Draw(25.0,25.0,c,col);
    Draw(26.0,26.0,c,col);
    Draw(26.0,27.0,c,col);

    Draw(21.0,21.0,c,col);
    Draw(22.0,20.0,c,col);
    Draw(23.0,19.0,c,col);
    Draw(24.0,18.0,c,col);
    Draw(25.0,17.0,c,col);
    Draw(26.0,16.0,c,col);
    Draw(26.0,15.0,c,col);


    Draw(27.0,16.0,c,col);
    Draw(27.0,17.0,c,col);
    Draw(27.0,18.0,c,col);
    Draw(27.0,19.0,c,col);
    Draw(27.0,20.0,c,col);
    Draw(27.0,21.0,c,col);
    Draw(27.0,22.0,c,col);
    Draw(27.0,23.0,c,col);
    Draw(27.0,24.0,c,col);
    Draw(27.0,25.0,c,col);
    Draw(27.0,26.0,c,col);
    */
 
    
    // Iterators, counters required by algorithm
    int  x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

    // Calculate line deltas
    dx = x2 - x1;
    dy = y2 - y1;

    // Create a positive copy of deltas (makes iterating easier)
    dx1 = abs(dx);
    dy1 = abs(dy);

    // Calculate error intervals for both axis
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;

     // The line is X-axis dominant
    if (dy1 <= dx1) {
         // Line is drawn left to right
        if (dx >= 0) {
            x = x1; y = y1; xe = x2;
        } else { // Line is drawn right to left (swap ends)
            x = x2; y = y2; xe = x1;
        }
        Draw(x,y,c,col);
        // Rasterize the line
        for (i = 0; x < xe; i++) {
            x = x + 1;
            // Deal with octants...
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y = y + 1;
                } else {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            // Draw pixel from line span at
            // currently rasterized position
            Draw(x,y,c,col);
        }
    } else { // The line is Y-axis dominant
        // Line is drawn bottom to top
        if (dy >= 0) {
            x = x1; y = y1; ye = y2;
        } else { // Line is drawn top to bottom
            x = x2; y = y2; ye = y1;
        }
        Draw(x,y,c,col); // Draw first pixel
        // Rasterize the line
        for (i = 0; y < ye; i++) {
            y = y + 1;
            // Deal with octants...
            if (py <= 0) {
                py = py + 2 * dx1;
            } else {
                if ((dx < 0 && dy<0) || (dx > 0 && dy > 0)) {
                    x = x + 1;
                } else {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            // Draw pixel from line span at
            // currently rasterized position
            Draw(x,y,c,col);
        }
    }
    
}


