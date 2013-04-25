
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
	
	
	printf("New ISBN: ");
	
	for(;;) 
	{
		scanf("%s", &b_entries.isbn);
		
		if(strlen(b_entries.isbn)==0 || strlen(b_entries.isbn)>13) 
			printf("\nYou provided an invalid ISBN number\n");
		else 
			break;
	}
	
	printf("New Title: ");
	scanf("%s", &b_entries.title);
	
	printf("Copies Left: ");
	scanf("%s", &str);
	b_entries.copies = atoi(str);
	
	fwrite(&b_entries, sizeof(b_entries), 1, fp);
	
	fclose(fp);
}

void lendentry() {
	
	system("clear");
	
	if((fp = fopen("LENDING_ENTRIES", "a+")) == NULL) 
	{
		printf("cannot open file\n");
		return;
	}
	
	printf("Enter ISBN: ");
	
	for(;;) 
	{
		scanf("%s", &l_entries.isbn);
		
		if(strlen(l_entries.isbn)==0 || strlen(l_entries.isbn)>13) 
			printf("\nYou provided an invalid ISBN number\n");
		else 
			break;
	}
	
	printf("Enter name: ");
	scanf("%s", &l_entries.name);
	
	printf("Enter email: ");
	scanf("%s", &l_entries.email);
	
	fwrite(&l_entries, sizeof(l_entries), 1, fp);
	
	fclose(fp);
}

int main(int argc, char *argv[]) {
	
	int choice; /* holds menu choice of user */
	
	for(;;)
	{
		system("clear"); 
	
		printf("1. Book Entry\n2. Lending Entry\n3. Print Book Statuses\
		\n4. Exit\n");
		
		scanf("%d", &choice);
		
		switch(choice)
		{
			case 1:	bookentry();
					break;
					
			case 2: lendentry();
					break;
					
			case 3: printf("#\n");
					break;
					
			case 4: printf("Thank you for using me\n");
					exit(0);
			
			default: printf("u did some wrong\n");  
		}
	}
	
	return 0;
	
}
