CXX=g++ 
CXXFLAGS=-std=c++11 -Ideps -I.
RM=rm -f
RM_RECURSIVE=rm -rf
MKDIR=mkdir

SRCS=stuff_test.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: deps test

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o output/value_test $(OBJS)

deps:
	git clone --branch v1.1.0 https://github.com/mapbox/variant.git deps/variant


depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	$(RM) output/$(OBJS)
	$(RM_RECURSIVE) deps
	

dist-clean: clean
	$(RM) *~ .depend

include .depend