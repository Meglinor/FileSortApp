#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <thread>
#include <limits>

constexpr size_t KBYTE = 1024;
constexpr size_t MAX_MEMORY = KBYTE * KBYTE * 16;
using FILETYPE = unsigned int;
using VectorUInt = std::vector<FILETYPE>;

using std::cout;

class FileSort
{
public:
	FileSort();
	void startSort(const std::string& inputPath, const std::string& inputFile);
private:
	void sortAndSave(const std::string& outputFile);
	void sortAndSave(const std::string& outputFile, VectorUInt&& data);
	void sortAndMerge(const std::string& outputFile);
	void merge(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile);

	VectorUInt data_;
	size_t vectorSize_;
	size_t threadsMaxCount_ = 4;
	size_t tempFileCount_ = 0;
	std::string inputPath_;
};

