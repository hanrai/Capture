QT += core quick scxml
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    ocr.cpp \
    rect.cpp \
    mouseposition.cpp \
    engine.cpp \
    hotspot.cpp \
    singlecolor.cpp \
    desktopduplication.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ocr.h \
    rect.h \
    mouseposition.h \
    engine.h \
    hotspot.h \
    singlecolor.h \
    desktopduplication.h

DISTFILES +=

win32: LIBS += -L'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/' -ld3d11

INCLUDEPATH += 'C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um'
INCLUDEPATH += 'C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/winrt/wrl'
DEPENDPATH += 'C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um'

win32:!win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/d3d11.lib'
else:win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/libd3d11.a'

win32: LIBS += -L'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/' -ldxgi

INCLUDEPATH += 'C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um'

win32:!win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/dxgi.lib'
else:win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/libdxgi.a'
