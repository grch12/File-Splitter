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

int main(int argc, char* argv[])
{
    if (argc < 3)
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
    size_t amount;
    size_t eachFileSize = 0;
    size_t n = 0;
    try
    {
        amount = stoi((string)argv[2]);
        if (amount <= 0)
        {
            cout << "Amount must be an integer and bigger than zero.\n";
            return 0;
        }
        eachFileSize = length / amount;
        n = length - eachFileSize * amount;
    }
    catch (invalid_argument&)
    {
        amount = 0;
        string mode(argv[2]);
        if (mode == "-b")
        {
            if (argc < 4)
            {
                cout << "Too few arguments.\n";
                return 0;
            }
            eachFileSize = stoull((string)argv[3]);
        }
        else if (mode == "-k")
        {
            if (argc < 4)
            {
                cout << "Too few arguments.\n";
                return 0;
            }
            eachFileSize = stoull((string)argv[3]) * 1024;
        }
        else if (mode == "-m")
        {
            if (argc < 4)
            {
                cout << "Too few arguments.\n";
                return 0;
            }
            eachFileSize = stoull((string)argv[3]) * 1024 * 1024;
        }
        else
        {
            cout << "Mode must be -b, -k or -m.\n";
            return 0;
        }
        amount = length / eachFileSize + 1;
    }
    if (amount > length)
    {
        cout << "Amount too big.\n";
        return 0;
    }

    for (size_t i = 1; i <= amount; i++)
    {
        string newFilename;
        newFilename += argv[1];
        newFilename += "." + to_string(i);

        ifstream testFile(newFilename);
        if (!testFile.fail())
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

    sourceFile.close();
    return 0;
}