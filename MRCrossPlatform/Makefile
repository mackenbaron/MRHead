CC=g++ 
CFLAGS= -c -DLINUX -std=c++11 
BIN=../../bin
EXE=./MRCrossPlatform
CURDIR=$(shell pwd)

INC_CODE=-I./
INC=-Iutility -I/usr/local/include $(INC_CODE) -I/usr/local/include

LIBFLAG=-L/usr/local/lib  \
	-lopencv_videoio -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_imgcodecs 
#-pthread-lopenblas  -lboost_filesystem  -lglog -lboost_system

SRC_OBJ := $(patsubst %.cpp,%.o,$(wildcard MRCrossPlatform.cpp))
SRC_BIN  := $(patsubst %.cpp,%,$(wildcard MRCrossPlatform.cpp))

.PHONY:all
all: list Obj bin

.PHONY:list
list:
	@echo "=========== List Obj ============"
	@echo $(SRC_BIN1)
	@echo "================================="

.PHONY:Obj
Obj: $(SRC_OBJ)
$(SRC_OBJ): %.o :%.cpp
	$(CC) $(CFLAGS) $(INC) $< -o $@

.PHONY:bin
bin:$(EXE)
$(EXE):$(SRC_OBJ)
	$(CC) $(INC)  $^ -o $@  $(LIBFLAG)
	rm -f $^

.PHONY:clean
clean:
	rm -f *.o $(EXE)
