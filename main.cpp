#include <SFML/Graphics.hpp>

#include <Helper.h>
#include <locale>

#include "game/include/Game.h"
#include "game/include/Level.h"
#include "game/include/Character.h"
#include "game/include/Camera.h"


#include "env_fixes.h"



class SomeClass {
public:
    explicit SomeClass(int) {}
};

SomeClass *getC() {
    return new SomeClass{2};
}



int main() {

    init_threads();


    Game game = Game();
    game.start();

    return 0;
}
