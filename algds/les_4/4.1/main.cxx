#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
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
class Command
{
public:
    enum class Type
    {
        Unknown,
        Push,
        Pop
    };

    Command() = default;

    explicit Command(Type type, const T& value = 0) :
        m_type(type),
        m_value(value)
    { }

    bool is_pop() const
    {
        return m_type == Type::Pop;
    }

    T execute_on(std::list<T>& dest) const
    {
        T result{0};
        switch(m_type)
        {
        case Type::Push:
            dest.push_back(m_value);
            result = m_value;
            break;
        case Type::Pop:
            if (!dest.empty())
            {
                result = dest.back();
                dest.pop_back();
            }
            break;
        default:
            break;
        }
        return result;
    }

    bool is_valid() const
    {
        return m_type != Type::Unknown;
    }

private:
    Type m_type = Type::Unknown;
    T m_value = 0;

};

template <typename T>
std::istream& operator>> (std::istream& src, Command<T>& dest)
{
    char sign;
    if (src >> sign)
    {
        switch(sign)
        {
        case '+':
            {
                T tmp{0};
                if (src >> tmp)
                    dest = Command<T>(Command<T>::Type::Push, tmp);
            }
            break;
        case '-':
            dest = Command<T>(Command<T>::Type::Pop);
            break;
        default:
            break;
        }
    }
    return src;
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

    size_t sz = 0;
    
    std::string tmp;
    std::getline(*in, tmp);
    {
        std::istringstream ss(tmp);
        ::read_size(ss, sz);
    }

    std::vector<Command<int32_t>> commands;
    commands.reserve(sz);

    tmp.clear();
    while (sz-- > 0 && std::getline(*in, tmp))
    {
        Command<int32_t> cmd;
        std::istringstream ss(tmp);
        if (ss >> cmd && cmd.is_valid())
            commands.push_back(cmd);
    }
   
    std::list<int32_t> stack;

    std::for_each(commands.cbegin(),
                  commands.cend(),
                  [&stack, out](const Command<int32_t>& each)
                  {
                      int32_t result = each.execute_on(stack);
                      if (each.is_pop())
                          (*out) << result << '\n';
                  });

    if (::is_file_stream(type))
    {
        delete in; in = nullptr;
        delete out; out = nullptr;
    }
    
    return EXIT_SUCCESS;
}
