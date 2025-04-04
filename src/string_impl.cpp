#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>

class String {
private:
    char* data;
    size_t length;

public:
    String() : data(new char[1]{'\0'}), length(0) {}
    
    String(const char* str) : length(strlen(str)) {
        data = new char[length + 1];
#ifdef __GNUC__
        strcpy(data, str);
#else
        strcpy_s(data, length + 1, str);
#endif
    }
    
    String(const String& other) : length(other.length) {
        data = new char[length + 1];
#ifdef __GNUC__
        strcpy(data, other.data);
#else
        strcpy_s(data, length + 1, other.data);
#endif
        
    }
    
    String(String&& other) noexcept : data(other.data), length(other.length) {
        other.data = nullptr;
        other.length = 0;
    }
    
    ~String() {
        delete[] data;
    }
    
    String& operator=(const String& other) {
        if (this == &other) return *this;
        
        delete[] data;
        length = other.length;
        data = new char[length + 1];

#ifdef __GNUC__
        strcpy(data, other.data);
#else
        strcpy_s(data, length + 1, other.data);
#endif
        
        
        return *this;
    }
    
    String& operator=(String&& other) noexcept {
        if (this == &other) return *this;
        
        delete[] data;
        data = other.data;
        length = other.length;
        
        other.data = nullptr;
        other.length = 0;
        
        return *this;
    }
    
    String& operator+=(const String& other) {
        size_t new_length = length + other.length;
        char* new_data = new char[new_length + 1];

#ifdef __GNUC__
        strcpy(new_data, data);
        strcpy(new_data + length, other.data);
#else
        strcpy_s(new_data, length + 1, data);
        strcpy_s(new_data + length, other.length + 1, other.data);
#endif

        
        delete[] data;
        data = new_data;
        length = new_length;
        
        return *this;
    }

    String& operator+=(const char* str) {
        size_t str_length = strlen(str);
        size_t new_length = length + str_length;
        char* new_data = new char[new_length + 1];

#ifdef __GNUC__
        strcpy(new_data, data);
        strcpy(new_data + length, str);
#else
        strcpy_s(new_data, length + 1, data);
        strcpy_s(new_data + length, str_length + 1, str);
#endif

        
        delete[] data;
        data = new_data;
        length = new_length;
        
        return *this;
    }
    
    friend String operator+(const String& lhs, const String& rhs) {
        String temp(lhs);
        temp += rhs;
        return temp;
    }

    friend String operator+(const String& lhs, const char* rhs) {
        String temp(lhs);
        temp += rhs;
        return temp;
    }

    friend String operator+(const char* lhs, const String& rhs) {
        String temp(lhs);
        temp += rhs;
        return temp;
    }
    
    bool operator<(const String& other) const {
        return stricmp(data, other.data) < 0;
    }
    bool operator>(const String& other) const {
        return stricmp(data, other.data) > 0;
    }

    static int stricmp (const char *p1, const char *p2)
    {
        unsigned const char *s1 = reinterpret_cast<unsigned const char*>(p1);
        unsigned const char *s2 = reinterpret_cast<unsigned const char*>(p2);
        unsigned char c1, c2;
 
        do
        {
            c1 = static_cast<unsigned char>(toupper(*s1++));
            c2 = static_cast<unsigned char>(toupper(*s2++));
            
            if (c1 == '\0')
            {
                return c1 - c2;
            }
        }
        while (c1 == c2);
 
        return c1 - c2;
    }
    
    void print() const {
        std::cout << data << '\n';
    }
};

void example() {
    std::cout << "\n--- String Class Example ---\n";

    String s1("Hello");
    String s2("World");
    String s3 = s1 + ", " + s2 + "!";

    std::cout << "Concatenated: ";
    s3.print();

    String s4 = s3; // copy constructor
    std::cout << "Copied: ";
    s4.print();

    String s5;
    s5 = std::move(s4); // move assignment
    std::cout << "Moved: ";
    s5.print();
    
    s1 += "!!!";
    std::cout << "Appended s1: ";
    s1.print();

    std::vector<String> vec = {String("banana"), String("apple"), String("orange")};
    std::sort(vec.begin(), vec.end());
    std::cout << "Sorted: ";
    for (const auto& s : vec) s.print();
}

static int printWords(const char* fileName)
{
    std::vector<String> strings;
    std::ifstream file(fileName);

    if (!file) {
        std::cerr << "Error: Cannot open file " << fileName << '\n';
        return 1;
    }

    char buffer[1024]; // max word size
    while (file.getline(buffer, sizeof(buffer))) {
        strings.emplace_back(buffer);
    }

    file.close();

    std::sort(strings.begin(), strings.end(), std::greater<String>());

    std::cout << "\n--- Words Reversed Order ---\n";
    for (const auto& str : strings) {
        str.print();
    }

    return  0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << '\n';
        return 1;
    }

    int exitCode = printWords(argv[1]);
    if (exitCode != 0)
    {
        return exitCode;
    }

    example();
    
    return 0;
}
