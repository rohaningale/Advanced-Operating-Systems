#include "fs.h"

int writeBlock1(int dev, int block, int offset, void *buf, int len){
	/* API to bs_bwrite(int dev, int block, int offset, void *buf, int len) */
	
	return bs_bwrite(dev, block, offset, buf, len);
}

int min1(int a, int b){
	if(a > b) 
		return b;
	return a;
}

int fs_write1(int fd, void *buf, int nbytes){

	int i, j, status;
	int blocksToWrite=0;
	struct inode tempInode;
	int bytesToWrite=0, minBytes=0;
	/* Check for valid fd and mode in the open file table */
	
	if(oft[fd].state != FS_OPEN || (oft[fd].mode != O_RDWR && oft[fd].mode != O_WRONLY)){
		printf("\n File not open or opened in Read mode.");
		printf("\n Could not write to file.");
		return -1;
	}
	
	/* Check for valid nbytes */
	
	if(nbytes <=0 || strlen((char*)buf)==0){
		printf("\n Invalid nbytes value entered or invalid data length.");
		printf("\n Could not write to file.");
		return -1;	
	}
	
	/* Valid data. Scale down nbytes if greater than strlen(buf) */
	if(strlen((char*)buf) < nbytes){
		nbytes = strlen((char*)buf);
	}
	
	/* Check if this operation is overwriting the file */
	if(oft[fd].in.size > 0){
		tempInode = oft[fd].in;
		/* Overwriting, thus clean the file data blocks. */
		for(;oft[fd].in.size>0;){
			/* for all data blocks, clear the mask bit */
			if(fs_clearmaskbit(tempInode.blocks[oft[fd].in.size-1]) != OK){
				printf("\n Error in clearing block %d",oft[fd].in.size-1);
				printf("\n Could not write to file.");
				return -1;
			}
			/* decrement the size */
			oft[fd].in.size--;
		}
	}
	
	blocksToWrite = nbytes / MDEV_BLOCK_SIZE;
	if(nbytes % MDEV_BLOCK_SIZE !=0){
		blocksToWrite++;
	}
	
	bytesToWrite = nbytes;
	
	/* Initialize j to first data block */
	j = FIRST_INODE_BLOCK + NUM_INODE_BLOCKS; 
	for(i=0; i<blocksToWrite && j<512; j++){
		if(fs_getmaskbit(j) == 0){
			/* clear the data block to write */
			memset(block_cache, NULL, 512);
			if(writeBlock(0, j, 0, block_cache, 512) == SYSERR){
				printf("\n Unable to free block %d.",j);
				printf("\n Could not write to file.");
				return -1;
			}
			/* get the minimum bytes to write */
			minBytes = min(512, bytesToWrite);
			/* copy the data into block_cache */
			memcpy(block_cache, buf, minBytes);
			
			/* write the data into the data block */
			if(writeBlock(0, j, 0, block_cache, 512) == SYSERR){
				printf("\n Unable to write to block %d.",j);
				printf("\n Could not write to file.");
				return -1;
			}
			
			buf = (char*) buf + minBytes;
			bytesToWrite = bytesToWrite - minBytes;
			fs_setmaskbit(j);
			
			/* keep track of this data block into blocks[] */
			oft[fd].in.blocks[i++] = j;
		}
	}
	/* Update the size of the data blocks */
	oft[fd].in.size = blocksToWrite;
	return nbytes;
}
