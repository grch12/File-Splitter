#include <fstream>
#include <iostream>
#include <string>
#include <memory>

#include "clipp.h"

using std::string;
using namespace clipp;

size_t fileSize(std::ifstream& file)
{
	std::streampos fpos = file.tellg();
	file.seekg(0, std::ios_base::end);
	size_t n = file.tellg();
	file.seekg(fpos);
	return n;
}

bool fileExist(const string& filename)
{
	std::ifstream testFile(filename);
	bool exist = !testFile.fail();
	testFile.close();
	return exist;
}

int main(int argc, char* argv[])
{
	string sourceFileName;
	size_t amount = 0;
	size_t eachFileSize;
	size_t n = 0;
	size_t size;

	size_t unit = 0;

	// 处理参数
	auto amountCli = (command("-a"),
		value("amount", amount));

	auto sizeCli = ((option("-b", "--byte").set(unit, 1llu).doc("use byte as the unit") |
		option("-k", "--kib").set(unit, 1024llu).doc("use kilobyte as the unit") |
		option("-m", "--mib").set(unit, 1048576llu).doc("use megabyte as the unit")),
		value("size", size));

	auto cli = (value("input file", sourceFileName),
		(amountCli | sizeCli));

	if (!parse(argc, argv, cli))
	{
		std::cout << make_man_page(cli, argv[0]);
		return 0;
	}

	std::ifstream sourceFile(sourceFileName);
	if (sourceFile.fail())
	{
		std::cout << "Could not open the source file.\n";
		return 0;
	}
	size_t length = fileSize(sourceFile);

	if (sourceFile.fail())
	{
		std::cout << "Could not open the source file.\n";
		return 0;
	}

	// 指定大小的模式
	if (!amount)
	{
		eachFileSize = size * unit;
		amount = length / eachFileSize + 1;
	}
	// 指定总数的模式，处理除法产生的“余数”（剩余的字节）
	else
	{
		// 总数不能为 0 ，也不能大于文件大小
		if (amount > length)
		{
			std::cout << "Amount too big.\n";
			return 0;
		}
		if (amount <= 0)
		{
			std::cout << "Amount must be an integer and bigger than 0.\n";
			return 0;
		}
		eachFileSize = length / amount;
		n = length - amount * eachFileSize;
	}

	// for 循环，依次创建每个新文件
	for (size_t i = 1; i <= amount; i++)
	{
		string newFilename(argv[1]);
		newFilename += "." + std::to_string(i);

		// 如果同名文件已存在，则询问是否覆盖
		if (fileExist(newFilename))
		{
			std::cout << "This program will overwrite " << newFilename
				<< ", do you want to continue?(Y/N)  ";
			char choice;
			std::cin >> choice;
			if (choice == 'N' || choice == 'n')
				return 0;
		}

		std::cout << "Generating " + newFilename + '\n';

		size_t sizeToRead = eachFileSize;
		if (n > 0)
		{
			sizeToRead++;
			n--;
		}
		if (sizeToRead * i > length)
		{
			sizeToRead = length - eachFileSize * (i - 1);
		}
		std::unique_ptr<char[]> content(new char[sizeToRead]);
		std::ofstream newFile(newFilename, std::ios::binary);
		sourceFile.read(content.get(), sizeToRead);
		newFile.write(content.get(), sizeToRead);
		newFile.close();
	}

	// 关闭源文件
	sourceFile.close();
	return 0;
}