#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
std::istream& read_size(std::istream& input, T& sz)
{
    T tmp;
    if (input >> tmp)
        sz = tmp;
    return input;
}

template <typename T>
std::istream& read_array(std::istream& input, const size_t count, std::vector<T>& dst)
{
    T tmp;
    size_t readed = 0;
    while (readed < count && input >> tmp)
    {
        ++readed;
        dst.push_back(tmp);
    }
    return input;
}

template <typename T>
size_t inverses_count(const std::vector<T>& array)
{
    size_t result = 0;

    for (size_t i = 0, sz = array.size(); i < sz-1; ++i)
    {
        for (size_t j = i+1; j < sz; ++j)
        {
            if (array[i] > array[j])
            {
                ++result;
            }
        }
    }

    return result;
}

}

int main(int /*argc*/, char** /*argv*/)
{
    const std::string type(STREAM_TYPE);
    
    std::istream* in = nullptr;
    std::ostream* out = nullptr;

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

    size_t array_size = 0;
    ::read_size(*in, array_size);
   
    std::vector<int32_t> array;
    array.reserve(array_size);
    ::read_array(*in, array_size, array);
   
    (*out) << ::inverses_count(array) << std::endl; 

    if (::is_file_stream(type))
    {
        delete in; in = nullptr;
        delete out; out = nullptr;
    }
    
    return EXIT_SUCCESS;
}
