#ifndef HASH_H
#define HASH_H

#include <unordered_map>

class Key {
public:
    Key() : m_size(0) {}
    Key(const std::string &path, int size) : m_path(path), m_size(size) {}
    const std::string &getPath() const { return m_path; }
    int getSize() const { return m_size; }

    bool operator==(const Key &other) const {
        return other.m_path == m_path && other.m_size == m_size;
    }
private:
    std::string m_path;
    int m_size;
};
namespace std
{
    template <>
    struct hash <Key>
    {
        size_t operator()(const Key& k) const
        {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash <string>()(k.getPath());
            res = res * 31 + hash <int>() (k.getSize());
            return res;
        }
    };
}


#endif // HASH_H

