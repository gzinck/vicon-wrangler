CC=g++

LIBS_DIR=./libs
INCLUDE_DIR=./include
OUT_DIR=./build
OBJ_DIR=./obj

_OPENCV_CPP_FLAGS=`pkg-config opencv4 --cflags`
_OPENCV_LINK_FLAGS=`pkg-config opencv4 --libs`

# Include vicon and websocketpp when compiling
CPP_FLAGS=-std=c++11 -I$(INCLUDE_DIR)/vicon \
		  -I$(INCLUDE_DIR)/websocketpp \
		  -I$(INCLUDE_DIR)/asio/asio/include \
		  -I$(INCLUDE_DIR)/jsoncpp/dist \
		  $(_OPENCV_CPP_FLAGS)

# Link the dynamic libraries in the ./libs folder
LINK_FLAGS=-L$(LIBS_DIR) -lViconDataStreamSDK_CPP $(_OPENCV_LINK_FLAGS)

.PHONY: all
all: $(OUT_DIR)/ViconWrangler

.PHONY: deps
deps:
	brew install opencv

$(OUT_DIR)/ViconWrangler: $(OBJ_DIR)/Server.o $(OBJ_DIR)/ViconWrangler.o $(OBJ_DIR)/MessageHandler.o $(OBJ_DIR)/jsoncpp.o $(OBJ_DIR)/CoordinateConverter.o | $(OUT_DIR)
	$(CC) $(OBJ_DIR)/ViconWrangler.o \
		$(OBJ_DIR)/Server.o \
		$(OBJ_DIR)/jsoncpp.o \
		$(OBJ_DIR)/MessageHandler.o \
		$(OBJ_DIR)/CoordinateConverter.o \
		$(CPP_FLAGS) $(LINK_FLAGS) \
		-o $(OUT_DIR)/ViconWrangler

$(OBJ_DIR)/ViconWrangler.o: ViconWrangler.cpp Server.h $(INCLUDE_DIR)/jsoncpp | $(OBJ_DIR)
	$(CC) ViconWrangler.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/ViconWrangler.o

$(OBJ_DIR)/Server.o: Server.h Server.cpp MessageHandler.h $(INCLUDE_DIR)/asio $(INCLUDE_DIR)/websocketpp $(INCLUDE_DIR)/jsoncpp | $(OBJ_DIR)
	$(CC) Server.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/Server.o

$(OBJ_DIR)/MessageHandler.o: MessageHandler.cpp MessageHandler.h CoordinateConverter.h $(INCLUDE_DIR)/jsoncpp | $(OBJ_DIR)
	$(CC) MessageHandler.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/MessageHandler.o

$(OBJ_DIR)/CoordinateConverter.o: CoordinateConverter.cpp CoordinateConverter.h | $(OBJ_DIR)
	$(CC) CoordinateConverter.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/CoordinateConverter.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)
	cp $(LIBS_DIR)/vicon/* $(OUT_DIR)

$(OBJ_DIR)/jsoncpp.o: $(INCLUDE_DIR)/jsoncpp $(INCLUDE_DIR)/jsoncpp/dist/jsoncpp.cpp
	$(CC) $(INCLUDE_DIR)/jsoncpp/dist/jsoncpp.cpp \
		$(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/jsoncpp.o

$(INCLUDE_DIR)/jsoncpp:
	cd $(INCLUDE_DIR) && git clone https://github.com/open-source-parsers/jsoncpp.git
	cd $(INCLUDE_DIR)/jsoncpp && python amalgamate.py

$(INCLUDE_DIR)/websocketpp:
	cd $(INCLUDE_DIR) && git clone https://github.com/zaphoyd/websocketpp.git

$(INCLUDE_DIR)/asio:
	cd $(INCLUDE_DIR) && git clone https://github.com/chriskohlhoff/asio.git

.PHONY: clean
clean:
	rm -r $(OUT_DIR)
