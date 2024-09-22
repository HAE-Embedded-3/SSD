#include "inputController.h"
#include <sstream>
#include <stdexcept>

bool InputController::isHex(const char ch) {
    return isdigit(ch) || (ch >= 'A' && ch <= 'F');
}

bool InputController::isValidLBA(const std::string &lba) {
    if (lba.empty() || lba.size() > 2) {
        return false;
    }

    for (char c : lba) {
        if (!std::isdigit(c)) {
            return false; // 숫자가 아닌 경우 false
        }
    }

    return true;
}

bool InputController::isValidAddress(const std::string &address) {
    if (address.substr(0, 2) != "0x" || address.size() != 10)
        return false;

    for (size_t i = 2; i < address.size(); ++i) {

        if (!isHex(address[i])) {
            return false;
        }
    }
    return true;
}

void InputController::validateInput(const std::vector<std::string> &input) {
    if (input.empty()) {
        throw std::invalid_argument(Exception::INVALID_COMMAND);
    }

    Command command = findCommand(input[0]);

    switch (command) {
        case Command::WRITE:
            if (input.size() != 3 || !isValidLBA(input[1]) || !isValidAddress(input[2])) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        case Command::READ:
            if (input.size() != 2 || !isValidLBA(input[1])) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        case Command::EXIT:
        case Command::HELP:
        case Command::FULLWRITE:
        case Command::FULLREAD:
            if (input.size() != 1) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        default:
            throw std::invalid_argument(Exception::INVALID_COMMAND);
    }
}

std::vector<std::string> InputController::split(std::string str) {
    std::stringstream ss(str);
    std::string buffer;

    std::vector<std::string> result;

    while (ss >> buffer) {
        result.push_back(buffer);
    }

    return result;
}

void InputController::splitByDelimiter(std::string str, std::string delimiter) {
}

InputController::InputController() {
    commandMap = {{"write", Command::WRITE},
                  {"read", Command::READ},
                  {"exit", Command::EXIT},
                  {"help", Command::HELP},
                  {"fullwrite", Command::FULLWRITE},
                  {"fullread", Command::FULLREAD}};
}

void InputController::input() {
    std::string input;

    std::cout << ">>" << " ";

    getline(std::cin, input);

    if (input == "end") {
        return;
    }

    std::vector<std::string> inputv = split(input);

    try {
        validateInput(inputv);
        std::cout << "input : " << input << std::endl;
        std::cout << "input size : " << inputv.size() << std::endl;
    } catch (std::invalid_argument) {
        std::cout << "INVALID COMMAND" << std::endl;
    }
    std::cout << std::endl;
}

Command InputController::findCommand(const std::string &cmd) {
    auto it = commandMap.find(cmd);
    if (it != commandMap.end()) {
        return it->second;
    }

    return Command::INVALID;
}
