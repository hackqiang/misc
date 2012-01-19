/*
 *	usage:	./a.out bootloader.img [intervalsize]
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define DEFAULT_INTERVAL_SIZE 3*1024


void dumpfile(const char *src, const char *dest, int offset, int size)
{
	char cmd[256] = {0};
	sprintf(cmd, "dd if=%s of=%s bs=1 skip=%d count=%d", src, dest, offset, size);
	printf("%s\n",cmd);
	system(cmd);
}


int main(int argc, char **argv)
{
	char readbuf;
	char writefilename[32] = {0};
	int fd, i, intervalsize;
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
	dumpfile(argv[1], "b0", 0, 4096);
	
	while (read(fd, &readbuf, 1) == 1) {
			if (readbuf == 0) {
				null_count++;
			} else {
				if (null_count > intervalsize) {
					block_info[block_num][1] = null_count;
					block_num++;
					block_info[block_num][0] = lseek(fd, 0, SEEK_CUR)-1;
					sprintf(writefilename, "b%d", block_num-1);
					dumpfile(argv[1], writefilename,
							 block_info[block_num-1][0],
							 block_info[block_num][0]-block_info[block_num-1][0]-null_count);
				}
				null_count = 0;
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
