
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*isbn, title, #copies*/
/*isbn, name, e-mail*/

struct b_entry {
	char isbn[13]; 
	char title[40];
	short copies; 
} entries;

FILE *fp;

main() {
	
	char str[3];
	system("clear"); 
	
	if((fp = fopen("BOOK_ENTRIES", "a+")) == NULL) 
	{
		printf("cannot open file\n");
		return;
	}
	
	int i;
	
	for(i=0; i<2; i++) 
	{
		printf("New ISBN ");
		gets(entries.isbn);
		if(strlen(entries.isbn)==0) 
		{
			fclose(fp);
			exit(0);
		}
		
		printf("New Title ");
		gets(entries.title);
		
		printf("Copies Left ");
		gets(str);
		entries.copies = atoi(str);
		
		fwrite(&entries, sizeof(entries), 1, fp);
	}
}
