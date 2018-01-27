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
#include "3dparty/json.hpp"

using json = nlohmann::json;

class MarkovChain
{
public:
    MarkovChain(const unsigned jOrder = 0);

    void append(const std::vector<std::string> &jWords);
    std::string toText() const;

private:
    const unsigned m_order;
    json m_json;

    json &jRoot() { return m_json["markonChain"]; }
    json &jOrder() { return jRoot()["order"]; }
    json &jWords() { return jRoot()["words"]; }
};

class StringUtil
{
public:
    static std::wstring convert(const std::string &str);
    static std::string convert(const std::wstring &wstr);

    /* Функции разделяют строку на слова не зависимо от кодировки */
    static std::vector<std::string> split(const std::string &str);
    static std::vector<std::wstring> split(const std::wstring &wstr);
};

int main(int argc, char *argv[])
{
    std::locale::global(std::locale("ru_RU.utf8"));
    std::wcout.imbue(std::locale());
    std::wcerr.imbue(std::locale());
    std::wcin.imbue(std::locale());

    /* Обработать аргументы командной строки */

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

    if (argc == 2) {
        std::stringstream ss;
        ss.str(argv[1]);

        int order;
        ss >> order;

        if (!ss.eof() || ss.fail() || order < 0) {
            std::cerr
                << "Ошибка: неверно задан порядок цепи Маркова."
                << "\n"
                << std::endl
                << helpMessage
                << std::endl
            ;
            exit(EXIT_FAILURE);
        }

        markovChainOrder = order;
    }
    else {
        std::cerr << helpMessage << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Принять список URL из стандартного потока. */

    std::vector<std::string> urls;

    while (true) {
        std::string url;

        std::cin >> url;

        if (std::cin.fail()) {
            if (std::cin.eof()) {
                break;
            }
            else {
                std::cerr << "Неудалось прочитать URL из потока" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        urls.push_back(url);
    }

    /* Создать цепь Маркова */

    MarkovChain markovChain{markovChainOrder};

    for (auto url: urls) {

        /* Получить содержимое файла, к которому ведет URL. */

        std::string command{"curl --silent " + url};

        static const char* mode = "r";

        FILE* pipe = popen(command.c_str(), mode);

        if (pipe == nullptr) {
            std::cerr << "Неудалось запустить curl." << std::endl;
            exit(EXIT_FAILURE);
        }

        static const size_t bufferSize = 1000; /* Необоснованное значение */
        std::vector<char> buffer(bufferSize);
        std::string string;
        while (fgets(buffer.data(), bufferSize, pipe) != nullptr) {
            string += buffer.data();
        }

        /* Добавить последовательность слов к цепи Маркова */

        auto words = StringUtil::split(string);
        markovChain.append(words);

    }

    std::cout << markovChain.toText() << std::endl;

    return EXIT_SUCCESS;
}

MarkovChain::MarkovChain(const unsigned order): m_order(order)
{
    jOrder() = m_order;
}

void MarkovChain::append(const std::vector<std::string> &words)
{
    if (words.size() < m_order + 2) {
        return;
    }

    for (unsigned i1 = 0; i1 < words.size() - m_order - 1; i1++) {
        json *jWord = &jWords();

        for (unsigned i2 = 0; i2 < m_order + 1; i2++) {
            auto word = words[i1 + i2];
            jWord = &(*jWord)[word];
        }


        json &jsonWord = (*jWord)[words[i1 + m_order + 1]];

        if (jsonWord.is_null()) {
            jsonWord = 1;
        }
        else {
            jsonWord = jsonWord.get<unsigned>() + 1;
        }
    }
}

std::string MarkovChain::toText() const
{
    static const int indentSize = 4;
    return m_json.dump(indentSize);
}

std::wstring StringUtil::convert(const std::string &str)
{
    if (str.empty()) {
        return std::wstring{};
    }

    std::mbstate_t state = std::mbstate_t();
    const char *strData = str.data();
    std::vector<wchar_t> wArray(str.size());
    std::mbsrtowcs(wArray.data(), &strData, wArray.size(), &state);
    return std::wstring{wArray.data()};
}

std::string StringUtil::convert(const std::wstring &wstr)
{
    if (wstr.empty()) {
        return std::string{};
    }

    std::mbstate_t state = std::mbstate_t();
    const wchar_t *wstrData = wstr.data();
    std::size_t strDataSize = 1 + std::wcsrtombs(nullptr, &wstrData, 0, &state);
    std::vector<char> charArray(strDataSize);
    std::wcsrtombs(charArray.data(), &wstrData, charArray.size(), &state);
    return std::string{charArray.data()};
}

std::vector<std::string> StringUtil::split(const std::string &str)
{
    auto wwords = split(convert(str));

    std::vector<std::string> words(wwords.size());

    for (size_t i = 0; i < words.size(); i++) {
        words[i] = convert(wwords[i]);
    }

    return words;
}

std::vector<std::wstring> StringUtil::split(const std::wstring &wstr)
{
    std::vector<std::wstring> result;

    std::wstring word;

    for (wchar_t symbol: wstr) {
        if (iswalpha(symbol)) {
            word += towlower(symbol);
            continue;
        }

        if (word.empty()) {
            continue;
        }

        result.push_back(word);
        word.clear();
    }

    if (!word.empty()) {
        result.push_back(word);
    }

    return result;
}
