CC:=g++
HASHFLG=-Wno-deprecated
BUILDFLG=-w -ffunction-sections -fdata-sections -fmodulo-sched -msse
EXE_CLT=bin/meta-apo-train
EXE_CLB=bin/meta-apo-calibrate

all:
	$(CC) -o $(EXE_CLT) src/key_calibrate_train.cpp $(HASHFLG) $(BUILDFLG)
	$(CC) -o $(EXE_CLB) src/key_calibrate.cpp $(HASHFLG) $(BUILDFLG)

clean:
	rm -rf bin/* src/*.o
