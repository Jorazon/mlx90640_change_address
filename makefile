PROJECTNAME=change_address

SRCDIR=./src
OBJDIR=./obj
INCDIR=./include
BUILDDIR=./build

CXX=g++
COMMONFLAGS=-Wall -Wextra -Wsign-compare -Wpedantic -g
CFLAGS=$(COMMONFLAGS)
CPPFLAGS=$(COMMONFLAGS) -Weffc++
RM=rm -fr

# Create include flag list 
INC_PATH=$(patsubst %,-I%,$(INCDIR))

# Find all .c and .cpp source files
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c)

# Create object list by replacing source file directory and extension with object file directory and extension
OBJS := $(addprefix $(OBJDIR)/,$(addsuffix .o,$(notdir $(basename $(SRCS)))))

default: $(PROJECTNAME)

# Project requires build directory and object files to link
$(PROJECTNAME): $(OBJS) $(BUILDDIR)
	$(CXX) -o $(BUILDDIR)/$(PROJECTNAME) $(OBJS)

# Object files require the object directory and source files
$(OBJS): $(OBJDIR) $(SRCS)

# Compile or assemble the C++ source files into object files, but do not link
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(INC_PATH) -c -o $@ $<

# Compile or assemble the C source files into object files, but do not link
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) $(CFLAGS) $(INC_PATH) -c -o $@ $<

# Create directory for compiled objects
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Create directory for built files
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Delete diercorties and files created by make 
clean:
	$(RM) $(OBJDIR) $(BUILDDIR)
