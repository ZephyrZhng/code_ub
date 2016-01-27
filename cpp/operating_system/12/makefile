# thread: thread.cpp group_tree.cpp pcb.cpp
# 	g++ -std=c++14 thread.cpp group_tree.cpp pcb.cpp -o thread -I.

thread: thread.o pcb.o group_tree.o
	g++ -std=c++14 -o thread thread.o pcb.o group_tree.o
thread.o: thread.cpp pcb.h group_tree.h
	g++ -std=c++14 -c thread.cpp
pcb.o: pcb.cpp pcb.h
	g++ -std=c++14 -c pcb.cpp
group_tree.o: group_tree.cpp group_tree.h pcb.h
	g++ -std=c++14 -c group_tree.cpp