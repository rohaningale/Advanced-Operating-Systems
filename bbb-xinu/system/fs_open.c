#include "fs.h"

int fs_open1(char *filename, int flag){

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
			break;
		}
		if(oft[j].state == FS_CLOSE && oftindex == -1){
			oftindex = j;
		}
	}
	
	if(j < NUM_FD){
		printf("\n File already opened.");
		printf("\n Could not open a file already opened.");
		return -1;
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
