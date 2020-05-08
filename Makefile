DOC_GEN = pdflatex
DOC_VWR = xdg-open
DOC_FLG = --halt-on-error
DOC_BIB = biber
DOC_OUT = argent-manual
DOC_TMP = *.aux *.bbl *.bcf *.blg *.log *.pdf *.synctex.gz *.xml


doc:
	$(DOC_GEN) $(DOC_FLG) $(DOC_OUT)
	$(DOC_BIB) $(DOC_OUT)
	$(DOC_GEN) $(DOC_FLG) $(DOC_OUT)
	$(DOC_VWR) $(DOC_OUT).pdf &


clean:
	rm -fv $(DOC_TMP)


.PHONY: doc clean

