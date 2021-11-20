INC_DIR :=  ./Include/
SRC_DIR = ./src/
#EXE_DIR := ./bin
CC := g++
CPPFLAGS := -g -Wno-deprecated
LIBS := -lpthread
INC_FILE := $(addprefix -I, $(INC_DIR))
CPP_FILE :=  $(wildcard ./src/*.cc)
OBJS := $(patsubst %.cc, %.o, $(CPP_FILE))
# objs:=$(addsuffix /*.o, $(SRC_DIR))
TARGET := server.exe
$(TARGET):$(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(INC_FILE) $(LIBS)
%.o:%.cc
	$(CC) $(CPPFLAGS) -o $@ -c $< $(INC_FILE) $(LIBS)
object:
	@echo $(OBJS)
clean:
	rm -rf $(TARGET) $(OBJS)
