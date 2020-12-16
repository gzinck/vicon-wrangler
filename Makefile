CC=g++

LIBS_DIR=./libs
INCLUDE_DIR=./include
OUT_DIR=./build
OBJ_DIR=./obj

# Include vicon and websocketpp when compiling
CPP_FLAGS=-std=c++11 -I $(INCLUDE_DIR)/vicon \
		  -I $(INCLUDE_DIR)/websocketpp \
		  -I $(INCLUDE_DIR)/asio/asio/include

# Link the dynamic libraries in the ./libs folder
LINK_FLAGS=-L$(LIBS_DIR) -l ViconDataStreamSDK_CPP

.PHONY: all
all: $(OUT_DIR)/ViconWrangler

$(OUT_DIR)/ViconWrangler: $(OBJ_DIR)/Server.o $(OBJ_DIR)/ViconWrangler.o | $(OUT_DIR)
	$(CC) $(OBJ_DIR)/ViconWrangler.o \
		$(OBJ_DIR)/Server.o \
		$(CPP_FLAGS) $(LINK_FLAGS) \
		-o $(OUT_DIR)/ViconWrangler

$(OBJ_DIR)/Server.o: Server.h Server.cpp $(INCLUDE_DIR)/asio $(INCLUDE_DIR)/websocketpp | $(OBJ_DIR)
	$(CC) Server.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/Server.o

$(OBJ_DIR)/ViconWrangler.o: Server.h ViconWrangler.cpp | $(OBJ_DIR)
	$(CC) ViconWrangler.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/ViconWrangler.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)
	cp $(LIBS_DIR)/vicon/* $(OUT_DIR)

$(INCLUDE_DIR)/websocketpp:
	cd $(INCLUDE_DIR) && git clone https://github.com/zaphoyd/websocketpp.git

$(INCLUDE_DIR)/asio:
	cd $(INCLUDE_DIR) && git clone https://github.com/chriskohlhoff/asio.git

.PHONY: clean
clean:
	rm -r $(OUT_DIR)
