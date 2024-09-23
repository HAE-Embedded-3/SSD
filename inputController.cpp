#include "inputController.h"

// 예외 string
namespace Exception {
    const std::string INVALID_COMMAND = "INVALID COMMAND";
}

namespace StorageInfo {
    const std::string LBA_PREFIX = "0x";
    const int LBA_SIZE = 10;
} // namespace StorageInfo

bool InputController::isHex(const char ch) {
    return isdigit(ch) || (ch >= 'A' && ch <= 'F');
}

bool InputController::isValidLBA(const std::string &lba) {
    if (lba.empty() || lba.size() > 2) {
        return false;
    }

    for (char c : lba) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    return true;
}

bool InputController::isValidValue(const std::string &address) {
    if (address.substr(0, 2) != StorageInfo::LBA_PREFIX || address.size() != StorageInfo::LBA_SIZE)
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
            if (input.size() != 3 || !isValidLBA(input[1]) || !isValidValue(input[2])) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        case Command::READ:
            if (input.size() != 2 || !isValidLBA(input[1])) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        case Command::FULLWRITE:
            if (input.size() != 2 || !isValidValue(input[1])) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        case Command::EXIT:
        case Command::HELP:
        case Command::FULLREAD:
            if (input.size() != 1) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        case Command::TESTAPP:
            if (input.size() != 1) {
                throw std::invalid_argument(Exception::INVALID_COMMAND);
            }
            break;
        default:
            throw std::invalid_argument(Exception::INVALID_COMMAND);
    }
}

// " ", 공백 split
std::vector<std::string> InputController::split(std::string str) {
    std::stringstream ss(str);
    std::string buffer;

    std::vector<std::string> result;

    while (ss >> buffer) {
        result.push_back(buffer);
    }

    return result;
}

// delimiter split
std::vector<std::string> InputController::splitByDelimiter(std::string str, std::string delimiter) {
    std::vector<std::string> result;
    auto start = 0;
    auto end = str.find(delimiter);
    while (end != std::string::npos) {
        std::string sub_str = str.substr(start, end - start);
        if (!sub_str.empty()) {
            result.push_back(sub_str);
        }
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}

InputController::InputController() {
    commandMap = { {"write", Command::WRITE},
                  {"read", Command::READ},
                  {"exit", Command::EXIT},
                  {"help", Command::HELP},
                  {"fullwrite", Command::FULLWRITE},
                  {"fullread", Command::FULLREAD} };
}

std::vector<std::string> InputController::input() {
    std::string input;

    std::cout << ">>"
              << " ";

    getline(std::cin, input);

    std::vector<std::string> inputv = split(input);

    try {
        validateInput(inputv);
        return inputv;
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
        throw;
    }
}

std::vector<std::string> InputController::input(const std::string& cmd) {
    std::vector<std::string> inputv = split(cmd);

    try {
        validateInput(inputv);
        return inputv;
    }
    catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        throw;
    }
}

/// commandMap에서 찾음
Command InputController::findCommand(const std::string &cmd) {
    auto it = commandMap.find(cmd);
    if (it != commandMap.end()) {
        return it->second;
    }

    auto testCommand = parseTestCommand(cmd);
    if (testCommand.first == Command::TESTAPP)
        return Command::TESTAPP;

    return Command::INVALID;
}

// testApp 명령어
std::pair<Command, uint32_t> InputController::parseTestCommand(const std::string& cmd) {
    std::regex pattern(R"(testApp(\d+))");
    std::smatch matches;

    if (std::regex_match(cmd, matches, pattern)) {
        return { Command::TESTAPP, std::stoul(matches[1]) };
    }
    return { Command::INVALID, 0 };
}
