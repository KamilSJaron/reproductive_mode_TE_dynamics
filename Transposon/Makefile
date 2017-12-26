CXXFLAGS = -g
 # -std=c++11 -O3 -Wall -pedantic

PRG = transposon
GIT_HEADER = include/gitversion.h
CPP_FILES = $(wildcard src/*.cpp)
# TEST_FILES = $(wildcard test/*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(CPP_FILES))
# INSTAL_PREFIX = /usr/local


$(PRG) : $(GIT_HEADER) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# .git/COMMIT_EDITMSG is a file that get updated with every commit
# .git/HEAD is a file that get updated with every switch of branches
# if any of those have changed, commit in binary have to be updated
$(GIT_HEADER) : ../.git/HEAD ../.git/COMMIT_EDITMSG
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@

# if repository is cloned, it does not contain last commit message
# to make a git header regarless if absence of the last commit message
# we create a blank file with last commit message
.git/COMMIT_EDITMSG :
	touch $@

.PHONY : clean
clean :
	rm transposon src/*.o
