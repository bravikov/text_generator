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
#include <list>

void printHelpAndExit();

struct Options
{
    std::string markovFilename;
    uint32_t numberOfGeneratedWords;
};

Options parseOptions(int argc, char *argv[]);
std::vector<std::string> getInputWords();

int main(int argc, char *argv[])
{
    StringUtil::setLocale();

    const Options options = parseOptions(argc, argv);

    MarkovChain markovChain{options.markovFilename};

    std::vector<std::string> words = getInputWords();

    /* Сгенерировать текст */

    if (words.size() != markovChain.order() + 1) {
        std::cerr
            << "Ошибка: количество слов в открывке не соответствует порядку"
               " цепи Маркова."
            << std::endl;
        exit(EXIT_FAILURE);
    }

    for (
        decltype(options.numberOfGeneratedWords) i = 0;
        i < options.numberOfGeneratedWords;
        i++
    ) {
        std::string nextWord = markovChain.nextWord(words);

        if (nextWord.empty()) {
            break;
        }

        words.push_back(nextWord);
    }

    std::string text;
    for (auto word: words) {
        text += word + " ";
    }
    text.pop_back(); // Удалить последний пробел

    std::cout << text << std::endl;

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

        int64_t k = 0;

        option2 >> k;

        auto maxK =
            std::numeric_limits<decltype(Options::numberOfGeneratedWords)>::max();

        if (!option2.eof() || option2.fail() || k < 0 || k > maxK) {
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

std::vector<std::string> getInputWords()
{
    std::string inputString;
    std::getline(std::cin, inputString);
    return StringUtil::split(inputString);
}
