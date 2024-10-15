NAME	:=	unit_tests

TEST_FILES	:= Zipper_tests.cpp	\

TESTED_SRC	:=

SRC	:= $(TEST_FILES) $(TESTED_SRC)

EXT	:=	cpp

CXXFLAGS	+=	-Wall -Wextra --coverage -std=c++20
LDFLAGS		+=	-lcriterion

sinclude ./oneBin.mk

.PHONY: all
all: $(NAME)

GCOVR	:= gcovr -r . $(foreach file,$(TEST_FILES),--exclude $(file))

CODE_COV	=	code.cov
BRANCH_COV	=	branch.cov

.PHONY: tests_run
tests_run: $(CODE_COV) $(BRANCH_COV)

$(CODE_COV) $(BRANCH_COV) &: $(NAME)
	./$(NAME)
	@$(GCOVR) > $(CODE_COV)
	@($(GCOVR) --txt-metric branch || $(GCOVR) --branches) > $(BRANCH_COV)
	@echo
	@echo -e '✅ Wrote code coverage report to \e[1;32m$(CODE_COV)\e[0m:'
	@cat $(CODE_COV)
	@echo
	@echo -e '✅ Wrote branch coverage report to \e[1;32m$(BRANCH_COV)\e[0m:'
	@cat $(BRANCH_COV)
	@find -name '*.gcno' -delete -or -name '*.gcda' -delete
