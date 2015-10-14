CMP = gcc
FLAGS = -lssl -lcrypto -g -Wall 
FNAME = tcpclient

all: client

client: $(FNAME).c
	$(CMP) $(FLAGS) $(FNAME).c -o $(FNAME)

clean:
	rm -f $(FNAME) SmallFile.txt MediumFile.pdf LargeFile.mp4
