all : blockfreelist.o disk.o inode.o
	clang blockfreelist.o disk.o inode.o nanofs.c -o ./nanofs

blockfreelist.o: disk.o
	clang -c blockfreelist.c

disk.o: 
	clang -c disk.c

inode.o: disk.o
	clang -c inode.c

clean :
	rm *.o
