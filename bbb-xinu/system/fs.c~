
#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "fs.h"

int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

int next_open_fd = 0;

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }
  
  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

int fs_create(char *filename, int mode){
	int flen, status;
	int i;
	struct inode in;
	/* Check for valid mode */
	
	if(mode != O_CREAT){
		printf("\n Invalid value for mode.");
		printf("\n Could not create file.");
		return -1;
	}
	
	/* Check filename length */
	flen = strnlen(filename, FILENAMELEN);
	if(flen > FILENAMELEN){
		return -1;
	}
	
	/* compare fsd.root_dir.entry[i].name with the filename */
	
	for(i=0; i<fsd.root_dir.numentries; i++){
		if(strcmp(fsd.root_dir.entry[i].name, filename)==0){
			printf("\n File name already exists.");
			printf("\n Try with different filename.");
			return -1;
		}
	}
	
	/* filename is valid */
	/* check for availability of inode */
	if(fsd.inodes_used >= fsd.ninodes){
		printf("\n All inodes are consumed.");
		printf("\n Could not create file.");
		return -1;
	}
	
	/* scan through all inodes and get the free inode */
	
	for(i = 0; i < fsd.ninodes; i++){
		/* get inodes block by block */
		if(inodes_state[i] != USED){
			break;
		}
	}
	
	if((status = fs_get_inode_by_num(0, i, &in))==SYSERR){
		printf("\n Could not obtain inode.");
		printf("\n Could not create file.");
		return -1;
	}

	/* fill up the inode information */	
	in.id = i;
	in.type = INODE_TYPE_FILE;
	in.nlink = 0;
	in.device = 0;
	in.size = 0;

	/* writeback to the memory */
	if((status = fs_put_inode_by_num(0, i, &in))==SYSERR){
		printf("\n Could not write inode.");
		printf("\n Could not create file.");
		return -1;
	}

	/* update the inodes_state array with ith index as used. */
	printf("\n Obtained inode. \n");
	inodes_state[i] = USED;
	
	/* fill up the entry structure of the root_dir */
	fsd.root_dir.entry[fsd.root_dir.numentries].inode_num = i;
	strcpy(fsd.root_dir.entry[fsd.root_dir.numentries].name, filename);
		
	/* file is created */
	
	/* increment inodes_used and the numentries count */
	fsd.inodes_used++;
	fsd.root_dir.numentries++;
	return OK;
}

int fs_open(char *filename, int flag){

	/* flag = O_RDONLY(0), O_WRONLY(1), O_RDWR(2) */
		
	int i, j, status;
	int oftindex = -1;
	struct inode in;
	/* compare fsd.root_dir.entry[i].name with the filename */
	
	if(!(flag == O_WRONLY || flag == O_RDONLY || flag == O_RDWR)){
		printf("\n Incorrect flag entered.");
		printf("\n Could not open file.");
		return -1;
	}
	
	/* compare fsd.root_dir.entry[i].name with the filename */
	/* check for presence of filename in the directory */
	for(i=0; i<fsd.root_dir.numentries; i++){
		if(strcmp(fsd.root_dir.entry[i].name, filename)==0){
			break;
		}
	}

	if(i >= fsd.root_dir.numentries){
		printf("\n File not found.");
		printf("\n Could not open file.");
		return -1;	
	}
	
	/* check if the file is already opened */
	
	for(j=0; j<NUM_FD; j++){
		if(oft[j].in.id == fsd.root_dir.entry[i].inode_num){
			if(oft[j].state == FS_CLOSE){
				oftindex = j;
				break;
			}else if(oft[j].state == FS_OPEN){
				printf("\n File already opened.");
				printf("\n Could not open a file already opened.");
				return -1;
			}
		}
		if(oft[j].state == FS_CLOSE && oftindex == -1){
			oftindex = j;
		}
	}
	
	if(oftindex == -1){
		printf("\n Open file table exhausted.");
		printf("\n Could not open file.");
		return -1;
	}
	
	/* retrieve the inode block by the inode number */
	if((status = fs_get_inode_by_num(0, fsd.root_dir.entry[i].inode_num, &in)) == SYSERR){
		printf("\n Error in retrieving the inode block.");
		printf("\n Could not open file.");
		return -1;
	}
	
	/* make an openfiletable entry */
	oft[oftindex].state = FS_OPEN;
	oft[oftindex].fileptr = 0;
	oft[oftindex].de = &fsd.root_dir.entry[i];
	oft[oftindex].in = in;
	oft[oftindex].mode = flag;
	/* return the oftindex */
	return oftindex;
}

int writeBlock(int dev, int block, int offset, void *buf, int len){
	/* API to bs_bwrite(int dev, int block, int offset, void *buf, int len) */
	return bs_bwrite(dev, block, offset, buf, len);
}

int min(int a, int b){
	if(a > b) 
		return b;
	return a;
}

int fs_write(int fd, void *buf, int nbytes){

	int i, j, status;
	int blocksToWrite=0;
	struct inode tempInode;
	int bytesToWrite=0, minBytes=0;

	/* Check for valid fd and mode in the open file table */
	if(fd<0 || fd>NUM_FD){
		printf("\n Invalid file descriptor.");
		printf("\n Could not write.");
		return -1;
	}
	
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

	for(i=0; i<blocksToWrite && j<MDEV_BLOCK_SIZE; j++){
		if(fs_getmaskbit(j) == 0){
			/* clear the data block to write */
			memset(block_cache, NULL, MDEV_BLOCK_SIZE);
			if(writeBlock(0, j, 0, block_cache, MDEV_BLOCK_SIZE) == SYSERR){
				printf("\n Unable to free block %d.",j);
				printf("\n Could not write to file.");
				return -1;
			}
			/* get the minimum bytes to write */
			minBytes = min(MDEV_BLOCK_SIZE, bytesToWrite);
			/* copy the data into block_cache */
			memcpy(block_cache, buf, minBytes);
			
			/* write the data into the data block */
			if(writeBlock(0, j, 0, block_cache, MDEV_BLOCK_SIZE) == SYSERR){
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
	printf("\n Size updated to %d blocks.",oft[fd].in.size);
	if((status = fs_put_inode_by_num(0, oft[fd].in.id, &oft[fd].in))==SYSERR){
		printf("\n Could not write inode.");
		printf("\n Could not write file.");
		return -1;
	}
	oft[fd].filesize = nbytes;
	return nbytes;
}

int readBlock(int dev, int block, int offset, void *buf, int len){
	/* API to bs_bread(int dev, int block, int offset, void *buf, int len) */
	return bs_bread(dev, block, offset, buf, len);
}

int fs_read(int fd, void *buf, int nbytes){
	
	int i, j, status, temp=0;
	int blocksToRead=0;
	struct inode tempInode;
	int bytesRead=0, minBytes=0;
	
	/* Valid fd checking */
	if(fd<0 || fd>NUM_FD){
		printf("\n Invalid file descriptor.");
		printf("\n Could not write.");
		return -1;
	}
	/* Checking valid file mode */
	if(oft[fd].state != FS_OPEN || (oft[fd].mode != O_RDWR && oft[fd].mode != O_RDONLY)){
		printf("\n File not open or opened in Write mode.");
		printf("\n Could not read from file.");
		return -1;
	}
	
	/* Check for valid nbytes */
	if(nbytes <=0){
		printf("\n Invalid nbytes value entered or invalid data length.");
		printf("\n Could not write to file.");
		return -1;	
	}
	
	/* Check if file is empty */
	if(oft[fd].in.size == 0){
		printf("\n Desired file empty.");
		return 0;
	}
	nbytes += oft[fd].fileptr;
	blocksToRead = nbytes / MDEV_BLOCK_SIZE;
	if(nbytes % MDEV_BLOCK_SIZE !=0){
		blocksToRead++;
	}

	/* Truncate to the minimum size blocks */
	blocksToRead = min(oft[fd].in.size, blocksToRead);

	/* set the start block for reading the file */
	i = (oft[fd].fileptr / MDEV_BLOCK_SIZE);

	memset(buf, NULL, MAXSIZE);
	for(j=(oft[fd].fileptr % MDEV_BLOCK_SIZE); i<blocksToRead; i++){
		memset(block_cache, NULL, MDEV_BLOCK_SIZE+1);

		if(readBlock(0, oft[fd].in.blocks[i], j, block_cache, MDEV_BLOCK_SIZE-j) == SYSERR){
			printf("\n Error in reading the file.");
			return -1;
		}

		strcpy((buf+temp), block_cache);
		temp = strlen(block_cache);
		bytesRead += temp;
		j=0; // reset the offset for read to 0
	}
	oft[fd].fileptr = bytesRead;
	return bytesRead;
}

/* seek by offset. */
/* check for combined value (offset+fileptr) <= filesize */

int fs_seek(int fd, int position, int offset){
	
	/* Valid fd checking */
	if(fd<0 || fd>NUM_FD){
		printf("\n Invalid file descriptor.");
		printf("\n Could not seek.");
		return -1;
	}
	/* Check if file is open. */
	if(oft[fd].state != FS_OPEN){
		printf("\n File not open.");
		printf("\n Could not seek.");
		return -1;
	}

	/* position = FS_BEGIN, FS_CURR, FS_EOF */
	switch(position){
		case FS_BEGIN:
			oft[fd].fileptr = 0;
			break;
		case FS_EOF:
			oft[fd].fileptr = oft[fd].filesize;
			break;
		case FS_CURR:
			break;
		default:
			printf("\n Incorrect position parameter.");
			printf("\n Could not seek.");
			return -1;
	}
	
	
	if((offset + oft[fd].fileptr) < 0){
		printf("\n Negative offset.");
		return -1;
	}
	
	if(oft[fd].filesize < (offset + oft[fd].fileptr)){
		oft[fd].fileptr = oft[fd].filesize;
		printf("\n fileptr set to %d",oft[fd].fileptr);
	}else{
		oft[fd].fileptr = oft[fd].fileptr + offset;
	}
	return oft[fd].fileptr;
}


int fs_close(int fd){
	if(fd < 0 || fd >= NUM_FD){
		printf("\n Invalid file descriptor.");
		return -1;
	}
	/* Add clean up */
	if(oft[fd].state == FS_OPEN){
		oft[fd].state = FS_CLOSE;
		oft[fd].fileptr = 0;
		return OK;
	}
	return -1;
}
