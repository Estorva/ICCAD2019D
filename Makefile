EXEC = exec
BIN_DIR = bin
OPT_DIR = $(BIN_DIR)/opt
DBG_DIR = $(BIN_DIR)/dbg
OPT_EXEC = $(OPT_DIR)/$(EXEC)
DBG_EXEC = $(DBG_DIR)/$(EXEC)
DBGFLAG = _DEBUG_
MAIN_CPP = src/main.cpp
MAIN_O = obj/main.o
HFILE := $(wildcard src/*.h)
CPPFILE := $(wildcard src/*.cpp) # a list of files end in .cpp
CPPFILE := $(filter-out src/test_adjacencylist.cpp,$(CPPFILE))
OFILE := $(addprefix obj/,$(addsuffix .o,$(basename $(notdir $(CPPFILE))))) # replace src/*.cpp w/ obj/*.o in CPPFILE
OFILE := $(filter-out obj/main.o,$(OFILE)) # remove main.o
CC = g++
CFLAGS = --std=c++11 -Wall -Wextra

.PHONY = all dbg clean al

all: CFLAGS += -O2
all: EXEC = $(OPT_DIR)/$(EXEC)
dbg al: CFLAGS += -g3 -O0 -ggdb $(addprefix -D, $(DBGFLAG))
dbg: EXEC = $(DBG_DIR)/$(EXEC)

all: $(OPT_EXEC)
	@# Link the exec from under bin to here
	@ln -sf $< .

dbg: $(DBG_EXEC)
	@ln -sf $< .

$(OPT_EXEC): $(MAIN_CPP) $(OFILE) | $(OPT_DIR)
	$(CC) $(CFLAGS) -c $(MAIN_CPP) -o $(MAIN_O)
	$(CC) $(CFLAGS) $(MAIN_O) $(OFILE) -o $@

$(DBG_EXEC): $(MAIN_CPP) $(OFILE) | $(DBG_DIR)
	$(CC) $(CFLAGS) -c $(MAIN_CPP) -o $(MAIN_O)
	$(CC) $(CFLAGS) $(MAIN_O) $(OFILE) -o $@

obj/%.o: src/%.cpp src/%.h | obj
	$(CC) $(CFLAGS) -c $< -o $@

al: $(DBG_DIR)/al
	@ln -sf $< .

$(DBG_DIR)/al: src/test_adjacencylist.cpp $(HFILE) | $(DBG_DIR)
	$(CC) $(CFLAGS) -c src/test_adjacencylist.cpp -o obj/main_al.o
	$(CC) $(CFLAGS) obj/main_al.o obj/adjacencylist.o -o $(DBG_DIR)/al

### Create folders
$(OPT_DIR): | $(BIN_DIR)
	mkdir $(OPT_DIR)

$(DBG_DIR): | $(BIN_DIR)
	mkdir $(DBG_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

obj:
	mkdir obj

clean:
	@# removes all *.o files and binary files under bin/
	rm -rf obj bin
