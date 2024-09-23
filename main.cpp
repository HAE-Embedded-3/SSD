#include "ssd.h"
#include "testScript.h"
#include "testShellApplication.h"

int main(void) {
	SSD<uint32_t> ssd;
    InputController input_controller;
    TestShellApplication<uint32_t> app(ssd,input_controller);
    TestScript test_script;

    test_script.addCommand("fullwrite 0xDEADBEEF");
    test_script.addCommand("fullread");
    test_script.addCommand("write 0 0x10000000");
    test_script.addCommand("read 0");

    app.registerTestScript(test_script);
    app.start();

    return (0);
}