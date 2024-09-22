#ifndef _INPUT_VALIDATOR__
#define _INPUT_VALIDATOR__

#include <iostream>
#include <map>
#include <string>
#include <vector>

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
    bool isValidAddress(const std::string &address);
    void validateInput(const std::vector<std::string> &input);
    std::vector<std::string> split(std::string str);
    std::vector<std::string> splitByDelimiter(std::string str, std::string delimiter);

  public:
    InputController();
    InputController &operator=(const InputController &src) = delete;

    std::vector<std::string> input();
    Command findCommand(const std::string &cmd);
};

#endif // !1 