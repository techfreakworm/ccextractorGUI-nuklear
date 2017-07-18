AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = ccextractorgui
ccextractorgui_SOURCES = \
	../src/ccextractorGUI.c \
	../src/ccextractorGUI.h \
	../src/activity.c \
	../src/terminal.c \
	../src/preview.c \
	../src/ccx_cli_thread.c \
	../src/ccx_cli_thread.h \
	../src/command_builder.c \
	../src/command_builder.h \
	../src/file_browser.c \
	../src/file_browser.h \
	../src/popups.c \
	../src/popups.h \
	../src/tabs.c \
	../src/tabs.h \
	../src/stb_image.h \
	../src/nuklear_lib/nuklear.h \
	../src/nuklear_lib/nuklear_glfw_gl2.h

ccextractorgui_CFLAGS = -std=gnu99

#ccextractorgui_CPPFLAGS = -I../fonts/ -I../icons/ -I../nuklear_lib

ccextractorgui_LDADD = -lGLEW 

if SYS_IS_LINUX
ccextractorgui_LDADD += -lglfw3 -lX11 -lXinerama -lXcursor -lXi -lXrandr -lXxf86vm -lGL -lGLU -lm -ldl -lpthread
ccextractorgui_CFLAGS += -s -O3 -DUNIX
endif

if SYS_IS_MAC
ccextractorgui_CFLAGS += -O3 -DUNIX
ccextractorgui_LDFLAGS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
ccextractorgui_LDADD += -lglfw  -lm -L/usr/local/lib -lpthread
endif

if HARDSUBX_IS_ENABLED
if OCR_IS_ENABLED
ccextractorgui_CFLAGS += -DENABLE_OCR 
endif
endif









