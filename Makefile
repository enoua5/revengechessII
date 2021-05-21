BDIR = build/
NDIR = $(BDIR)native/
WDIR = $(BDIR)web/
EXE = $(NDIR)revengechess
JSEXE = $(WDIR)revengechess.js
NCC = g++
WCC = em++
NOPTIONS = -Wall -Wextra -pedantic -g -Ofast
WOPTIONS = -O3 -s WASM=1 --bind --no-entry
MEMCHECK = valgrind --tool=memcheck --leak-check=yes --show-reachable=yes

all: $(EXE) $(JSEXE)

native: $(EXE)
	
web: $(JSEXE) webui

webui: $(WDIR)index.html $(WDIR)style.css $(WDIR)img/pieces/* $(WDIR)favicon.ico $(WDIR)src/board.js $(WDIR)src/infopane.js $(WDIR)src/util.js

test: $(EXE)
	$(EXE)

memcheck: $(EXE)
	$(MEMCHECK) $(EXE)
	
clean:
	rm -r $(BDIR)
	
$(EXE): src/native_main.cpp src/version.cpp src/engine/* src/game/* src/ui/* inc/*
	mkdir -p $(NDIR)
	$(NCC) $(NOPTIONS) -o $(EXE) -I inc/ src/native_main.cpp src/game/*.cpp src/ui/*.cpp src/engine/*.cpp src/version.cpp
	
$(JSEXE): src/emcc_bindings.cpp src/version.cpp src/engine/* src/game/* inc/*
	mkdir -p $(WDIR)
	$(WCC) $(WOPTIONS) -o $(JSEXE) -I inc/ src/game/*.cpp src/engine/*.cpp src/emcc_bindings.cpp src/version.cpp
	
$(WDIR)index.html: src/web/index.html
	mkdir -p $(WDIR)
	cp src/web/index.html $(WDIR)index.html
	
$(WDIR)style.css: src/web/style.css
	mkdir -p $(WDIR)
	cp src/web/style.css $(WDIR)style.css
	
$(WDIR)img/pieces/*: img/pieces/*
	mkdir -p $(WDIR)img/pieces
	cp img/pieces/* $(WDIR)img/pieces
	
$(WDIR)favicon.ico: img/favicon.ico
	mkdir -p $(WDIR)img
	cp img/favicon.ico $(WDIR)favicon.ico
	
$(WDIR)src/board.js: src/web/src/board.js
	mkdir -p $(WDIR)src
	cp src/web/src/board.js $(WDIR)src/
	
$(WDIR)src/infopane.js: src/web/src/infopane.js
	mkdir -p $(WDIR)src
	cp src/web/src/infopane.js $(WDIR)src/
	
$(WDIR)src/util.js: src/web/src/util.js
	mkdir -p $(WDIR)src
	cp src/web/src/util.js $(WDIR)src/
	
