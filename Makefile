CC=g++

LIBS_DIR=./libs
OUT_DIR=./build
OBJ_DIR=./obj

CPP_FLAGS=-std=c++11 -I include
# Link the dynamic libraries in the ./libs folder
LINK_FLAGS=-L$(LIBS_DIR) -l ViconDataStreamSDK_CPP

.PHONY: all
all: $(OUT_DIR)/ViconWrangler

$(OUT_DIR)/ViconWrangler: $(OUT_DIR) $(OBJ_DIR)/Server.o $(OBJ_DIR)/ViconWrangler.o
	$(CC) $(OBJ_DIR)/ViconWrangler.o \
		$(OBJ_DIR)/Server.o \
		$(CPP_FLAGS) $(LINK_FLAGS) \
		-o $(OUT_DIR)/ViconWrangler

$(OBJ_DIR)/Server.o: Server.cpp $(OBJ_DIR)
	$(CC) Server.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/Server.o

$(OBJ_DIR)/ViconWrangler.o: ViconWrangler.cpp $(OBJ_DIR)
	$(CC) ViconWrangler.cpp $(CPP_FLAGS) \
		-c -o $(OBJ_DIR)/ViconWrangler.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)
	cp $(LIBS_DIR)/* $(OUT_DIR)

.PHONY: clean
clean:
	rm -r $(OUT_DIR)
