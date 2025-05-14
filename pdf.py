# pdf_to_word_converter.py

import sys
from pdf2docx import Converter

def pdf_to_word(pdf_file, docx_file):
    cv = Converter(pdf_file)
    cv.convert(docx_file, start=0, end=None)
    cv.close()

if __name__ == "__main__":
    # Get file paths from command-line arguments
    if len(sys.argv) != 3:
        print("Usage: python pdf_to_word_converter.py <pdf_file> <docx_file>")
        sys.exit(1)

    pdf_file = sys.argv[1]
    docx_file = sys.argv[2]
    pdf_to_word(pdf_file, docx_file)