#include <fstream>
#include <iostream>
#include <string>

using namespace std;

size_t fileSize(ifstream& file)
{
    streampos fpos = file.tellg();
    file.seekg(0, ios_base::end);
    size_t n = file.tellg();
    file.seekg(fpos);
    return n;
}

bool fileExist(string filename)
{
    ifstream testFile(filename);
    bool exist = !testFile.fail();
    testFile.close();
    return exist;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "Too few arguments.\n";
        return 0;
    }
    else if (argc > 4)
    {
        cout << "Too many arguments. Some of them will be discarded.\n";
    }

    ifstream sourceFile(argv[1], ios::binary);
    if (sourceFile.fail())
    {
        cout << "Could not open the source file.\n";
        return 0;
    }

    size_t length = fileSize(sourceFile);
    size_t amount = 0;
    size_t eachFileSize;
    size_t n = 0;
    string mode(argv[2]);
    size_t param;

    // 尝试转为 size_t ，失败则退出
    try
    {
        param = stoull(argv[3]);
    }
    catch (invalid_argument&)
    {
        cout << "Invalid syntax.\n";
        return 0;
    }
    
    // 选择模式，不支持则退出
    if (mode == "-a")
    {
        amount = param;
        eachFileSize = length / amount;
    }
    else if (mode == "-b")
    {
        eachFileSize = param;
    }
    else if (mode == "-k")
    {
        eachFileSize = param * 1024;
    }
    else if (mode == "-m")
    {
        eachFileSize = param * 1024 * 1024;
    }
    else
    {
        cout << "Mode must be -a, -b, -k or -m.\n";
        return 0;
    }

    // 指定大小的模式
    if(!amount)
    {
        amount = length / eachFileSize + 1;
    }
    // 指定总数的模式，处理除法产生的“余数”（剩余的字节）
    else {
        n = length - amount * eachFileSize;
    }

    // 总数不能为 0 ，也不能大于文件大小
    if (amount > length)
    {
        cout << "Amount too big.\n";
        return 0;
    }
    if (amount <= 0)
    {
        cout << "Amount must be an integer and bigger than 0.\n";
    }

    // for 循环，依次创建每个新文件
    for (size_t i = 1; i <= amount; i++)
    {
        string newFilename(argv[1]);
        newFilename += "." + to_string(i);

        // 如果同名文件已存在，则询问是否覆盖
        if (fileExist(newFilename))
        {
            cout << "This program will overwrite " << newFilename
                << ", do you want to continue?(Y/N)  ";
            char choice;
            cin >> choice;
            if (choice == 'N' || choice == 'n')
                return 0;
        }

        cout << "Generating " + newFilename + '\n';

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
        char* content = NULL;
        content = new char[sizeToRead];
        ofstream newFile(newFilename, ios::binary);
        sourceFile.read(content, sizeToRead);
        newFile.write(content, sizeToRead);
        newFile.close();
        delete[] content;
    }


    // 关闭源文件
    sourceFile.close();
    return 0;
}