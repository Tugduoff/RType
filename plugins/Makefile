##
## EPITECH PROJECT, 2024
## RType Plugins Makefile
## File description:
## Makefile for the Plugins compilation
##

all:
	$(MAKE) -C components
	$(MAKE) -C systems

clean:
	$(MAKE) -C components clean
	$(MAKE) -C systems clean

fclean:
	$(MAKE) -C components fclean
	$(MAKE) -C systems fclean
	rm -f bin/components/*.so
	rm -f bin/systems/*.so

re:
	$(MAKE) -C components re
	$(MAKE) -C systems re

.PHONY: re clean fclean
