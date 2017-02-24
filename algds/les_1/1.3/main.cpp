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
    T tmp(0);
    if (input >> tmp)
        sz = tmp;
    return input;
}

template <typename T>
std::istream& read_array(std::istream& input, const size_t count, std::vector<T>& dst)
{
    T tmp(0);
    size_t readed = 0;
    while (readed < count && input >> tmp)
    {
        ++readed;
        dst.push_back(tmp);
    }
    return input;
}

template <typename T>
std::ostream& write_array(std::ostream& output, const std::vector<T>& src)
{
    std::for_each(src.cbegin(),
                  src.cend(),
                  [&output](const T& each)
                  {
                      output << each << " ";
                  });
    return output;
}

template <typename T>
std::vector<size_t> insertion_sort(std::vector<T>& src)
{
    const size_t src_size = src.size();

    std::vector<size_t> move_steps(src_size, 1);
    
    if (src_size > 1) // need sort array
    {
        for (size_t i = 0; i < src_size; ++i)
        {
            size_t j = i;
            while (j > 0 && src[j] < src[j-1])
            {
                std::swap(src[j], src[j-1]);
                --j;
            }
            move_steps[i] = j+1;
        }
    }

    return move_steps;
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
   
    std::vector<long> array;
    array.reserve(array_size);
    ::read_array(*in, array_size, array);
    
    std::vector<size_t> moves = ::insertion_sort(array);

    ::write_array(*out, moves);
    (*out) << std::endl;
    ::write_array(*out, array);
    (*out) << std::endl;

    if (::is_file_stream(type))
    {
        delete in; in = nullptr;
        delete out; out = nullptr;
    }    
    return EXIT_SUCCESS;
}
