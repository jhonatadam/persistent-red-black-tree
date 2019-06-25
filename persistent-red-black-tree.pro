TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    persistentredblacktree.cpp \
    persistentnode.cpp

HEADERS += \
    persistentredblacktree.h \
    persistentnode.h
