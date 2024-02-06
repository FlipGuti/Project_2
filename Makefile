CC=gcc
CXX=g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc

CXXFLAGS = -std=c++17 -I$(INC_DIR)
LDFLAGS = -lgtest -lpthread -lexpat

# Phony targets for building and cleaning the project
.PHONY: all directories runtests clean

all: directories runtests

runtests: $(TEST_BINS)
	@for test_bin in $^ ; do \
		./$$test_bin ; \
	done

# Source and Object Files
SRC_FILES = StringUtils.cpp StringDataSink.cpp StringDataSource.cpp DSVReader.cpp DSVWriter.cpp XMLReader.cpp XMLWriter.cpp
OBJ_FILES = $(SRC_FILES:%.cpp=$(OBJ_DIR)/%.o)


# Test Source and Object Files
TEST_SRC_FILES = StringUtilsTest.cpp StringDataSinkTest.cpp StringDataSourceTest.cpp DSVTest.cpp XMLTest.cpp
TEST_OBJ_FILES = $(TEST_SRC_FILES:%.cpp=$(OBJ_DIR)/%.o)
TEST_BINS = $(TEST_SRC_FILES:%.cpp=$(BIN_DIR)/%)


# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Compile test source files to object files, then link to create test binaries
$(BIN_DIR)/%: $(TEST_SRC_DIR)/%.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $< $(OBJ_FILES) -o $@ $(LDFLAGS)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)