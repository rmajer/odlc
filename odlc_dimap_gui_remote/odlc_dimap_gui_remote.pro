#Generated by VisualGDB project wizard. 
#Feel free to modify any flags you want.
#Visit http://visualgdb.com/makefiles for more details.

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = odlc_dimap_gui_remote
TEMPLATE = app

#By default the following file lists are updated automatically by VisualGDB.

SOURCES   += FormSystemLog.cpp hid.cpp ic_ht.cpp Logger.cpp MainWindow.cpp mcp2210.cpp mcp2210device.cpp odlc_dimap_gui_remote.cpp
HEADERS   += FormSystemLog.h hidapi.h ic_ht.h Logger.h MainWindow.h mcp2210.h mcp2210device.h odlc_dimap_gui.h symbols.h
FORMS     += MainWindow.ui SystemLog.ui
RESOURCES += 

include($$lower($$join(CONFIGNAME,,,.pro)))

QMAKE_CFLAGS 	+= $$COMMONFLAGS
QMAKE_CXXFLAGS 	+= $$COMMONFLAGS
QMAKE_LFLAGS 	+= $$COMMONFLAGS

OBJECTS_DIR = $$DESTDIR
MOC_DIR     = $$DESTDIR
RCC_DIR     = $$DESTDIR
UI_DIR      = $$DESTDIR
