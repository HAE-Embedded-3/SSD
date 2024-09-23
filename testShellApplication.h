#ifndef __TEST_SHELL_APPLICATION_H__
#define __TEST_SHELL_APPLICATION_H__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "inputController.h"
#include "ssd.h"
#include "testScript.h" 

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

    void write(const std::vector<std::string> &commands);
    void read(const std::vector<std::string> &commands);
    void exit();
    void help();
    void fullwrite(const std::vector<std::string>& commands);
    void fullread();
    void executeStorage(const std::vector<std::string> &commands); // storage에 동작 명령

  public:
    void start();
    void registerTestScript(TestScript &testScript);
    void executeTestScript(uint32_t index);
    TestShellApplication(Storage<T> &storage);
    TestShellApplication(Storage<T> &storage, const InputController &input_controller);
    TestShellApplication(const TestShellApplication &copy) = delete;
    TestShellApplication operator=(const TestShellApplication &src) = delete;
};

template <typename T>
void TestShellApplication<T>::write(const std::vector<std::string> &commands) {
    std::cout << "  [app write]" << std::endl;

    uint32_t idx = static_cast<uint32_t>(stoul(commands[1], nullptr, 10));
    std::string data = commands[2];

    _storage.write(idx, data);
}

template <typename T> 
void TestShellApplication<T>::read(const std::vector<std::string> &commands) {
    std::cout << "  [app read]" << std::endl;

    uint32_t idx = static_cast<uint32_t>(stoul(commands[1], nullptr, 10));
    //_storage.read(idx);
}

template <typename T> 
void TestShellApplication<T>::exit() {
    std::cout << "  [app exit]" << std::endl;
    std::exit(0);
}

template <typename T> 
void TestShellApplication<T>::help() {
    std::cout << "  [app help]" << std::endl;
}

template <typename T> 
void TestShellApplication<T>::fullwrite(const std::vector<std::string>& commands) {
    std::cout << "  [app fullwrite]" << std::endl;
        const std::string& data = commands[1];

    for (uint32_t idx = StorageInfo::MIN_LBA_IDX; idx <= StorageInfo::MAX_LBA_IDX; idx++) {
        std::cout << "  write idx : " << idx << std::endl;
        _storage.write(idx, data);
    }
}

template <typename T> 
void TestShellApplication<T>::fullread() {
    std::cout << "  [app fullread]" << std::endl;

    for (uint32_t idx = StorageInfo::MIN_LBA_IDX; idx <= StorageInfo::MAX_LBA_IDX; idx++) {
        std::cout << "  read idx : " << idx << std::endl;
        //_storage.read(idx);
    }
}

template <typename T>
void TestShellApplication<T>::executeStorage(const std::vector<std::string> &commands) {

    Command cmd = _input_controller.findCommand(commands[0]);

    switch (cmd) {
        case Command::WRITE:
            write(commands);
            break;
        case Command::READ:
            read(commands);
            break;
        case Command::EXIT:
            exit();
            break;
        case Command::HELP:
            help();
            break;
        case Command::FULLWRITE:
            fullwrite(commands);
            break;
        case Command::FULLREAD:
            fullread();
            break;
        case Command::TESTAPP: {
            auto testNum = _input_controller.parseTestCommand(commands[0]).second;
            executeTestScript(testNum);
            break;
        }
        default:
            break;
    }
}

template <typename T> void TestShellApplication<T>::start() {
    while (1) {
        std::cout << std::endl;
        std::vector<std::string> commands;

        try {
            commands = _input_controller.input();
        } catch (...) {
            continue;
        }

        executeStorage(commands);
    }
}

template <typename T> void TestShellApplication<T>::registerTestScript(TestScript &script) {
    _testScripts.push_back(script);
}

template <typename T> void TestShellApplication<T>::executeTestScript(uint32_t index) {
    /* todo: 해당 인덱스의 스크립트 순차적으로 실행 */
    try {
        auto& script = _testScripts.at(index - 1);
        for (auto& cmd : script.getCommands()) {
            auto command = _input_controller.input(cmd);
            executeStorage(command);
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

template <typename T>
TestShellApplication<T>::TestShellApplication(Storage<T> &storage) : _storage(storage) {
}

template <typename T>
TestShellApplication<T>::TestShellApplication(Storage<T> &storage,
                                              const InputController &input_controller)
    : _storage(storage), _input_controller(input_controller) {
}

#endif