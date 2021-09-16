#include "FileSort.h"


FileSort::FileSort()
{
	auto i = std::thread::hardware_concurrency();
	if (i > 0) threadsMaxCount_ = i;
	vectorSize_ = MAX_MEMORY / (threadsMaxCount_ * sizeof(FILETYPE));
	data_.reserve(vectorSize_);
}

void FileSort::startSort(const std::string& inputPath, const std::string& inputFile)
{
	inputPath_ = inputPath;
	cout << "Start sorting file - " << inputPath_ + inputFile << "\n";
	std::ifstream input;
	input.open(inputPath_ + inputFile, std::ios::binary);
	FILETYPE buffer;
	tempFileCount_ = 0;
	size_t readCount = 0;
	while (!input.eof())
	{
//		VectorUInt data;
//		data.reserve(vectorSize_);
//		while (readCount < vectorSize_ && input.read(reinterpret_cast<char*>(&buffer), sizeof(FILETYPE)))
		while (readCount < vectorSize_ && input.read(reinterpret_cast<char*>(&buffer), sizeof(FILETYPE)))
		{
			++readCount;
			data_.emplace_back(buffer);
		}
		std::string fPath = inputPath_ + "tmpout" + std::to_string(tempFileCount_++);
		cout << "Sorting and saving " << fPath << "\n";
		std::thread thr1([&fPath, this]() {sortAndSave(fPath); });
		thr1.join();
//		sortAndSave(fPath);
		readCount = 0;
	} 
	input.close();
	data_.reserve(0);
	sortAndMerge(inputPath_ + "output");
}

void FileSort::sortAndSave(const std::string& outputFile)
{
	std::ofstream output;
	std::sort(data_.begin(), data_.end());
	output.open(outputFile, std::ios::binary);
	if (output)
	{
		for_each(data_.begin(), data_.end(), [&output](FILETYPE i) {output.write(reinterpret_cast<const char*>(&i), sizeof(FILETYPE)); });
		output.close();
	}
	data_.clear();
}

void FileSort::sortAndSave(const std::string& outputFile, VectorUInt&& data)
{
	std::ofstream output;
	std::sort(data.begin(), data.end());
 	output.open(outputFile, std::ios::binary);
	for_each(data.begin(), data.end(), [&output](FILETYPE i) {output.write(reinterpret_cast<const char*>(&i), sizeof(FILETYPE)); });
	output.close();
}

void FileSort::sortAndMerge(const std::string& outputFile)
{
//	while (true)
	{
		size_t iteration = 0;
		for (size_t i = 0; i < tempFileCount_; ++i)
		{
			if (i + 1 < tempFileCount_)
			{
				cout << "Merge " << "tmpout" + std::to_string(i) << " and " << "tmpout" + std::to_string(i+1) << " into " << std::to_string(iteration) + std::to_string(i) << "\n";
				merge(inputPath_ + "tmpout" + std::to_string(i),
					  inputPath_ + "tmpout" + std::to_string(i+1),
					  inputPath_ + std::to_string(iteration) + std::to_string(i));
				++i;
			}
			else
			{
				cout << "Merge " << "tmpout" + std::to_string(i-1) << " and " << "tmpout" + std::to_string(i) << " into " << std::to_string(iteration) + std::to_string(i) <<  "\n";
				merge(inputPath_ + std::to_string(iteration) + std::to_string(i),
  					  inputPath_ + "tmpout" + std::to_string(i),
					  inputPath_ + std::to_string(iteration) + std::to_string(i));
				break;
				++iteration;
			}
		}
	}
}

void FileSort::merge(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile)
{
	std::ifstream input1(inputFile1, std::ios::binary);
	std::ifstream input2(inputFile2, std::ios::binary);
	std::ofstream output(outputFile, std::ios::binary);
	FILETYPE buffer1, buffer2;
	bool r1 = false;
	bool r2 = false;
	bool readNext1 = true;
	bool readNext2 = true;

	if (input1 && input2 && output)
	{
		while (!input1.eof() || !input2.eof())
		{
			if (readNext1) r1 = (bool)(input1.read(reinterpret_cast<char*>(&buffer1), sizeof(FILETYPE)));
			if (readNext2) r2 = (bool)(input2.read(reinterpret_cast<char*>(&buffer2), sizeof(FILETYPE)));

			if (r1 || r2)
			{
				if (r1 && r2)
				{
					if (buffer1 == buffer2)
					{
						output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
						readNext1 = true;
						output.write(reinterpret_cast<const char*>(&buffer2), sizeof(FILETYPE));
						readNext2 = true;
					}
					else
					if (buffer1 < buffer2)
					{
						output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
						readNext1 = true;
						readNext2 = false;
					}
					else
					if (buffer1 > buffer2)
					{
						output.write(reinterpret_cast<const char*>(&buffer2), sizeof(FILETYPE));
						readNext1 = false;
						readNext2 = true;
					}
				} else
				if (r1 && !r2)
				{
					output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
					readNext1 = true;
					readNext2 = false;
				} else
				if (!r1 && r2)
				{
					output.write(reinterpret_cast<const char*>(&buffer2), sizeof(FILETYPE));
					readNext1 = false;
					readNext2 = true;
				}
			}
			else break;
		}
		input1.close();
		input2.close();
		output.close();
	}
}