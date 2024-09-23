#ifndef _INPUT_VALIDATOR__
#define _INPUT_VALIDATOR__

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <regex>

/// <summary>
/// 입력값 제한
/// </summary>
enum class Command {
    WRITE,
    READ,
    EXIT,
    HELP,
    FULLWRITE,
    FULLREAD,
    TESTAPP,
    INVALID
};

/// <summary>
/// 입력, 유효성 검사
/// </summary>
class InputController {
  private:
    std::map<std::string, Command> commandMap;

    bool isHex(const char ch);
    bool isValidLBA(const std::string &lba);
    bool isValidValue(const std::string &address);
    void validateInput(const std::vector<std::string> &input);
    std::vector<std::string> split(std::string str);
    std::vector<std::string> splitByDelimiter(std::string str, std::string delimiter);

  public:
    InputController();
    InputController &operator=(const InputController &src) = delete;

    std::vector<std::string> input();
    std::vector<std::string> input(const std::string& cmd);
    Command findCommand(const std::string &cmd);
    std::pair<Command, uint32_t> parseTestCommand(const std::string& cmd);
};

#endif // !1 