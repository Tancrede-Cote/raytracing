#include "config.h"

int main(){
    App* app = new App();
    app->set_up_opengl();
    app->run();
    // delete app;
    return 0;
}