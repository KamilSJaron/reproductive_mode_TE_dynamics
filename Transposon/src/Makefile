../Transposon : main.o genome.o population.o chromosome.o locus.o random.o transposon.o mtrand.o
	g++ -g $^ -o $@

%.o: %.cpp
	g++ -g -c $<

clean:
	rm *.o test
