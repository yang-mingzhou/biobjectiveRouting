CC = gcc
CFLAGS = -O3 -mcmodel=medium
CPPFLAGS = -std=c++11 -lstdc++
C99FLAGS = -std=c99 -lm -lstdc++

MAP ?= test
NPARTITIONS ?= 3

HBOR_OBJ1 = kahip.o
HBOR_OBJ2 = bhepvPrecomputation.o
HBOR_OBJ3 = bhepvPathRetrieval.o
HBOR_OBJ4 = namoadrPathRetrievalWorkFlow.o

B3HBOR_OBJ1 = kahip.o
B3HBOR_OBJ2 = precomputation.o
B3HBOR_OBJ3 = encodeBhepv.o
B3HBOR_OBJ4 = online.o

all: HBOR B3HBOR 

HBOR: $(HBOR_OBJ1) $(HBOR_OBJ2) $(HBOR_OBJ3) $(HBOR_OBJ4)

B3HBOR: $(B3HBOR_OBJ1) $(B3HBOR_OBJ2) $(B3HBOR_OBJ3) $(B3HBOR_OBJ4)

$(HBOR_OBJ1): kahip.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

$(HBOR_OBJ2): bhepvPrecomputation.cpp bhepv.cpp bodPathRetrieval.c heap.c bod.c graph.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C99FLAGS) -o $@ $^

$(HBOR_OBJ3): bhepvPathRetrieval.cpp hborWithBhepv.cpp pathRetrieval.c heap.c boastar.c graph.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C99FLAGS) -o $@ $^

$(HBOR_OBJ4): namoadrPathRetrievalWorkFlow.cpp bhepvForNamoadr.cpp namoadrPathRetrieval.c heap.c namoadr.c graph.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C99FLAGS) -o $@ $^

$(B3HBOR_OBJ1): kahip.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

$(B3HBOR_OBJ2): precomputation.cpp hbor.cpp biobjectiveGraph.cpp pathRetrieval.c heap.c boastar.c graph.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C99FLAGS) -o $@ $^

$(B3HBOR_OBJ3): m.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

$(B3HBOR_OBJ4): onlineRouting.cpp hbor.cpp biobjectiveGraph.cpp pathRetrieval.c heap.c boastar.c graph.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C99FLAGS) -o $@ $^


run_HBOR: HBOR
	mkdir -p ../bhepv/$(MAP)/fragments
	./kahip.o ../Maps/${MAP}-road-d.txt ../bhepv/${MAP}/kahip.graph
	../../kahip/KaHIP/deploy/graphchecker ../bhepv/${MAP}/kahip.graph 
	../../kahip/KaHIP/deploy/kaffpa ../bhepv/${MAP}/kahip.graph --output_filename=../bhepv/${MAP}/kaffpaIndex.txt --k ${NPARTITIONS} --preconfiguration=strong
	./bhepvPrecomputation.o ${MAP} ${NPARTITIONS}
	./bhepvPathRetrieval.o ${MAP} ${NPARTITIONS}
	./namoadrPathRetrievalWorkFlow.o ${MAP} ${NPARTITIONS}

run_B3HBOR: B3HBOR
	mkdir -p ../b3hepv/$(MAP)/fragments
	./kahip.o ../Maps/${MAP}-road-d.txt ../b3hepv/${MAP}/kahip.graph
	../../kahip/KaHIP/deploy/graphchecker ../b3hepv/${MAP}/kahip.graph 
	../../kahip/KaHIP/deploy/kaffpa ../b3hepv/${MAP}/kahip.graph --output_filename=../b3hepv/${MAP}/kaffpaIndex.txt --k ${NPARTITIONS} --preconfiguration=strong
	./precomputation.o ${MAP}
	./encodeBhepv.o ${MAP}
	./online.o ${MAP} ${NPARTITIONS}


clean:
	rm -f $(HBOR_OBJ1) $(HBOR_OBJ2) $(HBOR_OBJ3) $(HBOR_OBJ4) $(B3HBOR_OBJ1) $(B3HBOR_OBJ2) $(B3HBOR_OBJ3) $(B3HBOR_OBJ4)
