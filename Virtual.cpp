#include <iostream>
#include <string>
#include <cctype>

class Compiler {
    std::string input;
    size_t pos = 0;

    char peek() {
        return pos < input.size() ? input[pos] : '\0';
    }

    char get() {
        return pos < input.size() ? input[pos++] : '\0';
    }

    void skipWhitespace() {
        while (isspace(peek())) get();
    }

    int parseNumber() {
        skipWhitespace();
        int num = 0;
        while (isdigit(peek())) {
            num = num * 10 + (get() - '0');
        }
        return num;
    }

    int parseFactor() {
        skipWhitespace();
        if (peek() == '(') {
            get(); // consume '('
            int result = parseExpression();
            if (get() != ')') throw std::runtime_error("Expected ')'");
            return result;
        } else {
            return parseNumber();
        }
    }

    int parseTerm() {
        int result = parseFactor();
        while (true) {
            skipWhitespace();
            if (peek() == '*') {
                get();
                result *= parseFactor();
            } else if (peek() == '/') {
                get();
                int divisor = parseFactor();
                if (divisor == 0) throw std::runtime_error("Division by zero");
                result /= divisor;
            } else {
                break;
            }
        }
        return result;
    }

    int parseExpression() {
        int result = parseTerm();
        while (true) {
            skipWhitespace();
            if (peek() == '+') {
                get();
                result += parseTerm();
            } else if (peek() == '-') {
                get();
                result -= parseTerm();
            } else {
                break;
            }
        }
        return result;
    }

public:
    int compile(const std::string& expr) {
        input = expr;
        pos = 0;
        int result = parseExpression();
        if (pos != input.size()) throw std::runtime_error("Unexpected characters at end");
        return result;
    }
};

int main() {
    Compiler compiler;
    std::string line;

    std::cout << "Enter an arithmetic expression (or 'exit' to quit):\n";
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit") break;

        try {
            int result = compiler.compile(line);
            std::cout << "Result: " << result << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}


