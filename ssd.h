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
	// 16진수 문자열이 "0x"로 시작하는지 확인
	if (hexStr.substr(0, 2) != "0x") {
		throw std::invalid_argument("String must start with '0x'");
	}

	T result;
	std::stringstream ss;

	// 16진수 문자열을 스트림에 삽입
	ss << std::hex << hexStr.substr(2); // "0x" 이후의 부분만 사용

	// 스트림에서 변환된 값을 읽어옴
	ss >> result;

	// 변환이 실패한 경우 예외 처리
	if (ss.fail()) {
		throw std::runtime_error("Conversion failed");
	}

	return result;
}

//로지컬 블럭 - 데이터형 설정 + string 형으로 바꿔주는 함수
template <typename T>
class LogicalBlock {
public:
    T data;

    LogicalBlock() : data(0) {}
    LogicalBlock(T value) : data(value) {}
	LogicalBlock(const std::string& str) {
		data = hexStringToInteger<T>(str);  // string을 int로 변환
	}

    std::string toString() const {
        return std::to_string(data);
    }
    friend std::ostream& operator<<(std::ostream& os, const LogicalBlock<T>& block) {
        os << block.data; // T의 데이터를 출력
        return os;
    }
};

//저장소 인터페이스 - write, read 함수
template <typename T>
class Storage {
public:
    // 순수가상함수 선언
    virtual void write(uint32_t index, std::string data) = 0;
    virtual void read(uint32_t index) = 0;
};

//SSD 저장소 - 저장 데이터: 로지컬블럭 벡터
template <typename T>
class SSD : public Storage<T> {
public:
    std::vector<LogicalBlock<T>> ssd;

    void init(); // 파일 생성
    void write(uint32_t index, std::string data);
    void read(uint32_t index);

    SSD() {
        std::cout << "SSD init" << std::endl;
        init();
    }
};
//내부에 넣어도 되지 않을까...
//nand.txt 기록 함수 -> nand.txt에 로지컬블럭 벡터 ssd write
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
	else { // nand.txt 파일 없을 때
		ssd.resize(100, LogicalBlock<T>{ 0 });
		std::ofstream writeFile;
		writeFile.open("nand.txt", std::ios::out | std::ios::trunc);
		if (writeFile.is_open())
		{
			for (const auto& str : ssd) { // T 자료형의 길이만큼 setw 설정
				writeFile << "0x" << std::setw(8) << std::hex << std::setfill('0') << str << std::endl;
			}
			writeFile.close();
		}
		else {
			std::cout << "can't create nand.txt\n";
		}
	}
}

//nand에 데어터 수정
template <typename T>
void SSD<T>::write(uint32_t index, std::string data) {
	//LogicalBlock<T> block(data);

	//100 이하의 index 필요 -> 조건에 맞으면 data 삽입
	if (index >= 0 && index < 100) 
		ssd[index] = data;
	else 
		std::cout << "WRITE INDEX ERROR" << std::endl;

	std::ifstream readFile;
	readFile.open("nand.txt", std::ios::in | std::ios::out | std::ios::app);
	//파일 데이터 수정하는 과정
	if (readFile.is_open())
	{
		//임시 string 벡터 생성
		std::vector<std::string> tmp;
		//읽기용 string 변수
		std::string line;
		//txt파일 읽어 임시 벡터에 저장
		while (std::getline(readFile, line)) {
			tmp.push_back(line);
		}
		//inex가 현재 저장된 데이터 양보다 작은 경우 데어터를 데이터 string으로 변환 ???
		//if (index < tmp.size()) tmp[index] = //uint32ToHexString(data.toString());
		if (index < tmp.size())
			tmp[index] = data;
		//if (index < tmp.size()) tmp[index] = data;
		//파일 닫기???
		readFile.close();
		std::ofstream writeFile;
		writeFile.open("nand.txt", std::ios::out | std::ios::trunc);
		//임시 벡터에 저장한 데이터들 nand.txt에 저장
		int linenum = 0;
		for (const auto& str : tmp) {
			if (linenum++ != index)
				writeFile << str << std::endl;
			else 
				writeFile << std::setw(8) << std::hex << std::setfill('0') << str << std::endl;
		}
		writeFile.close();
	}
	//파일 못 여는 경우 에러 출력
	else {
		std::cout << "can't open the file \n";
	}
}

template <typename T>
void SSD<T>::read(uint32_t index) {
	//index 범위 확인
	if (index < 0 || index >= 100) {
		std::cerr << "READ INDEX ERROR" << std::endl;
		return;
	}
	//필요한 데이터 result.txt에 저장 및 출력
	LogicalBlock<T> read_data = ssd[index];
	std::fstream read_file("result.txt", std::ios::out | std::ios::trunc);
	if (read_file.is_open()) {
		read_file << "0x" << std::hex << read_data << std::endl;
		std::cout << "0x" << std::hex << read_data << std::endl;
		read_file.close();
	}
	//파일 못 여는 경우 에러
	else {
		std::cerr << "can't open the file \n";
	}
	

}

//int main(void) {
//	SSD<LogicalBlock<uint32_t>> ssd;
//	ssd.(write3, 8);
//}

#endif