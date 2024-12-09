#include <Helper.h>
#include <locale>

#include "game/include/Game.h"

//////////////////////////////////////////////////////////////////////
/// NOTE: this include is needed for environment-specific fixes     //
/// You can remove this include and the call from main              //
/// if you have tested on all environments, and it works without it //
#include "env_fixes.h"                                              //
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// This class is used to test that the memory leak checks work as expected even when using a GUI
// class SomeClass {
// public:
//     explicit SomeClass(int) {}
// };

// SomeClass *getC() {
//     return new SomeClass{2};
// }
//////////////////////////////////////////////////////////////////////


int main() {
    ////////////////////////////////////////////////////////////////////////
    /// NOTE: this function call is needed for environment-specific fixes //
    init_threads();                                                       //
    ////////////////////////////////////////////////////////////////////////
    ///

    Game game = Game();
    game.start();

    return 0;
}
