// FileSortApp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <chrono>
#include "FileSort.h"

class SimpleTimer {
public:
	SimpleTimer() {
		start = std::chrono::high_resolution_clock::now();
	}

	~SimpleTimer() {
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;
		cout << "Estimated time: " << duration.count() << "\n";
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
};


std::string SplitFilename(const std::string& str)
{
	size_t found;
	found = str.find_last_of("/\\");
	return str.substr(0, found + 1);
}

int main(int argc, char* argv[])
{
	SimpleTimer simpleTimer;
	FileSort fileSort;
	std::string input = "input";
	std::string output = "output";
	if (argc >= 3)
	{
		input = std::string(argv[1]);
		output = std::string(argv[2]);
	}

	fileSort.startSort(SplitFilename(std::string(argv[0])), input, output);

    std::cout << "Operation complete!\n";
	simpleTimer.~SimpleTimer();
	std::cout << "Press any key to continue . . .\n";
	std::cin.get();
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
