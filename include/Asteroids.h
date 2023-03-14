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
	    }; 

        std::vector<std::pair<float, float>> vecModelShip;
        std::vector<std::pair<float, float>> vecModelAsteroid;

        sSpaceObject player;
        std::vector<sSpaceObject> vecAsteroids;

        void WrapCoordinates(float ix, float iy, float &ox, float &oy);
        void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r, float s = 0.65, short col = COLOUR::FG_WHITE);

         
};