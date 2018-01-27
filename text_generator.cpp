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

#include "string_util.h"
#include "markov_chain.h"

#include <iostream>
#include <string>
#include <sstream>

void printHelpAndExit();

struct Options
{
    std::string markovFilename;
    unsigned numberOfGeneratedWords;
};

Options parseOptions(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    StringUtil::setLocale();

    const Options options = parseOptions(argc, argv);

    MarkovChain markovChain{options.markovFilename};

    /* Принять открывок из стандартного потока ввода. */

    /* Сгенерировать текст */

    return EXIT_SUCCESS;
}

void printHelpAndExit()
{
    static const std::string helpMessage{
        "Вызов:\n"
        "\n"
        "  echo \"some text\" | ./text_generator markov_chain k\n"
        "\n"
        "Запуск без аргументов выводит справку.\n"
        "\n"
        "Количество слов в \"some text\" должно быть равно порядку цепи маркова"
            " + 1.\n"
        "\n"
        "Аргументы: \n"
        "\n"
        "  markov_chain — файл с цепью Маркова в формате JSON.\n"
        "  k — количество генерируемых слов, то есть, добавляемых к"
            " \"some text\".\n"
    };

    std::cerr << helpMessage << std::endl;
    exit(EXIT_FAILURE);
}

struct Options parseOptions(int argc, char *argv[])
{
    std::string markovFilename;
    unsigned numberOfGeneratedWords;

    if (argc == 3) {
        std::string filename{argv[1]};

        if (filename.empty()) {
            std::cerr << filename
                << "Ошибка: неверное имя файла с цепью Маркова.\n"
                << std::endl;

            printHelpAndExit();
        }

        markovFilename = filename;

        std::stringstream option2;
        option2.str(argv[2]);

        int k = 0;

        option2 >> k;

        if (!option2.eof() || option2.fail() || k < 0) {
            std::cerr
                << "Ошибка: неверное количество генерируемых слов.\n"
                << std::endl;

            printHelpAndExit();
        }

        numberOfGeneratedWords = k;
    }
    else {
        printHelpAndExit();
    }

    return {markovFilename, numberOfGeneratedWords};
}
