#include "Engine3D.h"

int main(){
    Engine3D engine(1280, 720);

    while (!engine.closedWindow()){
        engine.run();
    }

    return 0;
}