
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*isbn, title, #copies*/
/*isbn, name, e-mail*/

struct b_entry {
	char isbn[13]; 
	char title[40];
	short copies; 
} b_entries;

struct l_entry {
	char isbn[13];
	char name[40];
	char email[40];
} l_entries;

FILE *fp;

void bookentry() {
	
	char str[3]; /* temporary string to hold copies number for atoi() */
	
	system("clear"); 
	
	if((fp = fopen("BOOK_ENTRIES", "a+")) == NULL) 
	{
		printf("cannot open file\n");
		return;
	}
	
	for(;;) {
		printf("New ISBN ");
		gets(b_entries.isbn);
		if(strlen(b_entries.isbn)==0) 
		{
			fclose(fp);
			exit(0);
		}
		
		printf("New Title ");
		gets(b_entries.title);
		
		printf("Copies Left ");
		gets(str);
		b_entries.copies = atoi(str);
		
		fwrite(&b_entries, sizeof(b_entries), 1, fp);
	}

}

main() {
	
	int choice; /* holds menu choice of user */
	
	system("clear"); 
	
	printf("1. Book Entry\n2. Lending Entry\n3. Print Book Statuses\n");
	
	scanf("%d", &choice);
	
	switch(choice)
	{
		case 1:	bookentry();
				break;
				
		case 2: printf("@\n");
				break;
				
		case 3: printf("#\n");
				break;
		
		default: printf("u did some wrong\n");  
	}
	
	
}
