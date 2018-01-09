CXXFLAGS = -g
 # -std=c++11 -O3 -Wall -pedantic

PRG = TEAscus
GIT_HEADER = include/gitversion.h
CPP_FILES = $(wildcard src/[A-Z]*.cpp)
CPP_TEST_CLASSES = $(wildcard test/[A-Z]*.cpp)
# TEST_FILES = $(wildcard test/*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(CPP_FILES))
OBJ_TESTS = $(patsubst %.cpp, %.o, $(CPP_TEST_CLASSES))
ifndef INSTAL_PREFIX
    INSTAL_PREFIX = /usr/local
endif

# BUILDING

$(PRG) : src/main.o $(GIT_HEADER) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJ)

%.o : %.cpp $(GIT_HEADER)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# .git/COMMIT_EDITMSG is a file that get updated with every commit
# .git/HEAD is a file that get updated with every switch of branches
# if any of those have changed, commit in binary have to be updated
$(GIT_HEADER) : ../.git/HEAD ../.git/COMMIT_EDITMSG
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@

# if repository is cloned, it does not contain last commit message
# to make a git header regarless if absence of the last commit message
# we create a blank file with last commit message
../.git/COMMIT_EDITMSG :
	touch $@

# TESTING

TEAscus_test : test/TEAscus_test.o $(OBJ) $(OBJ_TESTS)
	gcc -o $@ $^ -lstdc++ -lcppunit -ldl

.PHONY : test
test : TEAscus_test
	./$< && rm $<

# INSTALL

.PHONY : install
install : $(PRG)
	install -C $< $(INSTAL_PREFIX)/bin

# CLEANING
.PHONY : clean
clean :
	rm transposon src/*.o test/*.o
