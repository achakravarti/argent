###############################################################################
#		      	 __   ____   ___  ____  __ _  ____ 
#                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
#		       /    \ )   /( (_ \ ) _) /    /  )(  
#                      \_/\_/(__\_) \___/(____)\_)__) (__) 
#
# Argent Library
# Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
#
# This file is part of the Argent Library. It defines the build rules of the
# Argent Library.
#
# The contents of this file are released under the GPLv3 License. See the
# accompanying LICENSE file or the generated Developer Manual (section I:?) for 
# complete licensing details.
#
# BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
# HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
################################################################################


DIR_BLD = bld

DIR_LIB = src
SRC_LIB = $(sort $(shell find $(DIR_LIB)/ -type f -name '*.c'))
OBJ_LIB = $(patsubst $(DIR_LIB)/%.c, $(DIR_BLD)/%.o, $(SRC_LIB))
BIN_LIB = bld/libargent.so

CC = ccache gcc
CFLAGS = -fPIC -g -Wall -Wextra -I $(shell pg_config --includedir)
LDFLAGS = -shared -L $(shell pg_config --libdir) -lpq




DOC_DIR = doc
DOC_GEN = cd $(DOC_DIR) && pdflatex
DOC_BIB = cd $(DOC_DIR) && biber
DOC_VWR = xdg-open
DOC_FLG = --halt-on-error
DOC_OUT = argent-manual
DOC_TMP = *.aux *.bbl *.bcf *.blg *.log *.pdf *.synctex.gz *.xml *.toc *.loa \
	*.lof *.lot *.idx *.ilg *.ind


DIR_TEST = test
SRC_TEST = $(sort $(wildcard $(DIR_TEST)/*.c))
BIN_TEST = $(DIR_BLD)/ag-tests


$(BIN_LIB): $(OBJ_LIB) | $(DIR_BLD)
	$(LINK.c) $^ -o $@

$(BIN_TEST): $(SRC_TEST) $(BIN_LIB) | $(DIR_BLD)
	$(CC) $(CFLAGS) $^ -o $@

$(DIR_BLD)/%.o: $(DIR_LIB)/%.c | $(DIR_BLD)
	$(COMPILE.c) $^ -o $@

$(DIR_BLD):
	mkdir -p $@




all: $(BIN_LIB)

test: $(BIN_TEST)
	./$(BIN_TEST)

doc:
	$(DOC_GEN) $(DOC_FLG) $(DOC_OUT)
	$(DOC_BIB) $(DOC_OUT)
	$(DOC_GEN) $(DOC_FLG) $(DOC_OUT)
	$(DOC_VWR) $(DOC_DIR)/$(DOC_OUT).pdf &

check: $(BIN_TEST)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --log-file=$(BIN_TEST).vglog         \
		$(BIN_TEST)

clean:
	rm -rfv $(DIR_BLD) $(DOC_TMP)


.PHONY: all doc clean

