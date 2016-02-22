CXX=g++ 
CXXFLAGS=-std=c++11 -Ideps -I.
RM=rm -f
RM_RECURSIVE=rm -rf
MKDIR=mkdir

CPP_FILES=stuff_test.cpp  Value/Value.cpp
SRC_FILES=$(CPP_FILES)
OBJS=$(subst .cpp,.o,$(CPP_FILES))

NODEPS=clean dist-clean clean-all deps

all: link

link: $(OBJS)
	$(CXX) $(CXXFLAGS) -o output/value_test $(OBJS)

run: link
	./output/value_test

deps:
	git clone --branch v1.1.0 https://github.com/mapbox/variant.git deps/variant


depend: .depend

.depend: $(SRC_FILES)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	$(RM) output/*
	$(RM) $(wildcard *.o) 
	$(RM) $(wildcard Value/*.o)
	$(RM_RECURSIVE) obj

clean-all: clean
	$(RM_RECURSIVE) deps
	

dist-clean: clean-all
	$(RM) *~ .depend

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include .depend
endif
