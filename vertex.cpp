#include "vertex.h"
#include <stdlib.h>
#include <string.h>
//#include <windows.h>
#include <stdio.h>

TextRGB8::TextRGB8(const char* str) {
    compile(str, strlen(str));
}
TextRGB8::TextRGB8(const char* str, const ColorRGB8& color) {
   copy(str, color);
}
TextRGB8::~TextRGB8(){
    destroy();
}
void TextRGB8::destroy(){
    if (m_string){
        free(m_string);
        m_string = nullptr;
        m_color = nullptr;
        m_capacity = 0;
        m_size = 0;
    }
}

void TextRGB8::setColor(const ColorRGB8& color) {
    for (size_t i = 0; i < size(); i++) {
        m_color[i] = color;
    }
}
void TextRGB8::resize(unsigned int size){
    if (size > m_capacity) {
       printf("resizing %s\n", m_string);
       m_capacity = size;
       void* lastString = (void*)m_string;
       void* lastColor = (void*)m_color;
       void* buffer = malloc((sizeof(char) + sizeof(ColorRGB8)) * m_capacity + sizeof(char));
       m_string = (char*)buffer;
       m_color = (ColorRGB8*)((intptr_t)(buffer) + sizeof(char) * m_capacity + sizeof(char));
       if (lastString) {
          memcpy(m_string, lastString, (m_size + 1) * sizeof(char));
          memcpy(m_color, lastColor, m_size * sizeof(ColorRGB8));
          free(lastString);
       } else {
          m_string[0] = '\0';
       }
    }
    m_size = size;
}
void TextRGB8::clear(){
    m_size = 0;
    m_string[m_size] = '\0';
}

void TextRGB8::compile(const char* code, unsigned int codeSize) {
    // exemple #|2156FFlol hi|#
    unsigned int size = 0;
    ColorRGB8 currentColor(255, 255, 255);
    for (unsigned int i = 0; code[i] != 0;) {
        if (code[i] == '#' && code[i + 1] == '|' && i + 7 < codeSize) {
            i += 8;
        }
        else if (code[i] == '|' && code[i + 1] == '#') {
            i += 2;
        }
        else {
            size++;
            i++;
        }
    }
    resize(size);
    for (unsigned int i = 0, j = 0; i < codeSize;) {
        if (code[i] == '#' && code[i + 1] == '|' && i + 7 < codeSize) {
            unsigned int r = (code[i + 2] - (('a' - 10) * (code[i + 2] >= 'a')) - (('0') * ((code[i + 2] >= '0') && (code[i + 2] <= '9')))) * 16 + (code[i + 3] - (('a' - 10) * (code[i + 3] >= 'a')) - (('0') * ((code[i + 3] >= '0') && (code[i + 3] <= '9'))));
            unsigned int g = (code[i + 4] - (('a' - 10) * (code[i + 4] >= 'a')) - (('0') * ((code[i + 4] >= '0') && (code[i + 4] <= '9')))) * 16 + (code[i + 5] - (('a' - 10) * (code[i + 5] >= 'a')) - (('0') * ((code[i + 5] >= '0') && (code[i + 5] <= '9'))));
            unsigned int b = (code[i + 6] - (('a' - 10) * (code[i + 6] >= 'a')) - (('0') * ((code[i + 6] >= '0') && (code[i + 6] <= '9')))) * 16 + (code[i + 7] - (('a' - 10) * (code[i + 7] >= 'a')) - (('0') * ((code[i + 7] >= '0') && (code[i + 7] <= '9'))));
            currentColor.r = r;
            currentColor.g = g;
            currentColor.b = b;
            i += 8;
        }
        else if (code[i] == '|' && code[i + 1] == '#') {
            currentColor.r = 255;
            currentColor.g = 255;
            currentColor.b = 255;
            i += 2;
        }
        else {
            m_string[j] = code[i];
            m_color[j++] = currentColor;
            i++;
        }
    }
    m_string[m_size] = '\0';
}
void TextRGB8::append(const TextRGB8& text){
    size_t oldSize = m_size;
    resize(m_size + text.m_size);
    memcpy(m_string + oldSize, text.m_string, (text.m_size + 1) * sizeof(char));
    for(size_t i = 0 ; i < text.m_size; i++){
        m_color[oldSize + i] = text.m_color[i];
    }
}

void TextRGB8::append(const char* text){
    size_t oldSize = m_size;
    size_t textSize = strlen(text);
    resize(m_size + textSize);
    memcpy(m_string + oldSize, text, (textSize + 1) * sizeof(char));
    memset(m_color + oldSize, 255, textSize * sizeof(ColorRGB8));
}
void TextRGB8::popBack(){
    for(size_t i = 1; i < m_size; i++){
        m_string[i - 1] = m_string[i];
        m_color[i - 1] = m_color[i];
    }
    m_size--;
    m_string[m_size] = '\0';
}
void TextRGB8::popFront(){
    m_size--;
    m_string[m_size] = '\0';
}
void TextRGB8::copy(const char* other){
    size_t otherSize = strlen(other);
    if (otherSize > m_capacity){
        m_capacity = otherSize;
        m_size = otherSize;
        free(m_string);
        void* buffer = malloc((sizeof(char) + sizeof(ColorRGB8)) * m_capacity + sizeof(char));
        m_string = (char*)buffer;
        m_color = (ColorRGB8*)((intptr_t)(buffer) + sizeof(char) * m_capacity + sizeof(char));
        memcpy(m_string, other, sizeof(char) * (m_size + 1));
        memset(m_color, 255, m_size * sizeof(ColorRGB8));
    } else {
        m_size = otherSize;
        memcpy(m_string, other, sizeof(char) * (m_size + 1));
        memset(m_color, 255, m_size * sizeof(ColorRGB8));
    }
}
void TextRGB8::copy(const char* other, const ColorRGB8& color){
    size_t otherSize = strlen(other);
    if (otherSize > m_capacity){
        m_capacity = otherSize;
        m_size = otherSize;
        free(m_string);
        void* buffer = malloc((sizeof(char) + sizeof(ColorRGB8)) * m_capacity + sizeof(char));
        m_string = (char*)buffer;
        m_color = (ColorRGB8*)((intptr_t)(buffer) + sizeof(char) * m_capacity + sizeof(char));
        memcpy(m_string, other, sizeof(char) * (m_size + 1));
        for(size_t i = 0; i < m_size; i++){
            m_color[i] = color;
        }
        memset(m_color, 255, m_size * sizeof(ColorRGB8));
    } else {
        m_size = otherSize;
        memcpy(m_string, other, sizeof(char) * (m_size + 1));
        for(size_t i = 0; i < m_size; i++){
            m_color[i] = color;
        }
    }
}

void TextRGB8::copy(const TextRGB8& other){
    destroy();
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    void* buffer = malloc((sizeof(char) + 1) * m_capacity + sizeof(ColorRGB8) * m_capacity);
    memcpy(buffer, (void*)other.m_string, (sizeof(char) + 1) * m_capacity + sizeof(ColorRGB8) * m_capacity);
    m_string = (char*)buffer;
    m_color = (ColorRGB8*)((intptr_t)(buffer) + sizeof(char) * m_capacity + sizeof(char));
}

