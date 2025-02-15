all: Lab2

CXX = g++
CXX_FLAGS = -I ./inc
LINKFLAGS = -pedantic -fomit-frame-pointer -funroll-all-loops -O3 -g
VERSION = -std=c++11
OUTPUT_DIR = testcase/output/

#main.o has to be the last
Lab2: bstar.o function.o container.o main.o
	$(CXX) $(LINKFLAGS) $(CXX_FLAGS) $(VERSION) bstar.o container.o function.o main.o -o Lab2 
main.o: inc/function.h inc/bstar.h inc/container.h main.cpp
	$(CXX) $(LINKFLAGS) $(CXX_FLAGS) $(VERSION) inc/bstar.h inc/container.h inc/function.h main.cpp -c  
container.o: inc/container.h src/container.cpp
	$(CXX) $(LINKFLAGS) $(CXX_FLAGS) $(VERSION) inc/container.h src/container.cpp -c
function.o: inc/function.h src/function.cpp
	$(CXX) $(LINKFLAGS) $(CXX_FLAGS) $(VERSION) inc/function.h src/function.cpp -c
bstar.o: inc/bstar.h src/bstar.cpp
	$(CXX) $(LINKFLAGS) $(CXX_FLAGS) $(VERSION) inc/bstar.h src/bstar.cpp -c

test:
	./Lab2 0.5 testcase/simple/simple.block testcase/simple/simple.nets $(OUTPUT_DIR)output_sim.txt
	python3 layout/draw_block_layout.py layout/layout.txt layout/layout.png
	./verifier 0.5 testcase/simple/simple.block testcase/simple/simple.nets $(OUTPUT_DIR)output_sim.txt

test1:
	./Lab2 0 testcase/ami33/ami33.block testcase/ami33/ami33.nets $(OUTPUT_DIR)output_33.txt
	python3 layout/draw_block_layout.py layout/layout.txt layout/layout.png
	./verifier 0 testcase/ami33/ami33.block testcase/ami33/ami33.nets $(OUTPUT_DIR)output_33.txt

test2:
	./Lab2 0 testcase/ami49/ami49.block testcase/ami49/ami49.nets $(OUTPUT_DIR)output_49.txt
	python3 layout/draw_block_layout.py layout/layout.txt layout/layout.png
	./verifier 0 testcase/ami49/ami49.block testcase/ami49/ami49.nets $(OUTPUT_DIR)output_49.txt

test3:
	./Lab2 0.5 testcase/vda317b/vda317b.block testcase/vda317b/vda317b.nets $(OUTPUT_DIR)output_vda.txt
	python3 layout/draw_block_layout.py layout/layout.txt layout/layout.png
	./verifier 0.5 testcase/vda317b/vda317b.block testcase/vda317b/vda317b.nets $(OUTPUT_DIR)output_vda.txt

draw:
	python3 layout/draw_block_layout.py layout/layout.txt layout/layout.png
	
submit:
	cp Makefile ./110511210
	cp main.cpp ./110511210
	cp -r ./src ./110511210
	cp -r ./inc ./110511210	
	tar -cvf 110511210.tar ./110511210

clean:
	rm -rf *.o *.txt *.HPWL *.gch Lab2