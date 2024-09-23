#ifndef __SSD_HPP__
#define __SSD_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <vector>
#include <string>
#include <iomanip>

template <typename T>
T hexStringToInteger(const std::string& hexStr) {
	// 16���� ���ڿ��� "0x"�� �����ϴ��� Ȯ��
	if (hexStr.substr(0, 2) != "0x") {
		throw std::invalid_argument("String must start with '0x'");
	}

	T result;
	std::stringstream ss;

	// 16���� ���ڿ��� ��Ʈ���� ����
	ss << std::hex << hexStr.substr(2); // "0x" ������ �κи� ���

	// ��Ʈ������ ��ȯ�� ���� �о��
	ss >> result;

	// ��ȯ�� ������ ��� ���� ó��
	if (ss.fail()) {
		throw std::runtime_error("Conversion failed");
	}

	return result;
}

//������ �� - �������� ���� + string ������ �ٲ��ִ� �Լ�
template <typename T>
class LogicalBlock {
public:
    T data;

    LogicalBlock() : data(0) {}
    LogicalBlock(T value) : data(value) {}
	LogicalBlock(const std::string& str) {
		data = hexStringToInteger<T>(str);  // string�� int�� ��ȯ
	}

    std::string toString() const {
        return std::to_string(data);
    }
    friend std::ostream& operator<<(std::ostream& os, const LogicalBlock<T>& block) {
        os << block.data; // T�� �����͸� ���
        return os;
    }
};

//����� �������̽� - write, read �Լ�
template <typename T>
class Storage {
public:
    // ���������Լ� ����
    virtual void write(uint32_t index, std::string data) = 0;
    virtual void read(uint32_t index) = 0;
};

//SSD ����� - ���� ������: �����ú� ����
template <typename T>
class SSD : public Storage<T> {
public:
    std::vector<LogicalBlock<T>> ssd;

    void init(); // ���� ����
    void write(uint32_t index, std::string data);
    void read(uint32_t index);

    SSD() {
        std::cout << "SSD init" << std::endl;
        init();
    }
};
//���ο� �־ ���� ������...
//nand.txt ��� �Լ� -> nand.txt�� �����ú� ���� ssd write
template <typename T>
void SSD<T>::init() {
	std::ifstream readFile;
	readFile.open("nand.txt", std::ios::in);
	if (readFile.is_open())
	{
		//std::cout << "open! \n";
		ssd.reserve(100);
		std::string line;
		while (std::getline(readFile, line)) {
			ssd.push_back(LogicalBlock<T> (line));
		}

		readFile.close();
	}
	else { // nand.txt ���� ���� ��
		ssd.resize(100, LogicalBlock<T>{ 0 });
		std::ofstream writeFile;
		writeFile.open("nand.txt", std::ios::out | std::ios::trunc);
		if (writeFile.is_open())
		{
			for (const auto& str : ssd) { // T �ڷ����� ���̸�ŭ setw ����
				writeFile << "0x" << std::setw(8) << std::hex << std::setfill('0') << str << std::endl;
			}
			writeFile.close();
		}
		else {
			std::cout << "can't create nand.txt\n";
		}
	}
}

//nand�� ������ ����
template <typename T>
void SSD<T>::write(uint32_t index, std::string data) {
	//LogicalBlock<T> block(data);

	//100 ������ index �ʿ� -> ���ǿ� ������ data ����
	if (index >= 0 && index < 100) 
		ssd[index] = data;
	else 
		std::cout << "WRITE INDEX ERROR" << std::endl;

	std::ifstream readFile;
	readFile.open("nand.txt", std::ios::in | std::ios::out | std::ios::app);
	//���� ������ �����ϴ� ����
	if (readFile.is_open())
	{
		//�ӽ� string ���� ����
		std::vector<std::string> tmp;
		//�б�� string ����
		std::string line;
		//txt���� �о� �ӽ� ���Ϳ� ����
		while (std::getline(readFile, line)) {
			tmp.push_back(line);
		}
		//inex�� ���� ����� ������ �纸�� ���� ��� �����͸� ������ string���� ��ȯ ???
		//if (index < tmp.size()) tmp[index] = //uint32ToHexString(data.toString());
		if (index < tmp.size())
			tmp[index] = data;
		//if (index < tmp.size()) tmp[index] = data;
		//���� �ݱ�???
		readFile.close();
		std::ofstream writeFile;
		writeFile.open("nand.txt", std::ios::out | std::ios::trunc);
		//�ӽ� ���Ϳ� ������ �����͵� nand.txt�� ����
		int linenum = 0;
		for (const auto& str : tmp) {
			if (linenum++ != index)
				writeFile << str << std::endl;
			else 
				writeFile << std::setw(8) << std::hex << std::setfill('0') << str << std::endl;
		}
		writeFile.close();
	}
	//���� �� ���� ��� ���� ���
	else {
		std::cout << "can't open the file \n";
	}
}

template <typename T>
void SSD<T>::read(uint32_t index) {
	//index ���� Ȯ��
	if (index < 0 || index >= 100) {
		std::cerr << "READ INDEX ERROR" << std::endl;
		return;
	}
	//�ʿ��� ������ result.txt�� ���� �� ���
	LogicalBlock<T> read_data = ssd[index];
	std::fstream read_file("result.txt", std::ios::out | std::ios::trunc);
	if (read_file.is_open()) {
		read_file << "0x" << std::hex << read_data << std::endl;
		std::cout << "0x" << std::hex << read_data << std::endl;
		read_file.close();
	}
	//���� �� ���� ��� ����
	else {
		std::cerr << "can't open the file \n";
	}
	

}

//int main(void) {
//	SSD<LogicalBlock<uint32_t>> ssd;
//	ssd.(write3, 8);
//}

#endif