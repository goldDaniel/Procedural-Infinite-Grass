##SRC_DIR = Src/*.cpp Src/**/*.cpp

##LINKER_FLAGS = -lSDL2 
##main:
##	g++ -g -I Src/ -Wall $(SRC_DIR) $(LINKER_FLAGS) -o run

CXX      := -g++
CXXFLAGS := -Wall  -MD -std=c++11
LDFLAGS  := -lSDL2main -lSDL2 -lSDL2_image -ldl -lassimp -lpthread -lGL -lBulletDynamics -lBulletCollision -lBulletSoftBody -lLinearMath
BUILD    := ./Build
OBJ_DIR  := $(BUILD)/objects
TARGET   := run
INCLUDE  := -ISrc/ -I /usr/local/include/bullet/
SRC      :=                      \
   $(wildcard Src/*.cpp) \
   $(wildcard Src/**/*.cpp) \
   $(wildcard Src/*.c) \
   $(wildcard Src/**/*.c) \

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<
-include $(OBJECTS:.o=.d)


$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) $(LDFLAGS) -o $(TARGET) 

.PHONY: all build clean debug release

build:
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O3 -DNDEBUG
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
