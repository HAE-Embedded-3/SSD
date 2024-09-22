#include "ssd.h"
#include "testScript.h"
#include "testShellApplication.h"

int main(void) {
	SSD<uint32_t> ssd;
    TestShellApplication<uint32_t> app(ssd);
    TestScript test_script;
    //È®ÀÎ¿ë
    ssd.write(2, 12345);
    ssd.write(234, 54321);
    ssd.read(2);
    ssd.read(514);

    test_script.addCommand("fullwrite 0xDEADBEEF");
    test_script.addCommand("fullread");

    app.registerTestScript(test_script);
    app.executeTestScript(0);

    return (0);
}