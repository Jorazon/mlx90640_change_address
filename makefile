OUT = change_address

CXX = g++
CFLAGS = 
CPPFLAGS = 
RM = rm -fr

SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./include
BUILDDIR = ./build

INC_PATH = $(patsubst %, -I%, $(INCDIR))

VPATH=%.o $(OBJDIR)
VPATH=%.c $(SRCDIR)
VPATH=%.cpp $(SRCDIR)
VPATH=%.h $(INCDIR)

SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c)
OBJS := $(addprefix $(OBJDIR)/, $(addsuffix .o,$(notdir $(basename $(SRCS)))))

$(info $(SRCS))
$(info $(OBJS))

all: $(OUT)

$(OUT): $(OBJS) $(BUILDDIR)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INC_PATH) -o $(BUILDDIR)/$(OUT) $(OBJS)

$(OBJS) : $(OBJDIR) $(SRCS)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(SRCDIR)/%.c
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INC_PATH) -c -o $@ $<

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INC_PATH) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	$(RM) $(OBJDIR)
