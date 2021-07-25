#include "iomanager.h"
#include "stdlib.h"

void IOManager::stdError(const char* error){
    fprintf(stderr, "%s\n", error);
}
void IOManager::fatalError(const char* error){
    fprintf(stderr, "%s\n", error);
    getchar();
    exit(-1);
}
Buffer IOManager::readFile(const char* filePath){
    FILE* file = nullptr;
    file = fopen(filePath, "rb");
    if (!file){
        stdError(filePath);
        stdError(" not found !\n");
        return {};
    }
    return readFile(file);
}

Buffer IOManager::readFile(FILE* file){
    Buffer buffer = {};
    fseek(file, 0l, SEEK_END);
    buffer.size = ftell(file);
    buffer.data = malloc(buffer.size);
    if (!buffer.data) return buffer;
    fseek(file, 0l, SEEK_SET);
    fread(buffer.data, 1, buffer.size, file);
    return buffer;
}

Buffer IOManager::readFile(FILE* file, long nbRead){
    Buffer buffer = {};
    buffer.size = nbRead;
    buffer.data = malloc(buffer.size);
    if (!buffer.data) return buffer;
    fread(buffer.data, 1, buffer.size, file);
    return buffer;
}
const char* IOManager::readFileToString(const char* filePath){
    FILE* file = nullptr;
    file = fopen(filePath, "rb");
    if (!file){
        fprintf(stderr, "%s not found !\n", filePath);
        return {};
    }
    return readFileToString(file);
}

const char* IOManager::readFileToString(FILE* file){
    char* buffer = nullptr;
    fseek(file, 0l, SEEK_END);
    long size = ftell(file);
    buffer = (char*)malloc(size + 1);
    if (!buffer) return buffer;
    fseek(file, 0l, SEEK_SET);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    return buffer;
}

const char* IOManager::readFileToString(FILE* file, long nbRead){
    char* buffer = nullptr;
    long size = nbRead;
    buffer = (char*)malloc(size + 1);
    if (!buffer) return buffer;
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    return buffer;
}

void IOManager::writeToFile(const char* filePath, const Buffer& buffer){
    FILE* file = fopen(filePath, "wb+");
    if (!file) return;
    writeToFile(file, buffer);
}
void IOManager::writeToFile(FILE* file, const Buffer& buffer){
    fwrite(buffer.data, 1, buffer.size, file);
}

void IOManager::writeToFile(FILE* file, const Buffer& buffer, long offset){
    fseek(file, offset, SEEK_CUR);
    fwrite(buffer.data, 1, buffer.size, file);
}
void IOManager::printBuffer(const Buffer& buffer){
   for(size_t i = 0; i < buffer.size; i++){
       printf("%c", ((char*)buffer.data)[i]);
   }
   printf("\n");
}
