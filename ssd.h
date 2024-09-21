#ifndef __SSD_HPP__
#define __SSD_HPP__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

template <typename T>
class LogicalBlock {
public:
    T data;

    LogicalBlock() : data(0) {}
    LogicalBlock(T value) : data(value) {}

    std::string toString() const {
        return std::to_string(data);
    }
    friend std::ostream& operator<<(std::ostream& os, const LogicalBlock<T>& block) {
        os << block.data; // T의 데이터를 출력
        return os;
    }
};

template <typename T>
class Storage {
public:
    // 순수가상함수 선언
    virtual void write(uint32_t index, LogicalBlock<T> data) = 0;
    virtual void read(uint32_t index) = 0;
};

template <typename T>
class SSD : public Storage<T> {
public:
    std::vector<LogicalBlock<T>> ssd;
    void init(); // 파일 생성

public:
    void write(uint32_t index, LogicalBlock<T> data);
    void read(uint32_t index);

    SSD() {
        std::cout << "SSD init" << std::endl;
        init();
    }
};

template <typename T>
void SSD<T>::init() {
	ssd.resize(100, T{ 0 });

	std::fstream writeFile;
	writeFile.open("nand.txt", std::ios::out | std::ios::app);
	if (writeFile.is_open())
	{
		for (const auto& str : ssd) {
			writeFile << str << std::endl;
		}
	}
	writeFile.close();
}

template <typename T>
void SSD<T>::write(uint32_t index, LogicalBlock<T> data) {

	if (index >= 0 && index < 100) ssd[index] = data;

	std::fstream readFile;
	readFile.open("nand.txt", std::ios::in | std::ios::out | std::ios::app);
	if (readFile.is_open())
	{
		std::vector<std::string> tmp;
		std::string line;
		while (std::getline(readFile, line)) {
			tmp.push_back(line);
		}

		if (index < tmp.size()) tmp[index] = data.toString();
		readFile.close();
		readFile.open("nand.txt", std::ios::out | std::ios::trunc);

		for (const auto& str : tmp) {
			readFile << str << std::endl;
		}
	}
	else {
		std::cout << "can't open the file \n";
	}
}

template <typename T>
void SSD<T>::read(uint32_t index) {

}

//int main(void) {
//	SSD<LogicalBlock<uint32_t>> ssd;
//	ssd.write(3, LogicalBlock<uint32_t>(8));
//}

#endif