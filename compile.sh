gcc -Ofast -frename-registers -funroll-loops src/main.c src/FastPrintF.c src/GeneralFunctions.c src/NGramLibrary.c src/PrefixTree.c src/Printer.c src/Trie.c src/WordLibrary.c -o src/superNGramSolverButCurrentlyLinear