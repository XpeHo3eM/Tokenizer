#include <iostream>
#include <vector>
#include <variant>
#include <unordered_map>



struct OpeningBracketToken {};

struct ClosingBracketToken {};

struct NumberToken {
  int value;
};

struct UnknownToken {
  std::string value;
};

struct MinToken {};

struct AbsToken {};

struct PlusToken {};

struct MinusToken {};

struct MultiplyToken {};

struct ModuloToken {};

struct DivideToken {};

struct MaxToken {};

struct SqrToken {};

struct CommaToken {};

using Token = std::variant<OpeningBracketToken, ClosingBracketToken, CommaToken, MinToken, MaxToken, AbsToken, SqrToken,
    PlusToken, MinusToken, MultiplyToken, ModuloToken, DivideToken, NumberToken, UnknownToken>;


const std::unordered_map<char, Token> symbol2Token {
    {'+', PlusToken{}},
    {'-', MinusToken{}},
    {'*', MultiplyToken{}},
    {'/', DivideToken{}},
    {'%', ModuloToken{}},
    {'(', OpeningBracketToken{}},
    {')', ClosingBracketToken{}},
    {',', CommaToken{}}
};

const std::unordered_map<std::string, Token> func2Token{
    {"abs", AbsToken{}},
    {"min", MinToken{}},
    {"max", MaxToken{}},
    {"sqr", SqrToken{}}
};


int ToDigit(unsigned char symbol) {
    return symbol - '0';
}


NumberToken ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);

        if (pos == input.size() - 1) {
            ++pos;
            break;
        }

        symbol = static_cast<unsigned char>(input[++pos]);
    }

    return NumberToken{value};
}


Token ParseName(const std::string& input, size_t& pos) {
    auto symbol = input[pos];
    std::string str;

    while (std::isalpha(symbol)) {
        str.push_back(symbol);

        if (pos == input.size() - 1) {
            ++pos;
            break;
        }

        symbol = input[++pos];
    }

    if (auto it = func2Token.find(str); it != func2Token.end()) {
        return it->second;
    }

    return UnknownToken{str};
}


std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;

    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
    
        if (std::isspace(symbol)) {
            ++pos;
        } else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        } else if (auto it = symbol2Token.find(symbol); it != symbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        } else {
            tokens.emplace_back(ParseName(input, pos));
        }
    }

    return tokens;
}


int main() {
    auto res  = Tokenize("(1 + 2) * 3 / 4 % 5 * (6 - 7)");
    auto res2 = Tokenize("abs(-1) + 2 * (3 - 4)");
    auto res3 = Tokenize("1 / 2 + abs(-3) - sqr(4)");
    auto res4 = Tokenize("1 - sqr(2 * 3) + 4 / (abs(-5))");
    auto res5 = Tokenize("min(sqr(1 * 2), max(3, abs(-4)))");
    auto res6 = Tokenize("sqrt(1 * 2)");
}