CXX=g++ 
CXXFLAGS=-std=c++11 -Ideps -I.
RM=rm -f
RM_RECURSIVE=rm -rf
MKDIR=mkdir

SRCS=stuff_test.cpp 
OBJS=$(subst .cpp,.o,$(SRCS))

NODEPS=clean all dist-clean clean

all: test

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o output/value_test $(OBJS)

run: test
	./output/value_test

deps:
	git clone --branch v1.1.0 https://github.com/mapbox/variant.git deps/variant


depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	$(RM) output/*
	$(RM) $(wildcard *.o)
	$(RM_RECURSIVE) obj
	$(RM_RECURSIVE) deps
	

dist-clean: clean
	$(RM) *~ .depend

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include .depend
endif
