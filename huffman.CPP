
//	This program compress a file using huffman lossless compression technique and
//	decompress a previously compressed file.


/*-------------------------- HEADER FILES--------------------------- */

		# include <stdio.h>
		# include <string.h>
		# include <stdlib.h>
		# include <alloc.h>
		# include <conio.h>


/*-------------------------LIST OF MACROS ----------------------------*/

		# define BUFSIZE    1024
		# define CODESIZE     24
		# define PATHSIZE     61


/*-------------------------- GLOBAL VARIABLES -----------------------*/

		typedef
			struct link {
				unsigned long val;
				unsigned char code[CODESIZE];
				struct link *left, *right;
			} tree;

		tree *a[257]={NULL};
		unsigned long fre[257]={0}, Total=0, total;
		unsigned char *code[256]={'\x0'};
		FILE  *f1, *f2;


/*------------------------ FUNCTIONS DECLARATION ---------------------*/

		void mdcode(tree *);
		void Compression(void);
		void deletecell(tree **);
		void Decompression(void);
		unsigned int search(void);
		void compress_save_file(void);
		unsigned int count_char(void);
		unsigned int  read_file_char(void);
		void decompress_save_file(unsigned int );
		void initialize(unsigned char [], unsigned int);
		unsigned int build_tree_structure(unsigned int );


/*-----------------------MAIN FUNCTION CODE -------------------------*/

void main( void )
{
	char ch;
	clrscr();

	printf("\n\t\t1. Compression.");
	printf("\n\t\t2. DeCompression.");
	printf("\n\n\tMake your choice by pressing '1' or '2' only :: ");

	do {
		ch = getch();

		if(ch == '1') {
			putch(ch);
			Compression();
		}
		if(ch == '2') {
			putch(ch);
			Decompression();
		}
	} while((ch != '1') && (ch != '2'));
}


/*------------------------COMPRESSION FUNCTION----------------------*/

 void Compression(void)
 {
	unsigned int i, count;

	count = read_file_char();

	i = build_tree_structure(count);

	printf("\n\tGenerating codes...");
	mdcode(a[i]);
	printf("\tDone.");

	compress_save_file();

	deletecell(&a[i]);

	printf("\n\n\t\tCompression completed. Press any key to return...");
	getch();

}


/* --------------------------FUNCTION SEARCH---------------------------*/

unsigned int search(void)
{

	// 	This function search the first zero value in the array b

	unsigned int i=0;

	while (fre[i]) i++;

	return i;
 }


/*-----------------------FUNCTION INITIALIZE-----------------------*/

void initialize(unsigned char buf[],unsigned int j)
{

	//	This function initialize the array buf with NULL value */

	unsigned int i=0;

	for(;i<j;) buf[i++]='\x0';
}


/*-----------------------READ_FILE_CHAR_FUNCTION-------------------*/

unsigned int read_file_char(void)
{

	//	This function read frequance of all character in a file.
	//	This frequencey is stored in array fre and is used in building
	//	HUFFMAN tree and HUFFMAN CODE.  */

	unsigned char buf[BUFSIZE] = {'\X0'};
	unsigned char sf[PATHSIZE], df[PATHSIZE]={'\x0'};
	unsigned int loc, i, count=0, l;
	unsigned int max = 65535, fre2[256] = {0};

	printf("\n\n\t\tGive the source filename :: "); gets(sf);

    printf("\n\tOpening file...");

	if((f1 = fopen(sf, "rb")) == NULL) {
		printf("\n\tError! File does not exist. Press any key to return...");
	getch();
		exit(0);
	}
	printf("\tDone.");

    printf("\n\tCounting characters...");

    loc = fread(buf, 1, BUFSIZE, f1); Total += loc;

	while(loc > 0) {

		for(l = 0; l<loc; l++) {
			i = buf[l];

			if(!fre[i]) {

				a[i]=(tree *)malloc(sizeof(tree));

				fre[i] = 1;
				count++;
				a[i]->val = 1;
				a[i]->code[0] = '\x0';
				a[i]->left = NULL;
				a[i]->right = NULL;
			}
			else {
				a[i]->val++;
				fre[i]++;
			}
		}
		loc = fread(buf, 1, BUFSIZE, f1); Total += loc;
	}
	printf("\tDone.");
    printf("\n\tThe file contains %ld bytes", Total);

	l = 1;
	for(i=0;i<256;i++) if(fre[i] > max) {l = 0; break;}

	printf("\n\n\t\tGive destination filename :: "); gets(df);

    printf("\n\tCreating file...");
	if((f2 = fopen(df, "wb")) == NULL) {
		printf("\n\tError! File can\'t be created. Press any key to return...");
		getch();
		exit(0);
    }
    printf("\tDone.");

	if(l) {
		fputc('I', f2);
		for(i=0;i<256;i++) fre2[i] = fre[i];
		loc = fwrite(fre2, sizeof(unsigned int), 256, f2);
		total = 256 * sizeof(unsigned int) + 1;
	}
	else {
		fputc('L', f2);
		loc = fwrite(fre, sizeof(unsigned long), 256, f2);
		total = 256 * sizeof(unsigned long) + 1;
	}

	if(loc != 256) {
		gotoxy(36,12);puts(" MESSAGE ");gotoxy(12,14);
		printf("can\'t write in Drive C .");
		getch();

		exit(1);
	}
	return count;
 }

 /*----------------------FUNCTION BUILD_TREE_STRUCTURE-----------------*/

 unsigned int build_tree_structure(unsigned int count)
 {

 /* This function build tree structure to generate HUFFMAN CODE */
	unsigned int i,j,x,y;
	unsigned long m1,m2;
	tree *p,*q;

    printf("\n\tBuilding tree structure...");
	while (count>1) {
		m1=m2=4294967295;

		for(i=0;i<257;i++) {
			if(fre[i]) {
				if(m1 > a[i]->val) {
					m1 = a[i]->val;
					y=i;
				}
			}
		}

		for(i=0;i<257;i++) {
			if(fre[i]) {
				if(i  != y) {
					if(m2 >a[i]->val) {
						m2 = a[i]->val;
						x=i;
					}
				}
			}
		}

		j=search();
		fre[j]=1;

		a[j]=(tree*)malloc(sizeof(tree));

		a[j]->val = (a[x]->val + a[y]->val);
		a[j]->code[0]='\x0';

		p = (tree *)malloc(sizeof(tree));

		q = (tree *)malloc(sizeof(tree));

		p->val = a[x]->val;
		q->val = a[y]->val;

		p->code[0] = '\x0';
		q->code[0] = '\x0';

		p->left = a[x]->left;
		p->right = a[x]->right;

		q->left = a[y]->left;
		q->right = a[y]->right;

		a[j]->left=p;
		a[j]->right=q;

		if((a[x]->left == NULL) && (a[x]->right == NULL)) p->val=x;
		if((a[y]->right == NULL) && (a[y]->left == NULL)) q->val=y;

		fre[y] = fre[x] = 0;

		free(a[x]);
		free(a[y]);

		count--;
	}
    printf("\tDone.");
	return j;
 }

  /*------------------------FUNCTION MDCODE--------------------*/

 void mdcode(tree *root)
 {

 /*This function generate code and store in an array */

	tree *p, *q;

	if((root->left == NULL) && (root->right == NULL ))
		code[root->val] = root->code;

	else {
		p=root->left;
		q=root->right;

		strcpy(p->code,root->code);
		strcpy(q->code,root->code);

		strcat(p->code,"0");
		strcat(q->code,"1");

		mdcode(p);
		mdcode(q);
	}
 }

 /*------------------------FUNCTION DELETE--------------------*/

 void deletecell(tree **root)
 {

 /*This function delete tree by traversing it postorder */

	if(*root != NULL) {
		deletecell(&((*root)->left));
		deletecell(&((*root)->right));
		free(*root);
	}
 }


/*---------------------- COMPRESS_SAVE_FILE FUNCTION----------------  */

void compress_save_file(void)
{

	//	This function compress a file by reading it and replacing all */
	//	character by its HUFMAN CODE and then save it .     */

	unsigned char buf1[BUFSIZE] = {'\X0'}, buf2[BUFSIZE] = {'\x0'}, ch;
	unsigned int i, j=0, x=8, y, loc, l, m1, m2;


    printf("\n\tCompressing and saving file...");
	rewind(f1);

	loc = fread(buf1, 1, BUFSIZE, f1);

	while(loc > 0) {

		for(l = 0; l<loc; l++) {
			i = buf1[l];
			m1 = strlen(code[i]);

			for(m2 = 0; m2 < m1; m2++) {
				y = *(code[i] + m2);
				if (y-48) ch = '\x1';
				else ch = '\x0';
				ch <<= --x;
				buf2[j] |= ch;

				if(!x) {
					x=8;j++;

					if(j == BUFSIZE) {
						j = fwrite(buf2,1,BUFSIZE,f2); total += j;
						if(j != BUFSIZE) {
							printf("\nError in writing. Press any key to return...");
							getch();
							exit(0);
						}
						j=0;
						initialize(buf2,BUFSIZE);
					}
				}
			}
		}

		loc = fread(buf1, 1, BUFSIZE, f1);
	}

	if(x != 8) j++;

	loc = fwrite(buf2, 1, j, f2); total += j;

	if(loc != j) {
		printf("\nError in writing. Press any key to return...");
		getch();
		exit(0);
	}
	printf("\tDone.");
	printf("\n\tCompressed file contains %ld bytes", total);
	fclose(f1);
	fclose(f2);

}

/*----------------------Decompression INSTALLF_H-----------------------*/

void Decompression(void)
{

	unsigned int i, count;

	count = count_char();

	i = build_tree_structure(count);

    printf("\n\tGenerating codes...");
	mdcode(a[i]);
	printf("\tDone.");

	decompress_save_file(i);

	deletecell(&a[i]);

	printf("\n\n\t\tDecompression completed. Press any key to return...");
	getch();

}


/*-------------------------FUNCTION COUNT_CHAR----------------------*/

unsigned int count_char(void)
{

	//	This function counts the characters in a file

	unsigned int count, i, fre2[256] = {0};
	unsigned char sf[PATHSIZE];
	char ch;

	printf("\n\n\t\tGive Source filename :: "); gets(sf);

    printf("\n\tOpening file...");
	if((f1 = fopen(sf, "rb")) == NULL) {
		printf("\n\tError! File does not exist. Press any key to return...");
		getch();
		exit(0);
    }
	printf("\tDone.");

    printf("\n\tReading file...");
	ch = fgetc(f1);

	if(ch == 'I') {
		fread(fre2, sizeof(unsigned int), 256, f1);
		total = 256 * sizeof(int) + 1;
		for(i = 0; i<256; i++) fre[i] = fre2[i];
            
	}

	else {
		if(ch == 'L') {
			fread(fre, sizeof(unsigned long), 256, f1);
			total = 256 * sizeof(long int) + 1;
        }
			else {
				printf("\n\nERROR! File is not in correct format. Aborting...");
				exit(0);
			}
	}

	count = 0;

	for(i=0;i<256;i++) {
		if(fre[i]){
			a[i] = (tree *)malloc(sizeof(tree));
			a[i]->val=fre[i];
			a[i]->code[0]='\x';
			a[i]->left=NULL;
			a[i]->right=NULL;
			count++;
		}
	}
    printf("\tDone.");
	return count;
}

/*---------------------FUNCTION DECOMPRESS_SAVE_FILE------------------*/

void decompress_save_file(unsigned int i)
{
	//	This function decompressed a compressed file and then save it.

	unsigned char buf1[BUFSIZE] = {'\X0'}, buf2[BUFSIZE] = {'\x0'}, ch;
	unsigned int j, loc, l, x;
	unsigned char df[PATHSIZE];
	tree *tamp;

	
	printf("\n\n\t\tGive destination filename :: "); gets(df);

	printf("\n\tCreating file...");
	f2 = fopen(df, "wb");
    printf("\tDone.");

    printf("\n\tDecompressing and saving file...");

	tamp = a[i];

	j = 0;

	while((loc = fread(buf1,1,BUFSIZE,f1)) > 0) {
        total += loc;
    	
		for(l = 0; l<loc; l++) {

			for(x=8;x>0;) {
				ch='\x1';
				ch <<= --x;
				if(buf1[l] & ch) tamp=tamp->right;
				else tamp=tamp->left;

				if((tamp->left==NULL) && (tamp->right==NULL)){
					buf2[j++] = (unsigned char)(tamp->val);

					if(j == BUFSIZE) {
						j = fwrite(buf2,1,BUFSIZE,f2); Total += j;
						if(j != BUFSIZE) {
							printf("\nError in writing. Press any key to return...");
							getch();
							exit(0);
						}
						j=0;
					}
					tamp=a[i];
				}
			}
		}

	}
	loc = fwrite(buf2,1,j,f2); Total += loc;

	if(loc != j) {
		printf("\nError in writing. Press any key to return...");
		getch();
		exit(0);
	}
	printf("\tDone.");

	printf("\n\tCompressed file contains %ld bytes.", total);
	printf("\n\tDecompressed file contains %ld bytes.", Total);


	fclose(f1);
	fclose(f2);
}
