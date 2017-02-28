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

class Logger
{
private:
    explicit Logger(std::ostream& output) :
        m_output(output)
    { }

public:
    static Logger& instance(std::ostream& output)
    {
        static Logger logger(output);
        return logger;
    }
    
    template <typename T>
    Logger& info_range(const T& first, const T& last)
    {
        m_output << first << " " << last;
        return *this;
    }

    Logger& info_string(const std::string& str)
    {
        m_output << str;
        return *this;
    }

private:
    std::ostream& m_output;

};

template <typename T, typename Iterator>
std::vector<T> merge(Iterator one_first, Iterator one_last,
                     Iterator two_first, Iterator two_last)
{
    std::vector<T> result;

    const size_t one_sz = std::distance(one_first, one_last);
    const size_t two_sz = std::distance(two_first, two_last);
    result.reserve(one_sz + two_sz);

    while (one_first != one_last || two_first != two_last)
    {
        if (   one_first != one_last
            && two_first == two_last)
        {
            result.push_back(*(one_first++));
        }
        else if (   one_first == one_last
                 && two_first != two_last)
        {
            result.push_back(*(two_first++));
        }
        else
        {
            if (*one_first < *two_first)
            {
                result.push_back(*(one_first++));
            }
            else
            {
                result.push_back(*(two_first++));
            }
        }
    }

    return result;
}

template <typename T, typename Iterator>
std::vector<T> merge_sort(Iterator first, Iterator last, Logger* logger = nullptr)
{
    std::vector<T> result;
    const size_t sz = std::distance(first, last);
    result.reserve(sz);

    if (sz == 1)
    {
        result.push_back(*first);
    }
    else if (sz > 1)
    {
        std::vector<T> one = merge_sort<T>(first, first + sz/2, logger);
        std::vector<T> two = merge_sort<T>(first + sz/2, last, logger);
        result = merge<T>(one.cbegin(), one.cend(),
                          two.cbegin(), two.cend());
    }
    
    if (logger != nullptr && sz > 1)
    {
        (*logger).info_range((*first).m_index, (*(last-1)).m_index)
                 .info_string(" ")
                 .info_range(*result.cbegin(), *(--result.cend()))
                 .info_string("\n");
    }
    return result;
}

template <typename T>
struct value_with_index
{
    T m_value = 0;
    size_t m_index = 0;

};

template <typename T>
bool operator< (const value_with_index<T>& lhs, const value_with_index<T>& rhs)
{
    return lhs.m_value < rhs.m_value;
}

template <typename T>
std::istream& operator>> (std::istream& src, value_with_index<T>& dst)
{
    src >> dst.m_value;
    return src;
}

template <typename T>
std::ostream& operator<< (std::ostream& dst, const value_with_index<T>& src)
{
    dst << src.m_value;
    return dst;
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
   
    std::vector<value_with_index<int32_t>> array;
    array.reserve(array_size);
    ::read_array(*in, array_size, array);

    for (size_t i = 0, sz = array.size(); i < sz; ++i)
    {
        array[i].m_index = i+1;
    }
    
    array = ::merge_sort<value_with_index<int32_t>>(array.cbegin(), array.cend(), &Logger::instance(*out));

    ::write_array(*out, array);
    (*out) << std::endl;

    if (::is_file_stream(type))
    {
        delete in; in = nullptr;
        delete out; out = nullptr;
    }
    
    return EXIT_SUCCESS;
}
