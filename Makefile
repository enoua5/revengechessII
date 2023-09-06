BDIR = build/
NDIR = $(BDIR)native/
WDIR = $(BDIR)web/
SRVDIR = $(BDIR)server/
EXE = $(NDIR)revengechess
JSEXE = $(WDIR)revengechess.js
SERVER_NAME = rc2serve
SERVER_EXE = $(SRVDIR)$(SERVER_NAME)
NCC = g++
WCC = em++
INCLUDE = -I inc/ -I third_party/nlohmann_json/include/ -I third_party/websocketpp/ -I third_party/asio/asio/include
NOPTIONS = -Wall -Wextra -pedantic -g -Ofast
SOPTIONS = -Wall -Wextra -pedantic -g -pthread --std=c++2a
SLIB = -lssl -lcrypto
SDEFS =
WOPTIONS = -O3 --bind --no-entry -s ASSERTIONS=1
MEMCHECK = valgrind --tool=memcheck --leak-check=yes --show-reachable=yes

all: native web server

native: $(EXE)
	
web: js_exe webui

js_exe: $(JSEXE)

webui: $(WDIR)index.html $(WDIR)style.css $(WDIR)img/pieces/* $(WDIR)img/*.png $(WDIR)favicon.ico $(WDIR)src/board.js $(WDIR)src/infopane.js $(WDIR)src/util.js $(WDIR)engine.worker.js $(WDIR)src/ai.js $(WDIR)src/client.js $(WDIR)src/tutorials.data.js

test: $(EXE)
	$(EXE)

memcheck: $(EXE)
	$(MEMCHECK) $(EXE)
	
test_server: server
	(cd $(SRVDIR); ./$(SERVER_NAME))
	
memcheck_server: server
	(cd $(SRVDIR); $(MEMCHECK) ./$(SERVER_NAME))
	
server_tls: SDEFS += -DRC_SERV_USE_TLS
server_tls: server

server: server_exe server_conf

server_exe_tls: SDEFS += -DRC_SERV_USE_TLS
server_exe_tls: server_exe

server_exe: $(SERVER_EXE)

$(SERVER_EXE): src/server_main.cpp src/version.cpp src/game/* src/server/*.cpp inc/*
	mkdir -p $(SRVDIR)
	$(NCC) $(SOPTIONS) -o $(SERVER_EXE) $(INCLUDE) $(SDEFS) src/server_main.cpp src/game/*.cpp src/server/*.cpp src/version.cpp $(SLIB)

server_conf: $(SRVDIR)conf.json

$(SRVDIR)conf.json: src/server/default_conf.json
	mkdir -p $(SRVDIR)
	cp src/server/default_conf.json $(SRVDIR)conf.json
	
clean:
	rm -r $(BDIR)
	
$(EXE): src/native_main.cpp src/version.cpp src/engine/* src/game/* src/ui/* inc/*
	mkdir -p $(NDIR)
	$(NCC) $(NOPTIONS) -o $(EXE) $(INCLUDE) src/native_main.cpp src/game/*.cpp src/ui/*.cpp src/engine/*.cpp src/version.cpp
	
$(JSEXE): src/emcc_bindings.cpp src/version.cpp src/engine/* src/game/* inc/*
	mkdir -p $(WDIR)
	$(WCC) $(WOPTIONS) -o $(JSEXE) $(INCLUDE) src/game/*.cpp src/engine/*.cpp src/emcc_bindings.cpp src/version.cpp
	
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
	
$(WDIR)img/*.png: img/*.png
	mkdir -p $(WDIR)img
	cp img/*.png $(WDIR)img
	
$(WDIR)src/board.js: src/web/src/board.js
	mkdir -p $(WDIR)src
	cp src/web/src/board.js $(WDIR)src/
	
$(WDIR)src/infopane.js: src/web/src/infopane.js
	mkdir -p $(WDIR)src
	cp src/web/src/infopane.js $(WDIR)src/
	
$(WDIR)src/util.js: src/web/src/util.js
	mkdir -p $(WDIR)src
	cp src/web/src/util.js $(WDIR)src/

$(WDIR)engine.worker.js: src/web/engine.worker.js
	mkdir -p $(WDIR)src
	cp src/web/engine.worker.js $(WDIR)

$(WDIR)src/ai.js: src/web/src/ai.js
	mkdir -p $(WDIR)src
	cp src/web/src/ai.js $(WDIR)src/
	
$(WDIR)src/client.js: src/web/src/client.js
	mkdir -p $(WDIR)src
	cp src/web/src/client.js $(WDIR)src/

$(WDIR)src/tutorials.data.js: src/web/src/tutorials.data.js
	mkdir -p $(WDIR)src
	cp src/web/src/tutorials.data.js $(WDIR)src/
