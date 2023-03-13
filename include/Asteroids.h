#include "ConsoleGameEngine.h"


class Asteroids: public ConsoleGameEngine {
    public:

        Asteroids();

        virtual bool OnUserCreate() override;
        virtual bool OnUserUpdate(float elapsedTime) override;

    private:
        std::vector<std::pair<float, float>> vecModelShip;
        void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r, float s = 1.0, short col = COLOUR::FG_WHITE);

        struct sSpaceObject {
		    int nSize;
		    float x;
		    float y;
		    float dx;
		    float dy;
		    float angle;
	    } player;
};