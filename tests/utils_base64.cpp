#include <iostream>
#include <string>
#include <vector>
#include <cassert>

static std::string EncodeB64(const std::string& in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4)
        out.push_back('=');
    return out;
}

static std::string DecodeB64(const std::string& in) {
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) {
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    }
    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

int main() {
    std::vector<std::string> samples = {
        "",
        "hello",
        "The quick brown fox jumps over the lazy dog",
        "1234567890",
        "!@#$%^&*()_+-={}|[]\\:\";'<>?,./"
    };

    for (const auto& sample : samples) {
        std::string encoded = EncodeB64(sample);
        std::string decoded = DecodeB64(encoded);
        if (decoded != sample) {
            std::cerr << "Mismatch for sample: " << sample << "\n";
            return 1;
        }
    }
    std::cout << "All tests passed\n";
    return 0;
}
