#make file for lab3
CXX := g++
CXXFLAGS := -I ./inc -std=c++11
LINKFLAGS := -pedantic -fomit-frame-pointer -funroll-all-loops -O3 -g

GREEN := '\033[0;32m'
NO_COLOR='\033[0m'

SRCDIR := src
OBJDIR := obj
INCDIR := inc
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRCS))

TESTDIR := testcase
OUTPUTDIR := result
VISUALDIR := visualize

.PHONY: all clean test1 draw

TARGET = Lab1

all: $(TARGET)

$(OBJDIR):
	@mkdir $(OBJDIR)

Lab1: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) $^ -o $@

-include $(DEPS)

#these %.o will be run when we use the OBJS, then it will find the src.cpp and use it
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) -MMD -c $< -o $@

test_w_d:
	./Lab1 ./lab1testcase/testcase/case0.txt ./lab1testcase/testcase/output0.txt $(VISUALDIR)/draw0.txt
	./Lab1 ./lab1testcase/testcase/case1.txt ./lab1testcase/testcase/output1.txt $(VISUALDIR)/draw1.txt
	./Lab1 ./lab1testcase/testcase/case2.txt ./lab1testcase/testcase/output2.txt $(VISUALDIR)/draw2.txt
	./Lab1 ./lab1testcase/testcase/case3.txt ./lab1testcase/testcase/output3.txt $(VISUALDIR)/draw3.txt
	./Lab1 ./lab1testcase/testcase/case4.txt ./lab1testcase/testcase/output4.txt $(VISUALDIR)/draw4.txt
	./Lab1 ./lab1testcase/testcase/case5.txt ./lab1testcase/testcase/output5.txt $(VISUALDIR)/draw5.txt
	./Lab1 ./lab1testcase/testcase/case6.txt ./lab1testcase/testcase/output6.txt $(VISUALDIR)/draw6.txt
	./Lab1 ./lab1testcase/testcase/case7.txt ./lab1testcase/testcase/output7.txt $(VISUALDIR)/draw7.txt

test:
	./Lab1 ./lab1testcase/testcase/case0.txt ./lab1testcase/testcase/output0.txt
	./Lab1 ./lab1testcase/testcase/case1.txt ./lab1testcase/testcase/output1.txt
	./Lab1 ./lab1testcase/testcase/case2.txt ./lab1testcase/testcase/output2.txt
	./Lab1 ./lab1testcase/testcase/case3.txt ./lab1testcase/testcase/output3.txt
	./Lab1 ./lab1testcase/testcase/case4.txt ./lab1testcase/testcase/output4.txt
	./Lab1 ./lab1testcase/testcase/case5.txt ./lab1testcase/testcase/output5.txt
	./Lab1 ./lab1testcase/testcase/case6.txt ./lab1testcase/testcase/output6.txt
	./Lab1 ./lab1testcase/testcase/case7.txt ./lab1testcase/testcase/output7.txt

test5:
	./Lab1 ./lab1testcase/testcase/case5.txt ./lab1testcase/testcase/output5.txt $(VISUALDIR)/draw5.txt

draw5:
	python3 draw_block_layout.py $(VISUALDIR)/draw5.txt $(VISUALDIR)/output5.png

check4:
	diff ./lab1testcase/testcase/output4.txt ./lab1testcase/golden/output4.txt


check:
	diff ./lab1testcase/testcase/output0.txt ./lab1testcase/golden/output0.txt
	diff ./lab1testcase/testcase/output1.txt ./lab1testcase/golden/output1.txt
	diff ./lab1testcase/testcase/output2.txt ./lab1testcase/golden/output2.txt
	diff ./lab1testcase/testcase/output3.txt ./lab1testcase/golden/output3.txt
	diff ./lab1testcase/testcase/output4.txt ./lab1testcase/golden/output4.txt
	diff ./lab1testcase/testcase/output5.txt ./lab1testcase/golden/output5.txt
	diff ./lab1testcase/testcase/output6.txt ./lab1testcase/golden/output6.txt
	diff ./lab1testcase/testcase/output7.txt ./lab1testcase/golden/output7.txt

draw: | $(VISUALDIR)
	python3 draw_block_layout.py $(VISUALDIR)/draw0.txt $(VISUALDIR)/output0.png
	python3 draw_block_layout.py $(VISUALDIR)/draw1.txt $(VISUALDIR)/output1.png
	python3 draw_block_layout.py $(VISUALDIR)/draw2.txt $(VISUALDIR)/output2.png
	python3 draw_block_layout.py $(VISUALDIR)/draw3.txt $(VISUALDIR)/output3.png
	python3 draw_block_layout.py $(VISUALDIR)/draw4.txt $(VISUALDIR)/output4.png
	python3 draw_block_layout.py $(VISUALDIR)/draw6.txt $(VISUALDIR)/output6.png
	python3 draw_block_layout.py $(VISUALDIR)/draw7.txt $(VISUALDIR)/output7.png

$(VISUALDIR):
	@mkdir $(VISUALDIR)

110511210: 
	@mkdir 110511210

tar: | 110511210
	@cp -r src ./110511210
	@cp -r inc ./110511210
	@cp main.cpp ./110511210
	@cp Makefile ./110511210
	tar -cvf 110511210.tar 110511210

clean:
	rm -rf $(OBJDIR) $(TARGET) $(VISUALDIR) ./110511210 110511210.tar
