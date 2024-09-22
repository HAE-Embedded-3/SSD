#ifndef __TEST_SHELL_APPLICATION_H__
#define __TEST_SHELL_APPLICATION_H__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ssd.h"
#include "testScript.h"
#include "inputController.h"

/*
template <typename T>
class LogicalBlock;

template <typename T>
class Storage;
*/

template <typename T> class TestShellApplication {
  private:
    Storage<T> &_storage;
    std::vector<TestScript> _testScripts{};
    InputController _input_controller;

    void write();
    void read();
    void exit();
    void help();
    void fullwrite();
    void fullread();

  public:
    void start();
    void registerTestScript(TestScript &testScript);
    void executeTestScript(uint32_t index);
    TestShellApplication(Storage<T> &storage);
    TestShellApplication(Storage<T> &storage,const InputController input_controller);
    TestShellApplication(const TestShellApplication &copy) = delete;
    TestShellApplication operator=(const TestShellApplication &src) = delete;
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
        _input_controller.input();
    }
}

template <typename T> void TestShellApplication<T>::registerTestScript(TestScript &script) {
    _testScripts.push_back(script);
}

template <typename T> void TestShellApplication<T>::executeTestScript(uint32_t index) {
    /* todo: 해당 인덱스의 스크립트 순차적으로 실행 */
    for (auto &script : _testScripts) {
        for (auto &command : script.getCommands()) {
            std::cout << command << std::endl;
        }
    }
    std::cout << std::endl;
}

template <typename T>
TestShellApplication<T>::TestShellApplication(Storage<T> &storage) : _storage(storage) {
}

template <typename T>
TestShellApplication<T>::TestShellApplication(Storage<T> &storage,const InputController input_controller) 
    : _storage(storage),
      _input_controller(input_controller){

}


#endif