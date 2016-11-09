#include "fs.h"


//int fs_create(char *filename, int mode);


int fs_create1(char *filename, int mode){
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
	
	/* Convert filename to uppercase */
	//toupper(filename);
	
	/* compare fsd.root_dir.entry[i].name with the filename */
	
	for(i=0; i<fsd.root_dir.numentries; i++){
		if(strcmp(fsd.root_dir.entry[i].name, filename)==0){
			printf("\n File name already exists.");
			printf("\n Try with different filename.");
			return -1;
		}
	}
	
	/* filename is valid */
	printf("\n fsd.inodes_used = %d, fsd.ninodes = %d",fsd.inodes_used,fsd.ninodes);
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
