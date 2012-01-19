/*
 *	usage:	./a.out bootloader.img [intervalsize]
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define DEFAULT_INTERVAL_SIZE 3*1024

#define READ_SIZE 2048

int main(int argc, char **argv)
{
	char readbuf[READ_SIZE]={0};
	int fd, n, i, intervalsize;
	int null_count = 0;
	int block_num = 0;
	int block_info[20][2] = {{0},{0}};
	
	intervalsize = DEFAULT_INTERVAL_SIZE;
	if (argc == 3)
		intervalsize = atoi(argv[2]);		
	
	fd = open(argv[1], O_RDONLY);
	if (fd<0) {
		printf("can't open %s\n", argv[1]);
		return -1;
	}

	/* skip first 4K */
	block_num++;
	lseek(fd, 4096, SEEK_CUR);
	block_info[block_num][0]= 4096;

	while ((n = read(fd, readbuf, READ_SIZE)) > 0) {
		for (i=0; i<n; i++) {
			if (readbuf[i] == 0) {
				null_count++;
			} else {
				if (null_count > intervalsize) {
					block_info[block_num][1] = null_count;
					block_num++;
					block_info[block_num][0] = lseek(fd, 0, SEEK_CUR) - READ_SIZE;
				}
				null_count = 0;
			}
		}
	}
	block_info[block_num+1][0] = lseek(fd, 0, SEEK_END);
	

	printf("No.\t\toff\t\tblocksize\tintervalsize\n");
	for (i=0; i <= block_num; i++)
		printf("%d\t0x%08x-0x%08x\t%8d\t%8d\n", 
			i, block_info[i][0], 
			block_info[i+1][0] - 1, 
			block_info[i+1][0] - block_info[i][0], 
			block_info[i][1]);


	return 0;
}
