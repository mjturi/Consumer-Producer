# Matthew Turi
# ID: 822202323

CXX = gcc
CXXFLAGS = -g -std=gnu99 -Wall -Wextra -Wuninitialized -Wunused-variable -Wdouble-promotion -Wmissing-prototypes -Wmissing-declarations -Werror=undef -Wno-unused-function

mizzo : io.o consumer.o producer.o main.o
	$(CXX) $(CXXFLAGS) io.o consumer.o producer.o main.o -o mizzo -lpthread -lrt

io.o : io.h io.c
	$(CXX) $(CXXFLAGS) -c io.c

consumer.o: consumer.h consumer.c
	$(CXX) $(CXXFLAGS) -c consumer.c

producer.o: producer.h producer.c
	$(CXX) $(CXXFLAGS) -c producer.c

main.o : main.c
	$(CXX) $(CXXFLAGS) -c main.c


clean :
	rm -f *.o mizzo
