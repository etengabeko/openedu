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
void scarecrow_sort(std::vector<T>& array, const size_t reach)
{
    size_t swaps_count = 0;

    do
    {
        swaps_count = 0;
        for (size_t i = 0, sz = array.size(); i < sz-reach; ++i)
        {
            if (array[i] > array[i+reach])
            {
                std::swap(array[i], array[i+reach]);
                ++swaps_count;
            }
        }
    } while (swaps_count > 0);
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

    size_t reach = 0;
    ::read_size(*in, reach);
   
    std::vector<int32_t> array;
    array.reserve(array_size);
    ::read_array(*in, array_size, array);

    ::scarecrow_sort(array, reach);
    
    array_size = array.size();
    bool is_sorted = array_size <= 1;

    if (!is_sorted)
    {
        for (size_t i = 0; i < array_size-1; ++i)
        {
            if (array[i] > array[i+1])
            {
                break;
            }
            if (i+1 == array_size-1)
            {
                is_sorted = true;
            }
        }
    }

    (*out) << (is_sorted ? "YES" : "NO") << std::endl;

    if (::is_file_stream(type))
    {
        delete in; in = nullptr;
        delete out; out = nullptr;
    }
    
    return EXIT_SUCCESS;
}
