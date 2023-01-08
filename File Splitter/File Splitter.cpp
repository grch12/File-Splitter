#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

size_t fileSize(ifstream* file)
{
    streampos fpos = file->tellg();
    file->seekg(0, ios_base::end);
    size_t n = file->tellg();
    file->seekg(fpos);
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
        cout << "Too many arguments. Some of them will be discard.\n";
    }

    ifstream sourceFile(argv[1], ios::binary);
    if (sourceFile.fail())
    {
        cout << "Open file failed.\n";
        return 0;
    }

    size_t length = fileSize(&sourceFile);
    int amount = atoi(argv[2]);
    if (amount > length)
    {
        cout << "Amount too big.\n";
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
        content = (char*)malloc(sizeToRead);
        ofstream newFile(newFilename, ios::binary);
        sourceFile.read(content, sizeToRead);
        newFile.write(content, sizeToRead);
        free(content);
    }
}