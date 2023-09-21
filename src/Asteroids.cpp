#include "Asteroids.h"

Asteroids::Asteroids() {

	_score = 0;
	_highScore = 0;

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
	vecAsteroids.push_back( { (int)4, 60.0f, 40.0f, -2.0f, 1.0f, 0.0 });
	vecAsteroids.push_back( { (int)3, 35.0f, 75.0f, 3.0f, -2.0f, 0.0 });
	vecAsteroids.push_back( { (int)2, 10.0f, 25.0f, -3.0f, 2.0f, 0.0 });
	vecAsteroids.push_back( { (int)5, 0.0f, 5.0f, 2.0f, -2.0f, 0.0 });

    refresh();

    return true;
}

void Asteroids::ResetGame() {

	_highScore = std::max(_highScore, _score);
	_score = 0;
	
	player.x = 25;
	player.y = 25;
	player.dx = 0.0f;
	player.dy = 0.0f;
	player.angle = 0.1f;

	vecAsteroids.clear();

	vecAsteroids.push_back( { (int)4, 20.0f, 20.0f, 8.0f, -6.0f, 0.0 });
	vecAsteroids.push_back( { (int)4, 100.0f, 20.0f, -5.0f, 3.0f, 0.0 });
	vecAsteroids.push_back( { (int)4, 50.0f, 40.0f, -2.0f, 1.0f, 0.0 });
	vecAsteroids.push_back( { (int)3, 35.0f, 75.0f, 3.0f, -2.0f, 0.0 });

	clear();
	refresh();
}

bool Asteroids::OnUserUpdate(float elapsedTime) {

	clear();

    if (sKeyState[KEY_UP] == 1) {
	   player.dx += sin(player.angle) * 100.0f * elapsedTime;
	   player.dy -= cos(player.angle) * 100.0f * elapsedTime;
    } else if (sKeyState[KEY_RIGHT] == 1) {
        player.angle -= 25.0f * elapsedTime;
    } else if (sKeyState[KEY_LEFT] == 1) {
        player.angle += 25.0f * elapsedTime;
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

				// Remove bullet
				a.remove = true;
				b.remove = true;

				if (a.hit == false) {
					vecAsteroids.push_back( { (int)2, a.x, a.y, 2.5f, 2.5f, 0.0, 0, 1 });
					vecAsteroids.push_back( { (int)2, a.x, a.y, -2.5f, -2.5f, 0.0, 0, 1 });
					vecAsteroids.push_back( { (int)2, a.x, a.y, 2.5f, -2.5f, 0.0, 0, 1 });
				} 

				int count = std::count_if(vecAsteroids.begin(),
                               vecAsteroids.end(),
                               [](sSpaceObject s) { return s.hit == false; });

				if (count < 5) {
					std::random_device dev;
    				std::mt19937 rng(dev());
					std::uniform_int_distribution<std::mt19937::result_type> dist10(-5,5);
					int noise = dist10(rng);
					vecAsteroids.push_back( { (int)4, 
											(float)m_nScreenWidth * noise, 
											(float)m_nScreenHeight * noise, 
											10.0f * sin(player.angle) + noise, 
											10.0f*cosf(player.angle) + noise, 
											0.0f} );
				}

				_score += 10;
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

	RenderText(2,2,std::string{"Score: " + std::to_string(_score)});
	RenderText(4,2,"High Score: " + std::to_string(_highScore));
	
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
