CXX=g++ 
CXXFLAGS=-std=c++11
RM=rm -f

SRCS=stuff_test.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: test

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o output/value_test $(OBJS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	$(RM) output/$(OBJS)
	

dist-clean: clean
	$(RM) *~ .depend

include .depend