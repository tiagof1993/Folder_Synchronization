CC = g++
CFLAGS = -g -Wall -pg
TARGET= FOLDER_SYNCHRONIZATION


folder_synchronization: synchronization.o
	$(CC) -o $(TARGET) synchronization.o
folder_synchronization.o: synchronization.cpp
	$(CC) -c $(CFLAGS) synchronization.cpp 