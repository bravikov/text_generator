/****************************************************************************
**
** MIT License
**
** Copyright (c) 2018 Dmitry Bravikov
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
** OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
** THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <locale>

#include "string_util.h"
#include "markov_chain.h"

struct Options
{
    unsigned markovChainOrder;
};

Options parseOptions(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    StringUtil::setLocale();

    const Options options = parseOptions(argc, argv);

    /* Принять список URL из стандартного потока. */

    std::vector<std::string> urls;

    while (true)
    {
        std::string url;

        std::cin >> url;

        if (std::cin.fail())
        {
            if (std::cin.eof())
            {
                break;
            }
            else
            {
                std::cerr << "Не удалось прочитать URL из потока" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        urls.push_back(url);
    }

    /* Создать цепь Маркова */

    MarkovChain markovChain{options.markovChainOrder};

    for (auto url: urls)
    {

        /* Получить содержимое файла, к которому ведет URL. */

        std::string command{"curl --silent " + url};

        static const char* mode = "r";

        FILE* pipe = popen(command.c_str(), mode);

        if (pipe == nullptr)
        {
            std::cerr << "Не удалось запустить curl." << std::endl;
            exit(EXIT_FAILURE);
        }

        static const size_t bufferSize = 1000; /* Необоснованное значение */
        std::vector<char> buffer(bufferSize);
        std::string string;
        while (fgets(buffer.data(), bufferSize, pipe) != nullptr)
        {
            string += buffer.data();
        }

        /* Добавить последовательность слов к цепи Маркова */

        auto words = StringUtil::split(string);
        markovChain.append(words);

    }

    std::cout << markovChain.toText() << std::endl;

    return EXIT_SUCCESS;
}

Options parseOptions(int argc, char* argv[])
{
    std::string helpMessage{
        "Вызов:\n"
        "\n"
        "  cat url_list | ./markov_chain_generator [order] | tee markov_chain\n"
        "\n"
        "Аргументы: \n"
        "\n"
        "  order — порядок цепи Маркова, целое положительное число или ноль.\n"
    };

    unsigned markovChainOrder = 0;

    if (argc == 2)
    {
        std::stringstream ss;
        ss.str(argv[1]);

        int order;
        ss >> order;

        if (!ss.eof() || ss.fail() || order < 0)
        {
            std::cerr
                << "Ошибка: неверный порядок цепи Маркова."
                << "\n"
                << std::endl
                << helpMessage
                << std::endl
            ;
            exit(EXIT_FAILURE);
        }

        markovChainOrder = order;
    }
    else
    {
        std::cerr << helpMessage << std::endl;
        exit(EXIT_FAILURE);
    }

    return {markovChainOrder};
}
