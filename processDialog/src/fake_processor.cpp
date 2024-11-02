#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <operation> <num1> <num2>n";
        return 1;
    }

    std::string operation = argv[1];
    double num1 = std::atof(argv[2]);
    double num2 = std::atof(argv[3]);
    double result;

    if (operation == "sum") {
        result = num1 + num2;
    } else if (operation == "mul") {
        result = num1 * num2;
    } else {
        std::cerr << "Invalid operation. Use 'sum' or 'mul'.n";
        return 1;
    }

    std::cout << "Result: " << result << std::endl;
    return 0;
}
