#ifndef IOMANAGER_H
#define IOMANAGER_H
#include "stdio.h"
#include "common.h"

class IOManager
{
public:
    static void stdError(const char* error);
    static void fatalError(const char* error);
    static Buffer readFile(const char* filePath);
    static Buffer readFile(FILE* file);
    static Buffer readFile(FILE* file, long nbRead);
    static const char* readFileToString(const char* filePath);
    static const char* readFileToString(FILE* file);
    static const char* readFileToString(FILE* file, long nbRead);
    static void writeToFile(const char* filePath, const Buffer& buffer);
    static void writeToFile(FILE* file, const Buffer& buffer);
    static void writeToFile(FILE* file, const Buffer& buffer, long offset);
    static void printBuffer(const Buffer& buffer);
};

#endif // IOMANAGER_H
