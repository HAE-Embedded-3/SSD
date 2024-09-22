#include "ssd.h"
#include "testScript.h"
#include "testShellApplication.h"

int main(void) {
	SSD<uint32_t> ssd;
    InputController input_controller;
    TestShellApplication<uint32_t> app(ssd,input_controller);

    app.start();
    //TestScript test_script;

    //test_script.addCommand("fullwrite 0xDEADBEEF");
    //test_script.addCommand("fullread");

    //app.registerTestScript(test_script);
    //app.executeTestScript(0);

    return (0);
}