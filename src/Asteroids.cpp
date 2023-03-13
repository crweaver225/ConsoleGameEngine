#include "Asteroids.h"

Asteroids::Asteroids() {
    ConstructConsole(80,80);
    Start();
}

bool Asteroids::OnUserCreate() {
    /*
    vecModelShip = 
		{
			{ 0.0f, -2.5f},
			{-1.25f, +1.25f},
			{+1.25f, +1.25f}
		}; // A simple Isoceles Triangle
        */
       
       vecModelShip = 
		{
			{ 0.0f, -2.5f},
			{-2.5f, +2.5f},
			{+2.5f, +2.5f}
		}; // A simple Isoceles Triangle

    // Initialise Player Position
	player.x = 20;
	player.y = 20;
	player.dx = 0.0f;
	player.dy = 0.0f;
	player.angle = 0.0f;

    DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);

    refresh();

    return true;

}
bool Asteroids::OnUserUpdate(float elapsedTime) {

    if (sKeyState[KEY_UP] == 1) {
        player.x -= 1.0;
        clear();
    } else if (sKeyState[KEY_DOWN] == 1) {
        player.x += 1.0;
        clear();
    } else if (sKeyState[KEY_RIGHT] == 1) {
        player.angle += 5.0f; //* elapsedTime;
        clear();
    } else if (sKeyState[KEY_LEFT] == 1) {
        player.angle -= 5.0f; //* elapsedTime;
        clear();
    }

    DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);
    refresh();
}

void Asteroids::DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r, float s, short col) {
    
    // Create translated model vector of coordinate pairs
	std::vector<std::pair<float, float>> vecTransformedCoordinates;
	int verts = vecModelCoordinates.size();
	vecTransformedCoordinates.resize(verts);

    // Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

    // Draw Closed Polygon
	//for (int i = 0; i < verts + 1; i++) {
    for (int i = 0; i < verts; i++) {
		int j = (i + 1);
		DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, 
				vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, PIXEL_SOLID, col);
	}


}
