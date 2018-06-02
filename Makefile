CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/Explorador

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -Wall -std=c++11
LIB := -lm -lpthread
INC := -I include

UNAME_S := $(shell uname -s)
   ifeq ($(UNAME_S),Linux)
       LIB += -L/usr/local/lib -lGL -lGLU -lglut
   endif
   ifeq ($(UNAME_S),Darwin)
       LIB += -L/usr/local/lib -lm -framework GLUT -framework OpenGL -framework Cocoa
   endif

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
