#include "Asteroids.h"

Asteroids::Asteroids() {

    ConstructConsole();
    Start();
}

bool Asteroids::OnUserCreate() {
    
    vecModelShip = 
		{
			{ 0.0f, -4.5f},
			{-3.5f, +3.5f},
			{+3.5f, +3.5f}
		}; // A simple Isoceles Triangle

    // Initialise Player Position
	player.x = 25;
	player.y = 25;
	player.dx = 0.0f;
	player.dy = 0.0f;
	player.angle = 0.1f;

    DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);

	int verts = 20;
	for (int i = 0; i < verts; i++) {
		float noise = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
		vecModelAsteroid.push_back(std::make_pair(noise * sinf(((float)i / (float)verts) * 6.28318f), noise * cosf(((float)i / float(verts) * 6.28318f))));
	}

	vecAsteroids.push_back( { (int)4, 20.0f, 20.0f, 8.0f, -6.0f, 0.0 });
	vecAsteroids.push_back( { (int)4, 100.0f, 20.0f, -5.0f, 3.0f, 0.0 });

    refresh();

    return true;
}

void Asteroids::ResetGame() {
	
	player.x = 25;
	player.y = 25;
	player.dx = 0.0f;
	player.dy = 0.0f;
	player.angle = 0.1f;

	vecAsteroids.clear();

	vecAsteroids.push_back( { (int)4, 20.0f, 20.0f, 8.0f, -6.0f, 0.0 });
	vecAsteroids.push_back( { (int)4, 100.0f, 20.0f, -5.0f, 3.0f, 0.0 });

	clear();
	refresh();
}

bool Asteroids::OnUserUpdate(float elapsedTime) {

	clear();

    if (sKeyState[KEY_UP] == 1) {
	   player.dx += sin(player.angle) * 100.0f * elapsedTime;
	   player.dy -= cos(player.angle) * 100.0f * elapsedTime;
    } else if (sKeyState[KEY_RIGHT] == 1) {
        player.angle -= 7.5f * elapsedTime;
    } else if (sKeyState[KEY_LEFT] == 1) {
        player.angle += 7.5f * elapsedTime;
    } else if (sKeyState[' ']) {
		vecBullets.push_back({(int)0, player.x, player.y, 50.0f * sinf(player.angle), -50.0f * cosf(player.angle), 100.0f});
	}

	player.x += player.dx * elapsedTime;
	player.y += player.dy * elapsedTime;

	WrapCoordinates(player.x, player.y, player.x, player.y);

    DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);

	for (auto &b : vecBullets) {
		b.x += b.dx * elapsedTime;
		b.y += b.dy * elapsedTime;
		b.angle -= 1.0f * elapsedTime;
		ConsoleGameEngine::Draw(b.x, b.y);
		for (auto &a : vecAsteroids) {

			if (isPointInsideCircle(a.x, a.y, a.nSize, b.x, b.y)) {

				a.remove = true;
				b.remove = true;

				vecAsteroids.push_back({ (int)4, 30.0f * sinf(player.angle - 3.14159f/2.0f) + player.x, 
				30.0f * cosf(player.angle - 3.14159f/2.0f) + player.y, 
				10.0f * sin(player.angle), 10.0f*cosf(player.angle), 0.0f});
			} 
		}
		if (isOffScreen(b.x, b.y)) { b.remove = true; }
	}

	auto i = std::remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](sSpaceObject o) { return (o.remove == true);});
	if (i != vecAsteroids.end()) {
		vecAsteroids.erase(i);
	}

	auto j = std::remove_if(vecBullets.begin(), vecBullets.end(), [&](sSpaceObject o) { return (o.remove == true);});
	if (j != vecBullets.end()) {
		vecBullets.erase(j);
	}

	for (auto &a : vecAsteroids) {

		if (isPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y)) {
			ResetGame();
		}  

		a.x += a.dx * elapsedTime;
		a.y += a.dy * elapsedTime;
		a.angle += 0.5f * elapsedTime;

		WrapCoordinates(a.x, a.y, a.x, a.y);

		DrawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, (float)a.nSize, FG_YELLOW);
	}
	
    refresh();
}

void Asteroids::WrapCoordinates(float ix, float iy, float &ox, float &oy) {
	if (ix < 0.0f) { 
		ox = (float)ScreenWidth() - ix; 
	}
	if (ix >= float(ScreenWidth())) { 
		ox = ix - (float)ScreenWidth(); 
	}
	if (iy < 0.0f) { 
		oy = iy + (float)ScreenHeight(); 
	}
	if (iy >= (float)ScreenHeight()) { 
		oy = iy - (float)ScreenHeight();
	}
}

void Asteroids::DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r, float s, short col) {
    
    // Create translated model vector of coordinate pairs
	std::vector<std::pair<float, float>> vecTransformedCoordinates;
	int verts = vecModelCoordinates.size();
	vecTransformedCoordinates.resize(verts);

    // Rotate
		for (int i = 0; i < verts; i++) {
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++) {
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++) {
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

    // Draw Closed Polygon
    for (int i = 0; i < verts + 1; i++) {
		int j = (i + 1);
		DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, 
				vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, PIXEL_SOLID, col);
	}

}
