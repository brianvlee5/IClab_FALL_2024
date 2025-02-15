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

TARGET = Legalizer

all: $(TARGET)

$(OBJDIR):
	@mkdir $(OBJDIR)

Legalizer: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) $^ -o $@

-include $(DEPS)

#these %.o will be run when we use the OBJS, then it will find the src.cpp and use it
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) -MMD -c $< -o $@


test1: | $(OUTPUTDIR)
	@./Legalizer $(TESTDIR)/testcase1_16900.lg $(TESTDIR)/testcase1_16900.opt $(OUTPUTDIR)/testcase1_16900_post.lg $(OUTPUTDIR)/testcase1_16900.drawlg $(OUTPUTDIR)/testcase1_16900.result
	@./Evaluator testcase/testcase1_16900.lg testcase/testcase1_16900.opt result/testcase1_16900_post.lg
	@python3 $(TESTDIR)/testcase_checker.py --lg $(OUTPUTDIR)/testcase1_16900.result --opt $(TESTDIR)/testcase1_16900.opt

draw1: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase1_16900.drawlg $(VISUALDIR)/testcase1_16900.png

test2: | $(OUTPUTDIR)
	@./Legalizer $(TESTDIR)/testcase2_100.lg $(TESTDIR)/testcase2_100.opt $(OUTPUTDIR)/testcase2_100_post.lg $(OUTPUTDIR)/testcase2_100.drawlg $(OUTPUTDIR)/testcase2_100.result
	@./Evaluator testcase/testcase2_100.lg testcase/testcase2_100.opt result/testcase2_100_post.lg
	@python3 $(TESTDIR)/testcase_checker.py --lg $(OUTPUTDIR)/testcase2_100.result --opt $(TESTDIR)/testcase2_100.opt

draw2: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase2_100.drawlg $(VISUALDIR)/testcase2_100.png

test3: | $(OUTPUTDIR)
	@./Legalizer $(TESTDIR)/testcase1_ALL0_5000.lg $(TESTDIR)/testcase1_ALL0_5000.opt $(OUTPUTDIR)/testcase1_ALL0_5000_post.lg $(OUTPUTDIR)/testcase1_ALL0_5000.drawlg $(OUTPUTDIR)/testcase1_ALL0_5000.result
	@./Evaluator testcase/testcase1_ALL0_5000.lg testcase/testcase1_ALL0_5000.opt result/testcase1_ALL0_5000_post.lg
	@python3 $(TESTDIR)/testcase_checker.py --lg $(OUTPUTDIR)/testcase1_ALL0_5000.result --opt $(TESTDIR)/testcase1_ALL0_5000.opt

draw3: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase1_ALL0_5000.drawlg $(VISUALDIR)/testcase1_ALL0_5000.png

test4: | $(OUTPUTDIR)
	@./Legalizer $(TESTDIR)/testcase1_MBFF_LIB_7000.lg $(TESTDIR)/testcase1_MBFF_LIB_7000.opt $(OUTPUTDIR)/testcase1_MBFF_LIB_7000_post.lg $(OUTPUTDIR)/testcase1_MBFF_LIB_7000.drawlg $(OUTPUTDIR)/testcase1_MBFF_LIB_7000.result
	@./Evaluator $(TESTDIR)/testcase1_MBFF_LIB_7000.lg $(TESTDIR)/testcase1_MBFF_LIB_7000.opt $(OUTPUTDIR)/testcase1_MBFF_LIB_7000_post.lg
	@python3 $(TESTDIR)/testcase_checker.py --lg $(OUTPUTDIR)/testcase1_MBFF_LIB_7000.result --opt $(TESTDIR)/testcase1_MBFF_LIB_7000.opt

draw4: | $(VISUALDIR)
	@python3 $(VISUALDIR)/visualizer.py $(OUTPUTDIR)/testcase1_MBFF_LIB_7000.drawlg $(VISUALDIR)/testcase1_MBFF_LIB_7000.png

check_homework: | $(OUTPUTDIR)
	@./Legalizer $(TESTDIR)/testcase1_16900.lg $(TESTDIR)/testcase1_16900.opt $(OUTPUTDIR)/testcase1_16900_post.lg
	@./Evaluator testcase/testcase1_16900.lg testcase/testcase1_16900.opt result/testcase1_16900_post.lg
	@echo $(GREEN)testcase1_16900 pass!$(NO_COLOR)
	@./Legalizer $(TESTDIR)/testcase2_100.lg $(TESTDIR)/testcase2_100.opt $(OUTPUTDIR)/testcase2_100_post.lg
	@./Evaluator testcase/testcase2_100.lg testcase/testcase2_100.opt result/testcase2_100_post.lg
	@echo $(GREEN)testcase2_100 pass!$(NO_COLOR)
	@./Legalizer $(TESTDIR)/testcase1_ALL0_5000.lg $(TESTDIR)/testcase1_ALL0_5000.opt $(OUTPUTDIR)/testcase1_ALL0_5000_post.lg
	@./Evaluator testcase/testcase1_ALL0_5000.lg testcase/testcase1_ALL0_5000.opt result/testcase1_ALL0_5000_post.lg
	@echo $(GREEN)testcase1_ALL0_5000 pass!$(NO_COLOR)
	@./Legalizer $(TESTDIR)/testcase1_MBFF_LIB_7000.lg $(TESTDIR)/testcase1_MBFF_LIB_7000.opt $(OUTPUTDIR)/testcase1_MBFF_LIB_7000_post.lg
	@./Evaluator $(TESTDIR)/testcase1_MBFF_LIB_7000.lg $(TESTDIR)/testcase1_MBFF_LIB_7000.opt $(OUTPUTDIR)/testcase1_MBFF_LIB_7000_post.lg
	@echo $(GREEN)testcase1_MBFF_LIB_7000 pass!$(NO_COLOR)
	@echo $(GREEN)all pass!$(NO_COLOR)

110511210: | 110511210
	@mkdir 110511210

tar: | 110511210
	@cp -r src ./110511210
	@cp -r inc ./110511210
	@cp main.cpp ./110511210
	@cp Makefile ./110511210
	@cp 110511210_rpt.pdf ./110511210
	tar -cvf 110511210.tar 110511210



$(OUTPUTDIR):
	@mkdir $(OUTPUTDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) ./110511210 110511210.tar