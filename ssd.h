#ifndef __SSD_HPP__
#define __SSD_HPP__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//������ �� - �������� ���� + string ������ �ٲ��ִ� �Լ�
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
        os << block.data; // T�� �����͸� ���
        return os;
    }
};

//����� �������̽� - write, read �Լ�
template <typename T>
class Storage {
public:
    // ���������Լ� ����
    virtual void write(uint32_t index, LogicalBlock<T> data) = 0;
    virtual void read(uint32_t index) = 0;
};

//SSD ����� - ���� ������: �����ú� ����
template <typename T>
class SSD : public Storage<T> {
public:
    std::vector<LogicalBlock<T>> ssd;

    void init(); // ���� ����
    void write(uint32_t index, LogicalBlock<T> data);
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
	ssd.resize(100, T{ 0 });

	std::fstream writeFile;
	writeFile.open("nand.txt", std::ios::out | std::ios::trunc);
	if (writeFile.is_open())
	{
		for (const auto& str : ssd) {
			writeFile << str << std::endl;
		}
	}
	writeFile.close();
}

//nand�� ������ ����
template <typename T>
void SSD<T>::write(uint32_t index, LogicalBlock<T> data) {

	//100 ������ index �ʿ� -> ���ǿ� ������ data ����
	if (index >= 0 && index < 100) ssd[index] = data;
	else std::cout << "WRITE INDEX ERROR" << std::endl;

	std::fstream readFile;
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
		if (index < tmp.size()) tmp[index] = data.toString();
		//���� �ݱ�???
		readFile.close();
		readFile.open("nand.txt", std::ios::out | std::ios::trunc);
		//�ӽ� ���Ϳ� ������ �����͵� nand.txt�� ����
		for (const auto& str : tmp) {
			readFile << str << std::endl;
		}
		readFile.close();
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
		read_file << read_data.toString() << std::endl;
		std::cout << "��� ���: " << read_data.toString() << std::endl;
		read_file.close();
	}
	//���� �� ���� ��� ����
	else {
		std::cerr << "can't open the file \n";
	}
	
	
}

//int main(void) {
//	SSD<LogicalBlock<uint32_t>> ssd;
//	ssd.write(3, LogicalBlock<uint32_t>(8));
//}

#endif