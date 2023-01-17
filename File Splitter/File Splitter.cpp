#include <iostream>
#include <fstream>
#include <string>

using namespace std;

size_t fileSize(ifstream &file)
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
    else if (argc > 3)
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
    int amount;
    try
    {
        amount = stoi((string)argv[2]);
    }
    catch (invalid_argument)
    {
        amount = 0;
    }
    if (amount > length)
    {
        cout << "Amount too big.\n";
        return 0;
    }
    if (amount <= 0)
    {
        cout << "Amount must be an integer and bigger than zero.\n";
        return 0;
    }
    size_t eachFileSize = length / amount;
    size_t n = length - eachFileSize * amount;

    for (int i = 0; i < amount; i++)
    {
        string newFilename;
        newFilename += argv[1];
        newFilename += "." + to_string(i + 1);

        cout << "Generating " + newFilename + '\n';

        size_t sizeToRead = eachFileSize;
        if (n > 0)
        {
            sizeToRead++;
            n--;
        }
        char* content = NULL;
        content = new char[sizeToRead];
        ofstream newFile(newFilename, ios::binary);
        sourceFile.read(content, sizeToRead);
        newFile.write(content, sizeToRead);
        newFile.close();
        delete[] content;
    }
}