#ifndef VERTEX
#define VERTEX

#include <vector>

struct Position {
    float x;
    float y;
};
struct ColorRGB8 {
    ColorRGB8() : r(0), g(0), b(0) { }
    ColorRGB8 operator+(ColorRGB8 color) {
        ColorRGB8 newColor(r + color.r, g + color.g, b + color.b);
        return newColor;
    }
    ColorRGB8(unsigned int R, unsigned int G, unsigned int B) :
        r(R), g(G), b(B) { }
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct ColorRGBA8 {
    ColorRGBA8() : r(0), g(0), b(0), a(0) { }
    ColorRGBA8 operator+(ColorRGBA8 color) {
        ColorRGBA8 newColor(r + color.r, g + color.g, b + color.b, a + color.a);
        return newColor;
    }
    ColorRGBA8(unsigned int R, unsigned int G, unsigned int B, unsigned int A) :
        r(R), g(G), b(B), a(A) { }
    ColorRGBA8(const ColorRGB8& c, unsigned int A) : r(c.r), g(c.g), b(c.b), a(A) { }
    ColorRGBA8(const ColorRGB8& c) : r(c.r), g(c.g), b(c.b), a(255) { }
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};
#define BlackRGBA8 ColorRGBA8(0, 0, 0, 255)
#define WhiteRGBA8 ColorRGBA8(255, 255, 255, 255)
#define GreenRGBA8 ColorRGBA8(0, 255, 0, 255)
#define RedRGBA8 ColorRGBA8(255, 0, 0, 255)
#define BlueRGBA8 ColorRGBA8(0, 0, 255, 255)
#define YellowRGBA8 ColorRGBA8(255, 255, 0, 255)
#define OrangeRGBA8 ColorRGBA8(255, 165, 0, 255)
#define GrayRGBA8 ColorRGBA8(128, 128, 128, 255)


#define BlackRGB8 ColorRGB8(0, 0, 0)
#define WhiteRGB8 ColorRGB8(255, 255, 255)
#define GreenRGB8 ColorRGB8(0, 255, 0)
#define RedRGB8 ColorRGB8(255, 0, 0)
#define BlueRGB8 ColorRGB8(0, 0, 255)
#define YellowRGB8 ColorRGB8(255, 255, 0)
#define OrangeRGB8 ColorRGB8(255, 165, 0)
#define GrayRGB8 ColorRGB8(128, 128, 128)



struct UV {
    float u;
    float v;
};

struct Vertex {
    float x, y;
    unsigned char r, g, b, a;
    float u, v;

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    void setColor(const ColorRGBA8 &color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }

    void setUV(float u, float v) {
        this->u = u;
        this->v = v;
    }
};

class TextRGB8 {
public:
    TextRGB8() {}
    TextRGB8(const char* str);
    TextRGB8(const char* str, const ColorRGB8& color);

    ~TextRGB8();

    void setColor(const ColorRGB8& color);
    void resize(unsigned int size);
    void clear();
    void compile(const char* code, unsigned int codeSize);
    void append(const TextRGB8& text);
    void append(const char* text);
    void popBack();
    void popFront();
    void copy(const char* other);
    void copy(const char* other, const ColorRGB8& color);
    void copy(const TextRGB8& other);

    TextRGB8& operator=(const char* other){
        this->copy(other);
        return *this;
    }
    TextRGB8& operator=(const TextRGB8& other){
        this->copy(other);
        return *this;
    }

    inline unsigned int size() const { return m_size ;}
    inline unsigned int capacity() const {return m_capacity;}
    inline const char* toCString() const { return m_string ;}
    inline const ColorRGB8* getColors() const {return m_color ;}
private:
    void destroy();
private:
    char* m_string = nullptr;
    ColorRGB8* m_color = nullptr;
    unsigned int m_capacity = 0;
    unsigned int m_size = 0;
};

/// For text justification
enum class Justification {
    LEFT, MIDDLE, RIGHT
};

#endif // VERTEX

