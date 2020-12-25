QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        lexer/dfamatcher.cpp \
        lexer/lexer.cpp \
        lexer/mytockens.cpp \
        lexer/scanner.cpp \
        lexer/tocken.cpp \
        main.cpp \
        parser/parseelement.cpp \
        parser/parser.cpp \
        regEngine/dfa.cpp \
        regEngine/mdfa.cpp \
        regEngine/nfa.cpp \
        regEngine/nfa_graph.cpp \
        regEngine/reg_tree.cpp
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin \
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    lexer/dfamatcher.h \
    lexer/lexer.h \
    lexer/mytockens.h \
    lexer/scanner.h \
    lexer/tocken.h \
    lexer/tockensDefine.h \
    logger.h \
    parser/parseelement.h \
    parser/parser.h \
    regEngine/dfa.h \
    regEngine/dfa_graph.h \
    regEngine/mdfa.h \
    regEngine/nfa.h \
    regEngine/nfa_graph.h \
    regEngine/reg_node.h \
    regEngine/reg_tree.h \
    regEngine/unitTest.h \
    util/treestructprinter.h
