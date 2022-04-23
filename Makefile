COMPILER   = gcc
OPTION	   = -g -std=c11 -mwindows -lwinmm -lgdi32 -I ./libgraphics -I ./simpleGUI
TARGET     = main.exe
SOURCE     = $(wildcard *.c)
SOURCE    += $(wildcard */*.c)
OBJECT     = $(SOURCE:.c=.o)
DEPENDENCY = $(OBJECT:.o=.d)

$(TARGET): $(OBJECT)
	$(COMPILER) -o $@ $^ $(OPTION)

%.o: %.c
	$(COMPILER) -c $< -o $@ $(OPTION)

-include $(DEPENDENCY)

.PHONY: clean
clean:
	-del /s *.o
	-del /s *.d
	-del $(TARGET)