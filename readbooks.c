#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct b_entry {
	char isbn[14]; 
	char title[40];
	short copies; 
} entries;

FILE *fp;

void main() {

	if((fp=fopen("BOOK_ENTRIES","r")) == NULL) {
		printf("Cannot open file\n");
		return;
	}
	
	while(!feof(fp)) {
		fread(&entries,sizeof(entries),1,fp);
		if(!feof(fp))
			printf("%10s %10s %10d\n",entries.isbn,entries.title,entries.copies);
	}
	
	fclose(fp);
}
