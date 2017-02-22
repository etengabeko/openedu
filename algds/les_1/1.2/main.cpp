#include <fstream>
#include <iostream>
#include <string>

#ifndef STREAM_TYPE
#define STREAM_TYPE "file"
#endif // STREAM_TYPE

namespace
{

bool is_file_stream(const std::string& type)
{
    return type == "file";
}

template <typename T>
std::istream& read(std::istream& input, T& val_a, T& val_b)
{
    T tmp_a, tmp_b;
    if (   input >> tmp_a
        && input >> tmp_b)
    {
        val_a = tmp_a;
        val_b = tmp_b;
    }
    return input;
}

template <typename T>
std::ostream& write(std::ostream& output, const T& value)
{
    output << value << std::endl;
    return output;
}

}

int main(int /*argc*/, char** /*argv*/)
{
    std::istream* in  = nullptr;
    std::ostream* out = nullptr;

    std::string type(STREAM_TYPE);

    if (::is_file_stream(type))
    {
        in = new std::ifstream("input.txt");
        out = new std::ofstream("output.txt");
    }
    else
    {
        in = &std::cin;
        out = &std::cout;
    }

    long long a = 0;
    long long b = 0;
    
    ::read(*in, a, b);
    ::write(*out, (a + b*b));
    
    if (::is_file_stream(type))
    {
        delete in; in = nullptr;
        delete out; out = nullptr;
    }

    return EXIT_SUCCESS;
}
