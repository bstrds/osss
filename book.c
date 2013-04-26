
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024
/*isbn, title, #copies*/
/*isbn, name, e-mail*/

struct b_entry {
	char isbn[14]; 
	char title[40];
	short copies; 
} b_entries;

struct l_entry {
	char isbn[14];
	char name[40];
	char email[40];
} l_entries;

FILE *fp;
FILE *fp2;

void bookentry() {
	
	char str[4]; /* temporary string to hold copies number for atoi() */
	
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

void getstats() {
	
	int child, status, n;
	int filed[2];
	char buf[19];
	char *isbn, *title, *t;
	int msize;
	char lengthbyte, lb;
	
	if(pipe(filed) == -1) 
		exit(1);
		
	if((child = fork()) == -1) 
			exit(1);	
	
	if((fp = fopen("BOOK_ENTRIES", "a+")) == NULL) 
	{
		printf("cannot open file\n");
		return;
	}
	
	if((fp2 = fopen("LENDING_ENTRIES", "a+")) == NULL) 
	{
		printf("cannot open file\n");
		return;
	}
	
	if(child > 0)
	{
		close(filed[0]);
		
		while(!feof(fp)) 
		{
			fread(&b_entries,sizeof(b_entries),1,fp);
			
			if(!feof(fp))
			{
				lengthbyte = (char)strlen(b_entries.isbn) + (char)strlen(b_entries.title) + (char)2;
				
				printf("WTF --> %d\n", lengthbyte);
				
				write(filed[1], lengthbyte, 1);
				
				write(filed[1], b_entries.isbn, strlen(b_entries.isbn));
				write(filed[1], ";", 1);
				write(filed[1], b_entries.title, strlen(b_entries.title));
				write(filed[1], ":", 1);
			}	
		}
		
		wait(&status);
		
		close(filed[1]);
		exit(0);
	}
	else
	{
		close(filed[1]);
		
		read(filed[0], &lb, 1);
		printf("LB = %d\n", lb);
		
		n = 1;
		
		while(n > 0) {
			
			n = read(filed[0], buf, 19);
			
			isbn = strtok(buf, ";");
			t = strtok(NULL, ";");
			title = strtok(t, ":");
			
			printf("ISBN = %s, TITLE = %s\n", isbn, title);
			
			//printf("%s %s\n", b_entries.isbn, b_entries.title);

			while(!feof(fp2)) 
			{
				fread(&l_entries,sizeof(l_entries),1,fp2);
				
				if(!feof(fp2))
				{
					if(strcmp(isbn, l_entries.isbn) == 0)
					{
						printf("%s %s\n", l_entries.name, l_entries.email);
					}
				}
			}
		}
		
		close(filed[0]);
		exit(0);
	}
	
	fclose(fp2);
	fclose(fp);
}

int main(int argc, char *argv[]) {
	
	int choice; /* holds menu choice of user */
	int term = 1;
	

	//system("clear"); 

	printf("\n1. Book Entry\n2. Lending Entry\n3. Print Book Statuses\
	\n4. Exit\n\n");
	
	scanf("%d", &choice);
	
	switch(choice)
	{
		case 1:	bookentry();
				break;
				
		case 2: lendentry();
				break;
				
		case 3: getstats();
				break;
				
		case 4: printf("Thank you for using me\n");
				term = -1;
				break;
		
		default: printf("u did some wrong\n");  
	}

	
	return 0;
	
}
