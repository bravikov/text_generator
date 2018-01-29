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

#include <locale>
#include <iostream>

void StringUtil::setLocale()
{
    std::locale::global(std::locale("ru_RU.utf8"));
    std::wcout.imbue(std::locale());
    std::wcerr.imbue(std::locale());
    std::wcin.imbue(std::locale());
}

std::wstring StringUtil::convert(const std::string& str)
{
    if (str.empty())
    {
        return std::wstring{};
    }

    std::mbstate_t state = std::mbstate_t();
    const char* strData = str.data();
    std::vector<wchar_t> wArray(str.size());
    std::mbsrtowcs(wArray.data(), &strData, wArray.size(), &state);
    return std::wstring{wArray.data()};
}

std::string StringUtil::convert(const std::wstring& wstr)
{
    if (wstr.empty())
    {
        return std::string{};
    }

    std::mbstate_t state = std::mbstate_t();
    const wchar_t* wstrData = wstr.data();
    std::size_t strDataSize = 1 + std::wcsrtombs(nullptr, &wstrData, 0, &state);
    std::vector<char> charArray(strDataSize);
    std::wcsrtombs(charArray.data(), &wstrData, charArray.size(), &state);
    return std::string{charArray.data()};
}

std::vector<std::string> StringUtil::split(const std::string& str)
{
    auto wwords = split(convert(str));

    std::vector<std::string> words(wwords.size());

    for (size_t i = 0; i < words.size(); i++)
    {
        words[i] = convert(wwords[i]);
    }

    return words;
}

std::vector<std::wstring> StringUtil::split(const std::wstring& wstr)
{
    std::vector<std::wstring> result;

    std::wstring word;

    for (wchar_t symbol: wstr)
    {
        if (iswalpha(symbol))
        {
            word += towlower(symbol);
            continue;
        }

        if (word.empty())
        {
            continue;
        }

        result.push_back(word);
        word.clear();
    }

    if (!word.empty())
    {
        result.push_back(word);
    }

    return result;
}
