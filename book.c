
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct b_entry {
	char isbn[20]; 
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
	
	/* temporary string to hold copies number for atoi() */
	char str[4]; 
	int i;
	
	if((fp = fopen("BOOK_ENTRIES", "a+")) == NULL) 
	{
		printf("cannot open file\n");
		return;
	}
	
	/* catching a stray newline.. :/ */
	getchar();
	
	printf("New ISBN: ");
	fgets(b_entries.isbn, sizeof(b_entries.isbn), stdin);
	
	/* removing the newline that gets added by fgets */
	for(i=0; i<strlen(b_entries.isbn); i++) 
	{
		if(b_entries.isbn[i] == '\n')
			b_entries.isbn[i] = 0;
	}
	
	printf("New Title: ");
	fgets(b_entries.title, sizeof(b_entries.title), stdin);
	for(i=0; i<strlen(b_entries.title); i++) 
	{
		if(b_entries.title[i] == '\n')
			b_entries.title[i] = 0;
	}
	
	printf("Copies Left: ");
	fgets(str, sizeof(str), stdin);
	b_entries.copies = atoi(str);
	
	/* writing everything to the "BOOK_ENTRIES" FILE */
	fwrite(&b_entries, sizeof(b_entries), 1, fp);
	
	fclose(fp);
}

void lendentry() {
	
	int i;
	
	if((fp = fopen("LENDING_ENTRIES", "a+")) == NULL) 
	{
		printf("cannot open file\n");
		return;
	}
	
	/* catching that newline again :P */
	getchar();
	
	
	/* read isbn from stdin */
	printf("Enter ISBN: ");
	fgets(l_entries.isbn, sizeof(l_entries.isbn), stdin);
	/* again, removing the newline that fgets() adds if the string
	 * you provide is not max length
	 */
	for(i=0; i<strlen(l_entries.isbn); i++) 
	{
		if(l_entries.isbn[i] == '\n')
			l_entries.isbn[i] = 0;
	}
	
	/* read name from stdin */
	printf("Enter name: ");
	fgets(l_entries.name, sizeof(l_entries.name), stdin);
	for(i=0; i<strlen(l_entries.name); i++) 
	{
		if(l_entries.name[i] == '\n')
			l_entries.name[i] = 0;
	}
	
	/* read email from stdin */
	printf("Enter email: ");
	fgets(l_entries.email, sizeof(l_entries.email), stdin);
	for(i=0; i<strlen(l_entries.email); i++) 
	{
		if(l_entries.email[i] == '\n')
			l_entries.email[i] = 0;
	}
	
	/* writing everything to the "LENDING_ENTRIES" file */
	fwrite(&l_entries, sizeof(l_entries), 1, fp);
	
	fclose(fp);
}

void getstats() {
	
	int child, status, n;
	int filed[2];
	char buf[40];
	char *isbn, *title, *t;
	unsigned char lengthbyte, lb;
	
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
				/* a byte where the length of the upcoming string is
				 * calculated and stored
				 */
				lengthbyte = (unsigned char)strlen(b_entries.isbn) + \
							 (unsigned char)strlen(b_entries.title) + \
							 (unsigned char)2;
				
				/* writing the length byte to the pipe */
				write(filed[1], &lengthbyte, 1);
				
				/* writing a book entry (isbn and title) to the pipe,
				 * the symbols ';' and ':' are simply deliminators 
				 * for later string splitting
				 */
				write(filed[1], b_entries.isbn, strlen(b_entries.isbn));
				write(filed[1], ";", 1);
				write(filed[1], b_entries.title, strlen(b_entries.title));
				write(filed[1], ":", 1);
			}	
			
		}
		
		/* sending a termination signal to the pipe */
		lengthbyte = strlen("TERMINATE")+1;
		write(filed[1], &lengthbyte, 1);
		write(filed[1], "TERMINATE", strlen("TERMINATE")+1);
		
		wait(&status);
		
		close(filed[1]);
		exit(0);
	}
	else
	{
		close(filed[1]);
		
		/* reading the first byte to determine the length of the 
		 * upcoming string
		 */
		read(filed[0], &lb, 1);
		
		n = 1;
		
		while(n > 0) {
			
			n = read(filed[0], buf, lb);
			
			if(strcmp(buf, "TERMINATE") == 0)
				break;
			
			/* splitting the string to get the tokens we need */
			isbn = strtok(buf, ";");
			t = strtok(NULL, ";");
			title = strtok(t, ":");
			
			printf("ISBN = %s, TITLE = %s\n", isbn, title);
			//printf("<%s> <%s>\n", isbn, title);	
				
			/* iteration of the "LENDING_ENTRIES" file to find
			 * who has borrowed the book we are currently looking at
			 */
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
			
			rewind(fp2);
			
			/* reading the length byte again for the next iteration */
			read(filed[0], &lb, 1);
		}
		
		close(filed[0]);
		exit(0);
	}
	
	fclose(fp2);
	fclose(fp);
}

int main(int argc, char *argv[]) {
	
	int choice; /* holds menu choice of user */
	char c[2];

	while(1) {
		
		printf("\n1. Book Entry\n2. Lending Entry\n3. Print Book Statuses\
				\n4. Exit\n\n");
		
		fgets(c, sizeof(c), stdin); 
		choice = atoi(c);
		
		switch(choice)
		{
			case 1:	bookentry();
					break;
					
			case 2: lendentry();
					break;
					
			case 3: getstats();
					break;
					
			case 4: printf("Thank you for using me\n");
					exit(0);
			
			default: printf("You did something wrong\n");  
		}
	}
	
	return 0;
	
}
