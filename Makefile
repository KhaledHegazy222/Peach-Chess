
TARGET := main.exe


SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin
DEP_DIR := dep



EXE := $(BIN_DIR)/$(TARGET)

SRC := $(wildcard $(SRC_DIR)/*.cpp) 
INC := $(wildcard $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/*.h) 
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
DEP := $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRC))

INC_PATH 	:= C:\SDL2\Mingw\SDL\x86\include \
			C:\SDL2\Mingw\SDL\x86\include\SDL2 \
 			C:\SDL2\Mingw\SDL2_image\x86\include \
 			C:\SDL2\Mingw\SDL2_image\x86\include\SDL2
LIB_DIR 	:= 	C:\SDL2\Mingw\SDL\x86\lib \
 			C:\SDL2\Mingw\SDL2_image\x86\lib
LIB := mingw32 SDL2main SDL2 SDL2_image

INC_OPT 		:= $(addprefix -I,$(INC_PATH)) $(addprefix -I,$(INC_DIR)) 
LIB_PATH_OPT 	:= $(addprefix -L,$(LIB_DIR))
LIB_OPT 		:= $(addprefix -l,$(LIB))

EXE := $(BIN_DIR)\$(TARGET)

CXX := g++
CXXFLAGS = -w -g -MMD -MP -MF $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$<) -std=c++14
DEF := 
DEF_OPT := $(addprefix -D:,$(DEF))

.PHONY: all build clean 

all : build 
	

build: $(OBJ) $(EXE)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp  
	$(CXX) $(CXXFLAGS) $(-DEF_OPT) $(INC_OPT) -c $< -o $@ 

$(EXE) : $(OBJ)
	$(CXX) -g $^ -o $@ $(LIB_PATH_OPT) $(LIB_OPT) 
	 


clean: 
	rm -f $(OBJ_DIR)/*.o
	rm -f $(DEP_DIR)/*.d
	rm -f $(BIN_DIR)/*.exe

-include $(DEP)