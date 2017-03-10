#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
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
std::vector<T> multiplex_arrays(const std::vector<T>& a, const std::vector<T>& b)
{
    std::map<T, std::vector<T>> buckets;
    const size_t buckets_count = 40000*40000;
    
    std::for_each(a.cbegin(), a.cend(), [&buckets, &b, &buckets_count](const T& each_a)
                                        {
                                            std::for_each(b.cbegin(), b.cend(), [&buckets, &each_a, &buckets_count](const T& each_b)
                                                                                {
                                                                                    T each = each_a * each_b;
                                                                                    T key = each / buckets_count;
                                                                                    buckets[key].push_back(each);
                                                                                });
                                        });
    
    std::vector<T> result;
    result.reserve(a.size() * b.size());
    for(auto it = buckets.begin(), end = buckets.end(); it != end; ++it)
    {
        std::sort(it->second.begin(), it->second.end());
        result.insert(result.end(), it->second.begin(), it->second.end());
    }    

    return result;
}

template <typename T>
T sum_tenth(std::vector<T>& array)
{
    T sum = 0;
    std::sort(array.begin(), array.end());
    for (size_t i = 0, sz = array.size(); i < sz; i += 10)
    {
        sum += array[i];
    }
    return sum;
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

    size_t a_size = 0;
    size_t b_size = 0;
    
    std::string tmp;
    std::getline(*in, tmp);
    {
        std::istringstream ss(tmp);
        ::read_size(ss, a_size);
        ::read_size(ss, b_size);
    }

    std::vector<long long> va;
    va.reserve(a_size);
    std::vector<long long> vb;
    vb.reserve(b_size);

    tmp.clear();
    std::getline(*in, tmp);
    {
        std::istringstream ss(tmp);
        ::read_array(ss, a_size, va);
    }
   
    tmp.clear();
    std::getline(*in, tmp);
    {
        std::istringstream ss(tmp);
        ::read_array(ss, b_size, vb);
    }

    std::vector<long long> mv = ::multiplex_arrays(va, vb);
    va.clear();
    vb.clear();

    (*out) << ::sum_tenth(mv) << '\n';

    if (::is_file_stream(type))
    {
        delete in; in = nullptr;
        delete out; out = nullptr;
    }
    
    return EXIT_SUCCESS;
}
