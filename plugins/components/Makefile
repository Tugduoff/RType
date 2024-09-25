##
## EPITECH PROJECT, 2024
## RType Components Makefile
## File description:
## Makefile for components
##

COMPONENTS = position \
			 velocity \
			 sprite \

all:
	@for component in $(COMPONENTS); do \
		$(MAKE) -C $$component; \
	done

clean:
	@for component in $(COMPONENTS); do \
		$(MAKE) -C $$component clean; \
	done

fclean:
	@for component in $(COMPONENTS); do \
		$(MAKE) -C $$component fclean; \
	done

re:
	@for component in $(COMPONENTS); do \
		$(MAKE) -C $$component re; \
	done

.PHONY: re clean fclean
