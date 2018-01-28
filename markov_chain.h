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

#ifndef MARKOVCHAIN_H
#define MARKOVCHAIN_H

#include "3dparty/json.hpp"

#include <string>
#include <vector>

using json = nlohmann::json;

class MarkovChain
{
public:
    MarkovChain(const uint32_t &order = 0);
    MarkovChain(const std::string &filename);

    void append(const std::vector<std::string> &jWords);
    std::string toText() const;
    uint32_t order() const;
    std::string nextWord(const std::vector<std::string> &words);

private:
    uint32_t m_order;
    json m_json;

    json &jRoot() { return m_json["markonChain"]; }
    json &jOrder() { return jRoot()["order"]; }
    json &jWords() { return jRoot()["words"]; }
    json &jNextWordsCount(json &j) { return j["nextWordsCount"]; }
    json &jNextWords(json &j) { return j["nextWords"]; }
};


#endif // MARKOVCHAIN_H
