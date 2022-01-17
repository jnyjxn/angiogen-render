#!/bin/bash

FILES=`ls *.md`

PANDOC='/usr/bin/env pandoc'

PANDOC_PDF_OPTIONS='-fmarkdown-implicit_figures --pdf-engine=pdflatex -V geometry:margin=1in' # -V geometry:"top=2cm,bottom=1.5cm,left=1cm,right=1cm"'



for file in $FILES
do
    echo $file
    OUTPUT="`basename $file .md`.pdf"
    $PANDOC $file $PANDOC_PDF_OPTIONS  -o $OUTPUT

done
