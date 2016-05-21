app = GrammarTransformationAndParserGenerators

SRCEXT = cpp
SRCDIR = src
OBJDIR = build
BINDIR = bin
INCDIR = include

DEBUG = 1

CFLAGS = -Wall -std=c++1y
LDFLAGS =
LIBS =
LIBDIR =

#************************ DO NOT EDIT BELOW THIS LINE! ************************

ifeq ($(DEBUG),1)
	DEBUG=-g
else
	DEBUG=
endif
INCDIR := $(addprefix -I,$(INCDIR))
LIBS := $(addprefix -l,$(LIBS))
LIBDIR := $(addprefix -L,$(LIBDIR))
CFLAGS += -c $(DEBUG) $(INCDIR) $(LIBDIR) $(LIBS)
SRCS := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
SRCDIRS := $(shell find . -name '*.$(SRCEXT)' -exec dirname {} \; | uniq)
OBJS := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.o,$(SRCS))

ifeq ($(SRCEXT),cpp)
	CC = $(CXX)
else
	CFLAGS += 
endif

.phony: all clean distclean


all: $(BINDIR)/$(app)

$(BINDIR)/$(app): buildrepo $(OBJS)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.$(SRCEXT)
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -r $(OBJDIR)

distclean: clean
	$(RM) -r $(BINDIR)/$(app)

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef


# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFLAGS) \
        $1
endef
