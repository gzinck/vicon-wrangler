CC=g++

LIBS_DIR=./libs
OUT_DIR=./build

# Link the dynamic libraries in the ./libs folder
CPP_FLAGS=-L$(LIBS_DIR) -l ViconDataStreamSDK_CPP -I include


adapter: ViconWrangler.cpp output
	$(CC) ViconWrangler.cpp $(CPP_FLAGS) \
		-o $(OUT_DIR)/ViconWrangler

output:
	mkdir -p $(OUT_DIR)
	cp $(LIBS_DIR)/* $(OUT_DIR)

.PHONY: clean
clean:
	rm -r $(OUT_DIR)
