#include "Engine3D.h"

int main(){
    Engine3D engine;

    while (!engine.closedWindow()){
        engine.run();
    }

    return 0;
}