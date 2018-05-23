CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/Game

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -Wall -std=c++11
LIB := -lGL -lGLU -lglut -lm
INC := -I include

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	$(RM) -r $(BUILDDIR) $(TARGET)

test: $(OBJECTS)
	 $(CC) $(CFLAGS) $(OBJECTS) test/$(TEST).cpp -o bin/$(TEST) $(LIB) $(INC)
 
.PHONY: clean

