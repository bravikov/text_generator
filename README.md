# Генератор текстов

Генерирует текст на основе цепи Маркова, созданного с помощью генератора цепи Маркова.

Принимает отрывок текста через стандартный поток ввода.

Выводит результат в стандартный поток вывода.

## Сборка

    make

## Запуск

    echo "some text" | ./text_generator markov_chain k

Запуск без аргументов выводит справку.

Количество слов в "some text" должно быть равно порядку цепи маркова + 1.

### Аргументы

 * markov_chain — файл с цепью Маркова в формате JSON.
 * k — количество генерируемых слов, то есть, добавляемых к "some text".

### Возвращаемое значение

 * 0, если цепь Маркова успешно сгенерирова.
 * 1, в любом другом случае.

# Генератор цепи Маркова

Создает цепь Маркова на основе текстовых файлов. Кодировка текста должна быть UTF-8. Список ссылок на текстовые файлы должен поступать через стандартный поток ввода. Выводит цепь в формате JSON в стандартный поток вывода.

## Сборка

    make

## Запуск

    cat url_list | ./markov_chain_generator [order] | tee markov_chain

Запуск без аргументов выводит справку.

### Аргументы

 * order — порядок цепи Маркова, целое положительное число или ноль.

### Возвращаемое значение

 * 0, если цепь Маркова успешно сгенерирова.
 * 1, в любом другом случае.

## JSON

### Цепь Маркова нулевого порядка

word и count — произвольные значения.

```json
{
    "markonChain": {
        "order": 0,
        "words": {
            "word": {
                "nextWords": {
                    "word": count,
                    "word": count
                },
                "nextWordsCount": count
            }
            "word": {
                "nextWords": {
                    "word": count,
                    "word": count
                },
                "nextWordsCount": count
            }
        }
    }
}
```

### Цепь Маркова первого порядка

word и count — произвольные значения.

```json
{
    "markonChain": {
        "order": 1,
        "words": {
            "word": {
                "word": {
                    "nextWords": {
                        "word": count,
                        "word": count
                    },
                    "nextWordsCount": count
                }
                "word": {
                    "nextWords": {
                        "word": count,
                        "word": count
                    },
                    "nextWordsCount": count
                }
            }
            "word": {
                "word": {
                    "nextWords": {
                        "word": count,
                        "word": count
                    },
                    "nextWordsCount": count
                }
            }
        }
    }
}
```

### Пример

 * Строка: "аа аа бб бб бб вв вв вв вв гг гг гг гг гг".
 * Порядок цепи: 1.

Цепь Маркова:

```json
{
    "markonChain": {
        "order": 1,
        "words": {
            "аа": {
                "аа": {
                    "nextWords": {
                        "бб": 1
                    },
                    "nextWordsCount": 1
                },
                "бб": {
                    "nextWords": {
                        "бб": 1
                    },
                    "nextWordsCount": 1
                }
            },
            "бб": {
                "бб": {
                    "nextWords": {
                        "бб": 1,
                        "вв": 1
                    },
                    "nextWordsCount": 2
                },
                "вв": {
                    "nextWords": {
                        "вв": 1
                    },
                    "nextWordsCount": 1
                }
            },
            "вв": {
                "вв": {
                    "nextWords": {
                        "вв": 2,
                        "гг": 1
                    },
                    "nextWordsCount": 3
                },
                "гг": {
                    "nextWords": {
                        "гг": 1
                    },
                    "nextWordsCount": 1
                }
            },
            "гг": {
                "гг": {
                    "nextWords": {
                        "гг": 3,
                        "хх": 1
                    },
                    "nextWordsCount": 4
                },
                "хх": {
                    "nextWords": {
                        "уу": 1
                    },
                    "nextWordsCount": 1
                }
            },
            "хх": {
                "уу": {
                    "nextWords": {
                        "йй": 1
                    },
                    "nextWordsCount": 1
                }
            }
        }
    }
}
```

# Тест

## Запуск

    test/run.sh
