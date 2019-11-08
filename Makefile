CC:=g++
ifneq (,$(findstring Darwin,$(shell uname)))
	exist = $(shell if [ -e '/usr/local/bin/g++-9' ]; then echo "exist"; else echo "notexist"; fi;)
	ifeq ($(exist),exist)
		CC:=g++-9
	else
		CC:=g++-8
	endif
endif
OMPFLG=-fopenmp
HASHFLG=-Wno-deprecated
BUILDFLG=-w -ffunction-sections -fdata-sections -fmodulo-sched -msse
EXE_CLT=bin/meta-apo-train
EXE_CLB=bin/meta-apo-calibrate

all:
	$(CC) -o $(EXE_CLT) src/key_calibrate_train.cpp $(HASHFLG) $(BUILDFLG) $(OMPFLG)
	$(CC) -o $(EXE_CLB) src/key_calibrate.cpp $(HASHFLG) $(BUILDFLG) $(OMPFLG)

clean:
	rm -rf bin/* src/*.o
