##
## EPITECH PROJECT, 2024
## RType Systems Makefile
## File description:
## Makefile for Systems
##

SYSTEMS = move \

all:
	@for system in $(SYSTEMS); do \
		$(MAKE) -C $$system; \
	done

clean:
	@for system in $(SYSTEMS); do \
		$(MAKE) -C $$system clean; \
	done

fclean:
	@for system in $(SYSTEMS); do \
		$(MAKE) -C $$system fclean; \
	done

re:
	@for system in $(SYSTEMS); do \
		$(MAKE) -C $$system re; \
	done

.PHONY: re clean fclean
