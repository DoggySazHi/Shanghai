#include "BDF.h"

#include <fstream>
#include <iostream>

BDF::BDF(const std::string &path) {
    decodeFile(path);

    if (chars == nullptr) {
        std::cerr << "Failed to load font\n";
        return;
    }
}

BDF::~BDF() {
    delete[] chars;
}

void BDF::decodeFile(const std::string &path) {
     std::ifstream font(path);

    if (font.fail()) {
        std::cerr << "Failed to open font file " << path << '\n';
        return;
    }

    bool finishedReadingHeader = false;
    bool readingBitmap = false;
    int charIndex = 0;
    int bitmapIndex = 0;

    std::string line;
    while (!font.eof()) {
        std::getline(font, line);

        if (line.empty()) {
            finishedReadingHeader = true;
            continue;
        }

        if (line.starts_with("CHARS ")) {
            charCount = std::stoi(line.substr(6));
            chars = new BDFChar[charCount];
            continue;
        }

        if (!finishedReadingHeader) {
            continue;
        }

        if (line.starts_with("ENCODING ")) {
            if (chars == nullptr) {
                std::cerr << "Font file " << path << " is malformed, no CHARS line found\n";
                return;
            }

            chars[charIndex].character = (char) std::stoi(line.substr(9));
            continue;
        }

        if (line.starts_with("BBX ")) {
            if (chars == nullptr) {
                std::cerr << "Font file " << path << " is malformed, no CHARS line found\n";
                return;
            }

            chars[charIndex].width = std::stoi(line.substr(4));
            chars[charIndex].height = std::stoi(line.substr(4 + (chars[charIndex].width / 10 + 1) + 1));

            continue;
        }

        if (line.starts_with("DWIDTH ")) {
            if (chars == nullptr) {
                std::cerr << "Font file " << path << " is malformed, no CHARS line found\n";
                return;
            }

            chars[charIndex].width = std::stoi(line.substr(8));
            continue;
        }

        if (line.starts_with("BITMAP")) {
            readingBitmap = true;
            bitmapIndex = charIndex;
            continue;
        }

        if (line.starts_with("ENDCHAR")) {
            ++charIndex;
            readingBitmap = false;
            continue;
        }

        if (!readingBitmap) {
            continue;
        }

        if (chars == nullptr) {
            std::cerr << "Font file " << path << " is malformed, no CHARS line found\n";
            return;
        }

        // row into bits
        int bitsCounted = 0;
        for (char c : line) {
            if (c == ' ') {
                continue;
            }

            // hex to binary
            int bit = (c >= '0' && c <= '9') ? c - '0' : c - 'A' + 10;

            for (int i = 0; i < 4; ++i) {
                if (bitsCounted >= chars[bitmapIndex].width) {
                    break;
                }

                chars[bitmapIndex].bitmap.push_back((bit >> (3 - i)) & 1);
                ++bitsCounted;
            }
        }
    }

    font.close();
}
