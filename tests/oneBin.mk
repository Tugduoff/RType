##
## EPITECH PROJECT, 2024
## RType
## File description:
## This Makefile aims at optimising recompilation of C/C++ projects.
## When a source file is compiled to an object file, a dependency
## file describing  which headers  the source file  depends on is
## created as  a side  product. This  Makefile then  uses all the
## generated dependency files  in order to recompile sources when
## headers are  modified, even if the source itself  has not been
## touched.
##

#################################################################
## Usage:
## In order to use this Makefile, you need to create a separate
## Makefile, which defines some variables, and which then
## includes this Makefile, using the 'include' directive
##
## Variables to define:
## NAME: The filename of the executable to be built.
##         Defaults to 'a.out', but this is rarely what you want.
##
## SRC:  The list of files the executable is built from.
##         Note: the sources should either be all C or all C++
##
## EXT:  The file extension of your source files, usually c or cpp
##         Note: You should not include the dot delimitor '.'
##
## If you forget to define any of these variables, you should see
## an error or warning message printed when running `make`,
## indicating what definition is missing
##
## Overridable variables:
## COLNAMES:  Function which builds a string describing what objects
##              are being built from a base file name. For example,
##              the default definition, from the input 'main', will
##              return 'main.o and main.d', with ANSI colour indicators
##
## UPDATMSG:  Function which returns a string explaining why a file is
##              is rebuilt. The default definition makes an internal
##              call to COLNAMES.
##
## BUILDMSG:  Function which returns a string explaining from what
##              prerequisite a target is built. The default
##              definition makes an internal call to COLNAMES.
##
## box:       String to be displayed at the start of a line in a
##              different colour depending on the type of action
##              taken
#################################################################

#### Definition of util variables to format text ####

comma:=,
empty:=
space:=$(empty) $(empty)

## @brief Function to comma-separate a list of words
##
## @param $1 The list of words to comma-separate
define commasep
$(subst $(space),$(comma)$(space),$1)
endef


#### Safety checks to provide sensible error ####
#### messages when the Makefile is misused ####

ifndef NAME
$(info \
	oneBin.mk: You should define the NAME variable to specify \
	the name of the executable to build. It currently defaults to 'a.out' \
)
endif

ifndef SRC
$(error \
	You need to provide a SRC variable which specifies the \
	list of source files to compile in order to build '$(NAME)' \
)
endif

ifndef EXT
$(error \
	You need to define EXT (either to c or cpp) in order to specify \
	the extension of your SRC files, which will determine the compiler to use\
)
endif

## @brief List of valid extensions to use for C/C++
VALID_EXTENSIONS	:=	c cpp C cc

# ? If the defined extension is not in the list of valid extensions
ifeq (,$(findstring $(EXT),$(VALID_EXTENSIONS)))
$(warning \
	The defined extension '$(EXT)' is not part of the list of valid \
	extensions ($(call commasep,$(VALID_EXTENSIONS))) \
)
endif


#### Variable definitions ####

NAME	?=	a.out

OBJ	=	$(SRC:.$(EXT)=.o)
DEPFILES = $(SRC:.$(EXT)=.d)


#### Explicit rules ####

$(NAME): $(OBJ) | $(DEPFILES)
	@echo -e '$(sgreebox) Linking  \e[1;32m$@\e[0m'
	@$(LINK.$(EXT)) $^ $(OUTPUT_OPTION)

clean:
	@echo -e '$(grey_box) Cleaning object files'
	@$(RM) $(OBJ) $(DEPFILES)

fclean: clean
	@echo -e '$(grey_box) Removing \e[1;32m$(NAME)\e[0m'
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re


#### Implicit rules to compile objects and track deps ####

## @brief function to produce colour output from a base name
##
## @param $1 The base name of the file
define COLNAMES ?=
\e[1;4;37m$1.o\e[0m and \e[1;4;37m$1.d\e[0m
endef

## @brief Function to generate a message describing why a rule is run
##
## @param $1 The base name of source file
## @param $2 The list of changed files
define UPDATMSG ?=
Updating $(call COLNAMES,$1) because {\e[1;36m$2\e[0m} changed
endef

## @brief Function to generate a string describing from what a target is built
##
## @param $1 The base name of source file
define BUILDMSG ?=
Building $(call COLNAMES,$1) from \e[1;36m$<\e[0m
endef

box	?=	[]

grey_box	:=	\e[1;2;37m$(box)\e[0m
greenbox	:=	\e[1;32m$(box)\e[0m
sgreebox	:=	\e[1;42m$(box)\e[0m
yellobox	:=	\e[1;33m$(box)\e[0m

%.o: CPPFLAGS += -MMD -MF $*.d -MT '$*.o $*.d' -MP
%.o: %.$(EXT) %.d
	@if [ ! -f "$@" ]; then												\
	 	echo -e '$(greenbox) $(call BUILDMSG,$*)';						\
	 else																\
	 	echo -e '$(yellobox) $(call UPDATMSG,$*,$(call commasep,$?))';	\
	 fi
	@$(COMPILE.$(EXT)) $< -o $*.o
	@touch $@

$(DEPFILES): ;

-include $(DEPFILES)
