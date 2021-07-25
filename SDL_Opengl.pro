TEMPLATE = app
CONFIG += console
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -LC:\Users\Mahdi\Documents\QtDeps\lib -lSDL2 -lopengl32 -lglew32 -lfreetype -ljsoncpp
INCLUDEPATH += include

SOURCES += main.cpp \
    window.cpp \
    iomanager.cpp \
    inputmanager.cpp \
    glslprogram.cpp \
    spritebatch.cpp \
    vertex.cpp \
    debugrenderer.cpp \
    utils.cpp \
    batchrenderer.cpp \
    picopng.cpp \
    camera2d.cpp \
    zgui.cpp \
    imageloader.cpp \
    tilesheet.cpp \
    fontmanager.cpp \
    resourcemanager.cpp \
    jsonfunctions.cpp \
    freetype-gl/texture-atlas.cpp \
    freetype-gl/platform.cpp \
    freetype-gl/edtaa3func.cpp \
    freetype-gl/distance-field.cpp \
    freetype-gl/texture-font.cpp \
    freetype-gl/utf8-utils.cpp \
    freetype-gl/vector.cpp \
    spritefont.cpp \
    button.cpp \
    PushButton.cpp \
    Slider.cpp \
    ZWindow.cpp \
    ImageButton.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    window.h \
    iomanager.h \
    inputmanager.h \
    glslprogram.h \
    common.h \
    spritebatch.h \
    vertex.h \
    spritefont.h \
    debugrenderer.h \
    utils.h \
    batchrenderer.h \
    picopng.h \
    camera2d.h \
    ztime.h \
    random.h \
    zgui.h \
    imageloader.h \
    tilesheet.h \
    gltexture.h \
    fontmanager.h \
    hash.h \
    resourcemanager.h \
    jsonfunctions.h \
    opengl.h \
    freetype-gl/distance-field.h \
    freetype-gl/edtaa3func.h \
    freetype-gl/platform.h \
    freetype-gl/texture-atlas.h \
    freetype-gl/texture-font.h \
    freetype-gl/utf8-utils.h \
    freetype-gl/vec234.h \
    freetype-gl/vector.h \
    freetypegl.h \
    Button.h \
    PushButton.h \
    Slider.h \
    ZWindow.h \
    ImageButton.h

RESOURCES +=

