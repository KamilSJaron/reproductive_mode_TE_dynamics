GIT_HEADER = gitversion.h
CPP_FILES = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(CPP_FILES))

../Transposon : $(OBJ)
	g++ -g $^ -o $@

%.o: %.cpp $(GIT_HEADER)
	g++ -g -c $<

$(GIT_HEADER) : ../../.git/HEAD ../../.git/COMMIT_EDITMSG
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@

clean:
	rm *.o test
