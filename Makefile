CC = g++ -pthread
CFLAGS = -g -Wall -std=c++0x
CCLINK = $(CC)
OBJS = bank.o main.o account.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) $(CFLAGS) -o Bank $(OBJS)
# Creating the object files
account.o: account.cpp account.h
bank.o: account.cpp bank.cpp bank.h account.h
main.o: main.cpp bank.h account.h bank.cpp account.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*


