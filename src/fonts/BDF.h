#ifndef SHANGHAI_BDF_H
#define SHANGHAI_BDF_H
#include <string>
#include <unordered_map>
#include <vector>

struct BDFChar {
    char character; // Character being represented
    int width, height; // Width and height of the character
    int xAdvance; // How much to advance the cursor after drawing this character
    std::vector<bool> bitmap; // Bitmap data for the character
};

class BDF {
public:
    explicit BDF(const std::string& path);
    ~BDF();

    [[nodiscard]] int getCharCount() const { return charCount; } // Get the number of characters in the font
    [[nodiscard]] BDFChar* getChars() const { return chars; } // Get the array of characters
private:
    BDFChar *chars = nullptr; // Array of characters
    int charCount = 0; // Number of characters in the font

    void decodeFile(const std::string& path);
};

#endif //SHANGHAI_BDF_H
