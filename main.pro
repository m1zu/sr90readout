TARGET = sr90readout
TEMPLATE = app
CONFIG += qt debug warn_on
QMAKE_CXXFLAGS+= -std=c++11 -Wno-deprecated-register

SOURCES += \
  main.cc \

CLASSES += \
  Helpers \
  ProgressBar \
  Functions \
  RootExecFunctionWrapper \
  HistCalculator

for(class, CLASSES) {
  HEADERS += $${class}.hh
  SOURCES += $${class}.cc
}

INCLUDEPATH += \
  $(ROOTSYS)/include

ROOTINCDIR = $$system(root-config --incdir)
!exists ($$ROOTINCDIR/TObject.h) : error("Could NOT find ROOT!")
LIBS += $$system(root-config --cflags --libs) -lHistPainter

DESTDIR = builds
OBJECTS_DIR = builds/.tmp
MOC_DIR = builds/.tmp
UI_DIR = builds/.tmp
RCC_DIR = builds/.tmp

HEADERS += \
    histogram_functions.hh
