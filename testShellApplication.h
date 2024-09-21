#ifndef __TEST_SHELL_APPLICATION_H__
#define __TEST_SHELL_APPLICATION_H__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "testScript.h"

template <typename T>
class LogicalBlock;

template <typename T>
class Storage;

template <typename T> class TestShellApplication {
private:
    Storage<T>& _storage;
    std::vector<TestScript> _testScripts{};

    void write();
    void read();
    void exit();
    void help();
    void fullwrite();
    void fullread();
    std::vector<std::string> split(std::string str, char delimiter);

public:
    void start();
    void registerTestScript(TestScript& testScript);
    void executeTestScript(uint32_t index);
    TestShellApplication(Storage<T>& storage);
    TestShellApplication(const TestShellApplication& copy) = delete;
    TestShellApplication operator=(const TestShellApplication& src) = delete;
};

template <typename T> void TestShellApplication<T>::write() {
}

template <typename T> void TestShellApplication<T>::read() {
}

template <typename T> void TestShellApplication<T>::exit() {
}

template <typename T> void TestShellApplication<T>::help() {
}

template <typename T> void TestShellApplication<T>::fullwrite() {
}

template <typename T> void TestShellApplication<T>::fullread() {
}

template <typename T> void TestShellApplication<T>::start() {
    while (1) {
        std::string test;

        getline(std::cin, test);

        if (test == "end") {
            break;
        }

        std::vector<std::string> input = split(test, ' ');

    }
}

template<typename T> void TestShellApplication<T>::registerTestScript(TestScript& script) {
    _testScripts.push_back(script);
}

template <typename T>
void TestShellApplication<T>::executeTestScript(uint32_t index) {
    /* todo: 해당 인덱스의 스크립트 순차적으로 실행 */
    for (auto& script : _testScripts) {
        for (auto& command : script.getCommands()) {
            std::cout << command << std::endl;
        }
    }
    std::cout << std::endl;
}

template <typename T>
TestShellApplication<T>::TestShellApplication(Storage<T>& storage) : _storage(storage) {}

template <typename T>
std::vector<std::string> TestShellApplication<T>::split(std::string str, char Delimiter) {
    std::istringstream iss(str);
    std::string buffer;

    std::vector<std::string> result;

    while (getline(iss, buffer, Delimiter)) {
        result.push_back(buffer);
    }

    return result;
}

#endif
