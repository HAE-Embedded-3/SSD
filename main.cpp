#include "ssd.h"
#include "testScript.h"
#include "testShellApplication.h"

int main(void) {
	SSD<uint32_t> ssd;
    InputController input_controller;
    TestShellApplication<uint32_t> app(ssd,input_controller);
    TestScript test_script1, test_script2;

    test_script1.addCommand("fullwrite 0xABCDFFFF");
    app.registerTestScript(test_script1);

    test_script2.addCommand("fullread");
    app.registerTestScript(test_script2);

    app.start();

    return (0);
}