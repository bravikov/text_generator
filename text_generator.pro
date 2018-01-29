QT -= gui

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    markov_chain_generator.cpp \
    text_generator.cpp \
    string_util.cpp \
    markov_chain.cpp \
# SOURCES_END

HEADERS += \
    string_util.h \
    markov_chain.h \
# HEADERS_END

OTHER_FILES += \
    Makefile \
    README.md \
    test/run.sh \
    test/url_list \
    test/mc_texts/*
# OTHER_FILES
