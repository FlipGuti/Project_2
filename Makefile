CC=gcc
CXX=g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc

CXXFLAGS = -std=c++17 -I $(INC_DIR)
LDFLAGS = -lgtest -lpthread

all: directories runtests

runtests: $(BIN_DIR)/StringDataSinkTest $(BIN_DIR)/StringDataSourceTest
    ./$(BIN_DIR)/StringDataSinkTest
    ./$(BIN_DIR)/StringDataSourceTest


# make DSVReader.cpp
# make DSVWriter.cpp
# make StringUtils.cpp
# make StringDataSink.cpp
# make StringDataSource.cpp

# run StringDataSinkTest.cpp
# run StringDataSourceTest



clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

