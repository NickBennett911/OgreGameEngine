#include <stdafx.h>
#include <application.h>

int main(int args, char ** blah) 
{
    ssuge::Application app;
    APPLICATION->initApp();      // app.initApp();
    APPLICATION->run();          // app.run();
    APPLICATION->closeApp();     // app.closeApp();
    return 0;
}
