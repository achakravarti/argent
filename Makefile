###############################################################################
#		      	 __   ____   ___  ____  __ _  ____ 
#                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
#		       /    \ )   /( (_ \ ) _) /    /  )(  
#                      \_/\_/(__\_) \___/(____)\_)__) (__) 
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

