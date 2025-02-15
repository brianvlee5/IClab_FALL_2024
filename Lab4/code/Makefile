#make file for lab3
CXX := g++
CXXFLAGS := -I ./inc -std=c++11
LINKFLAGS := -pedantic -fomit-frame-pointer -funroll-all-loops -O3 

GREEN := '\033[0;32m'
NO_COLOR='\033[0m'

SRCDIR := src
OBJDIR := obj
INCDIR := inc
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRCS))

TESTDIR := publicCase
OUTPUTDIR := result
VISUALDIR := visualize

.PHONY: all clean test1 test2 test3 check1 check2 check3 draw

TARGET = D2DGRter

all: $(TARGET)

$(OBJDIR):
	@mkdir $(OBJDIR)

D2DGRter: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) $^ -o $@

-include $(DEPS)

#these %.o will be run when we use the OBJS, then it will find the src.cpp and use it
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) -MMD -c $< -o $@

test:
	@./D2DGRter $(TESTDIR)/testcase0/testcase0.gmp $(TESTDIR)/testcase0/testcase0.gcl $(TESTDIR)/testcase0/testcase0.cst $(TESTDIR)/testcase0/testcase0.lg $(OUTPUTDIR)/testcase0.drawlg
	@./D2DGRter $(TESTDIR)/testcase1/testcase1.gmp $(TESTDIR)/testcase1/testcase1.gcl $(TESTDIR)/testcase1/testcase1.cst $(TESTDIR)/testcase1/testcase1.lg $(OUTPUTDIR)/testcase1.drawlg
	@./D2DGRter $(TESTDIR)/testcase2/testcase2.gmp $(TESTDIR)/testcase2/testcase2.gcl $(TESTDIR)/testcase2/testcase2.cst $(TESTDIR)/testcase2/testcase2.lg $(OUTPUTDIR)/testcase2.drawlg
	@./D2DGRter $(TESTDIR)/testcase3/testcase3.gmp $(TESTDIR)/testcase3/testcase3.gcl $(TESTDIR)/testcase3/testcase3.cst $(TESTDIR)/testcase3/testcase3.lg $(OUTPUTDIR)/testcase3.drawlg
	@./D2DGRter $(TESTDIR)/testcase4/testcase4.gmp $(TESTDIR)/testcase4/testcase4.gcl $(TESTDIR)/testcase4/testcase4.cst $(TESTDIR)/testcase4/testcase4.lg $(OUTPUTDIR)/testcase4.drawlg

check:
	@./Evaluator $(TESTDIR)/testcase0 testcase0
	@./Evaluator $(TESTDIR)/testcase1 testcase1
	@./Evaluator $(TESTDIR)/testcase2 testcase2
	@./Evaluator $(TESTDIR)/testcase3 testcase3
	@./Evaluator $(TESTDIR)/testcase4 testcase4

test1: | $(OUTPUTDIR)
	@./D2DGRter $(TESTDIR)/testcase0/testcase0.gmp $(TESTDIR)/testcase0/testcase0.gcl $(TESTDIR)/testcase0/testcase0.cst $(TESTDIR)/testcase0/testcase0.lg $(OUTPUTDIR)/testcase0.drawlg

test2: | $(OUTPUTDIR)
	@./D2DGRter $(TESTDIR)/testcase1/testcase1.gmp $(TESTDIR)/testcase1/testcase1.gcl $(TESTDIR)/testcase1/testcase1.cst $(TESTDIR)/testcase1/testcase1.lg $(OUTPUTDIR)/testcase1.drawlg

test3: | $(OUTPUTDIR)
	@./D2DGRter $(TESTDIR)/testcase2/testcase2.gmp $(TESTDIR)/testcase2/testcase2.gcl $(TESTDIR)/testcase2/testcase2.cst $(TESTDIR)/testcase2/testcase2.lg $(OUTPUTDIR)/testcase2.drawlg

test4: |$(OUTPUTDIR)
	@./D2DGRter testcase/testcase0.gmp testcase/testcase0.gcl testcase/testcase0.cst testcase/testcase0.lg $(OUTPUTDIR)/testcase0.drawlg
	@./Evaluator testcase testcase0

check1: | $(OUTPUTDIR)
	@./Evaluator $(TESTDIR)/testcase0 testcase0

check2: | $(OUTPUTDIR)
	@./Evaluator $(TESTDIR)/testcase1 testcase1

check3: | $(OUTPUTDIR)
	@./Evaluator $(TESTDIR)/testcase2 testcase2

draw1: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase0.drawlg $(VISUALDIR)/testcase0.png

draw2: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase1.drawlg $(VISUALDIR)/testcase1.png

draw3: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase2.drawlg $(VISUALDIR)/testcase2.png

draw4: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase0.drawlg $(VISUALDIR)/testcase0.png

gen_case:
	@python3 testcase/gen_testcase.py

check_homework: | $(OUTPUTDIR)
	@./D2DGRter $(TESTDIR)/testcase0/testcase0.gmp $(TESTDIR)/testcase0/testcase0.gcl $(TESTDIR)/testcase0/testcase0.cst $(TESTDIR)/testcase0/testcase0.lg
	@./Evaluator $(TESTDIR)/testcase0 testcase0
	@echo $(GREEN)testcase0 pass!$(NO_COLOR)
	@./D2DGRter $(TESTDIR)/testcase1/testcase1.gmp $(TESTDIR)/testcase1/testcase1.gcl $(TESTDIR)/testcase1/testcase1.cst $(TESTDIR)/testcase1/testcase1.lg
	@./Evaluator $(TESTDIR)/testcase1 testcase1
	@echo $(GREEN)testcase1 pass!$(NO_COLOR)
	@./D2DGRter $(TESTDIR)/testcase2/testcase2.gmp $(TESTDIR)/testcase2/testcase2.gcl $(TESTDIR)/testcase2/testcase2.cst $(TESTDIR)/testcase2/testcase2.lg
	@./Evaluator $(TESTDIR)/testcase2 testcase2
	@echo $(GREEN)testcase2 pass!$(NO_COLOR)
	@./D2DGRter $(TESTDIR)/testcase3/testcase3.gmp $(TESTDIR)/testcase3/testcase3.gcl $(TESTDIR)/testcase3/testcase3.cst $(TESTDIR)/testcase3/testcase3.lg
	@./Evaluator $(TESTDIR)/testcase3 testcase3
	@echo $(GREEN)testcase3 pass!$(NO_COLOR)
	@./D2DGRter $(TESTDIR)/testcase4/testcase4.gmp $(TESTDIR)/testcase4/testcase4.gcl $(TESTDIR)/testcase4/testcase4.cst $(TESTDIR)/testcase4/testcase4.lg
	@./Evaluator $(TESTDIR)/testcase4 testcase4
	@echo $(GREEN)testcase4 pass!$(NO_COLOR)
	@echo $(GREEN)all pass!$(NO_COLOR)

110511210:
	@mkdir 110511210

tar: | 110511210
	@cp -r src ./110511210
	@cp -r inc ./110511210
	@cp README.md ./110511210
	@cp main.cpp ./110511210
	@cp Makefile ./110511210
	tar -cvf 110511210.tar 110511210



$(OUTPUTDIR):
	@mkdir $(OUTPUTDIR)

$(VISUALDIR):
	@mkdir $(VISUALDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) ./110511210 110511210.tar