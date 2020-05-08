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
# accompanying LICENSE file or the generated Argent manual (section I:?) for 
# complete licensing details.
#
# BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
# HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
################################################################################


DOC_DIR = doc
DOC_GEN = cd $(DOC_DIR) && pdflatex
DOC_BIB = cd $(DOC_DIR) && biber
DOC_VWR = xdg-open
DOC_FLG = --halt-on-error
DOC_OUT = argent-manual
DOC_TMP = *.aux *.bbl *.bcf *.blg *.log *.pdf *.synctex.gz *.xml


doc:
	$(DOC_GEN) $(DOC_FLG) $(DOC_OUT)
	$(DOC_BIB) $(DOC_OUT)
	$(DOC_GEN) $(DOC_FLG) $(DOC_OUT)
	$(DOC_VWR) $(DOC_DIR)/$(DOC_OUT).pdf &


clean:
	rm -fv $(DOC_TMP)


.PHONY: doc clean

