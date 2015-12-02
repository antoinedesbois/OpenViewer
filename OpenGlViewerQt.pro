#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T08:03:33
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGlViewerQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    viewergl.cpp \
    Actor.cpp \
    Viewer.cpp \
    Camera.cpp \
    ConsoleListener.cpp \
    Scene.cpp \
    ModelContainer.cpp \
    Object.cpp \
    BoundingBox.cpp \
    ShaderProgram.cpp \
    Animable.cpp \
    Sphere.cpp \
    Light.cpp \
    ViewerState.cpp \
    DefaultTestLevel.cpp \
    ModelLoader.cpp \
    ObjModelLoader.cpp \
    SSSSTestLevel.cpp \
    FrameBuffer.cpp \
    Quad.cpp

HEADERS  += mainwindow.h \
    viewergl.h \
    Actor.h \
    Types.h \
    Material.h \
    Viewer.h \
    Camera.h \
    ConsoleListener.h \
    Scene.h \
    ModelContainer.h \
    Object.h \
    BoundingBox.h \
    ShaderProgram.h \
    Animable.h \
    Sphere.h \
    Light.h \
    ViewerState.h \
    DefaultTestLevel.h \
    ModelLoader.h \
    ObjModelLoader.h \
    SSSSTestLevel.h \
    FrameBuffer.h \
    Quad.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/includes
#LIBS += -lOpenGL32

LIBS += -L$$PWD/lib/x64/ -lglew -lglfw3 -lOpenGL -lSOIL
