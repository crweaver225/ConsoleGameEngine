#include "ConsoleGameEngine.h"


class Asteroids: public ConsoleGameEngine {
    public:

        Asteroids();

        virtual bool OnUserCreate() override;
        virtual bool OnUserUpdate(float elapsedTime) override;

    private:
         struct sSpaceObject {
		    int nSize;
		    float x;
		    float y;
		    float dx;
		    float dy;
		    float angle;
            bool remove = false;
	    }; 

        void ResetGame();

        std::vector<std::pair<float, float>> vecModelShip;
        std::vector<std::pair<float, float>> vecModelAsteroid;
        std::vector<std::pair<float, float>> vecModelBullets;

        sSpaceObject player;
        std::vector<sSpaceObject> vecAsteroids;
        std::vector<sSpaceObject> vecBullets;

        void WrapCoordinates(float ix, float iy, float &ox, float &oy);
        void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r, float s = 0.65, short col = COLOUR::FG_WHITE);

         
};