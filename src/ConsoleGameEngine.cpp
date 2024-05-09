#include "ConsoleGameEngine.h"

void ConsoleGameEngine::ConstructConsole() {
    
    // start ncurses
    initscr(); 
    // do not print input values
    noecho();
    // terminate ncurses on ctrl + c
    cbreak();
    //enable color
    start_color();

    m_nScreenHeight = getmaxy(stdscr);
	m_nScreenWidth = getmaxx(stdscr);

    this->system_window = newwin(m_nScreenHeight, m_nScreenWidth, 0, 0);

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

        memset(sKeyState, 0, 262 * sizeof(short));

        nodelay(system_window, true);

        int c = wgetch(system_window);
        sKeyState[c] = 1;
        
        // Refresh frame
        OnUserUpdate(fElapsedTime);

        std::this_thread::sleep_for(std::chrono::milliseconds(_frameRate));

    }
}


bool ConsoleGameEngine::isOffScreen(float ox, float oy) const {
    if (ox < 0.0f) { 
		return true;
	} else if (ox >= float(ScreenWidth())) { 
		return true;
	} else if (oy < 0.0f) { 
		return true; 
	} else if (oy >= (float)ScreenHeight()) { 
		return true;
	}
    return false;
}

bool ConsoleGameEngine::isPointInsideCircle(float cx, float cy, float radius, float x, float y) const {
    return (sqrt((x - cx)*(x-cx) + (y-cy)*(y-cy))) < radius;
}

void ConsoleGameEngine::Draw(int x, int y) {
    mvaddch(y, x, ACS_DIAMOND);
}

void ConsoleGameEngine::DrawChar(int x, int y, chtype c_type) {
    mvaddch(y, x, c_type);
}

void ConsoleGameEngine::RenderText(int x, int y, const std::string& text) {
    mvprintw(y,x,text.c_str());
    move(0, 0);
}

void ConsoleGameEngine::DrawLine(int x1, int y1, int x2, int y2) {
    
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
        Draw(x,y);
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
            Draw(x,y);
        }
    } else { // The line is Y-axis dominant
        // Line is drawn bottom to top
        if (dy >= 0) {
            x = x1; y = y1; ye = y2;
        } else { // Line is drawn top to bottom
            x = x2; y = y2; ye = y1;
        }
        Draw(x,y); // Draw first pixel
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
            Draw(x,y);
        }
    }
}

int ConsoleGameEngine::ScreenWidth() const {
    return m_nScreenWidth;
}

int ConsoleGameEngine::ScreenHeight() const {
    return m_nScreenHeight;
}
