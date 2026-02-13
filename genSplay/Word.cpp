#include <iostream>
#include "genSplay.hpp"
#include <fstream>
#include <cstring>
#include <cstdlib> //eixt()
using namespace std;

class Word
{
public:
    Word() : word(nullptr), freq(1) {}
    bool operator==(const Word &other)
    {
        return strcmp(word, other.word) == 0;
    }
    bool operator>(const Word &other)
    {
        return strcmp(word, other.word) > 0;
    }

private:
    char *word;
    int freq;
    friend class WordSplay;
};

class WordSplay : public SplayTree<Word>
{
public:
    WordSplay() : differentWords(0), wordcnt(0) {}
    void run(ifstream &fIn, char *fileName); // 运行程序
private:
    int differentWords;            // 统计单词个数
    int wordcnt;                   // 统计总单词数
    void visit(SplayNode<Word> *); // 重写visit函数
};

void WordSplay::run(ifstream &fIn, char *fileName)
{
    char s[100];
    char ch = ' ', i;
    Word rec;
    while (!fIn.eof())
    {
        while (true)
        {
            if (!fIn.eof() && !isalpha(ch))
            {
                fIn.get(ch);
            }
            else
            {
                break;
            }
        }
        if (fIn.eof())
        {
            break;
        }
        for (i = 0; !fIn.eof() && isalpha(ch); i++)
        {
            s[i] = toupper(ch);
            fIn.get(ch);
        }
        s[i] = '\0';
        if (!(rec.word = new char[strlen(s) + 1]))
        {
            delete[] rec.word;
            cerr << "NO Room for new Word\n";
            exit(1);
        }
        strcpy_s(rec.word, strlen(s) + 1, s);
        SplayNode<Word> *p = this->search(rec);
        if (p == nullptr)
        {
            this->insert(rec);
        }
        else
            p->value().freq++;
    }
    this->inorder();
    cout << "\n\nFile:" << fileName
         << "\ncontains:" << wordcnt << " word among which "
         << "differentWords" << " are " << differentWords;
}

void WordSplay::visit(SplayNode<Word> *node)
{
    this->differentWords++;
    this->wordcnt += node->value().freq;
}

int main(int argc, char *argv[])
{
    char fileName[80];
    WordSplay splayTree;
    if (argc != 2)
    {
        cout << "Enter the fileName" << endl;
        cin >> fileName;
    }
    else
        strncpy(fileName, argv[1], 80);
    ifstream fIn(fileName);
    if (fIn.fail())
    {
        cerr << "Cannot open " << fileName << endl;
        return 0;
    }
    splayTree.run(fIn, fileName);
    fIn.close();
    return 0;
}
