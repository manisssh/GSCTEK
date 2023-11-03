#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *strlwr(char *s) {
    for (int i = 0; i<strlen(s); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			/* Alphabets in ASCII
			------------------------------------
			A = 65, B = 66, C = 67, .... Z = 90
			a = 97, b = 98, c = 99, .... z = 122
			------------------------------------
				common difference = (97 - 65)
								= 32
			*/
			s[i] += 32;
		}
	}
    return s;
}

int copyFile(char *destination, char *source) {
    FILE *dst = fopen(destination, "a");
	if(dst == NULL) {
		return 1;
	}
    FILE *src = fopen(source, "r");
	if(src == NULL) {
		return -1;
	}

	char ch = getc(src);
	while(ch != EOF) {
        putc(ch, dst);
		ch = getc(src);
    }

    fclose(dst);
	fclose(src);
    return 0;
}

int generateCheckerFile(char *leftExpression, char *rightExpression, char *generalSolution) {
	/*
	1. Start
	2. Make a fresh temp file.
	3. Copy the contents of header.
	4. Copy the equation and its solution
		a. Left hand side,
		b. Right hand side,
		c. General solution in radian.
	5. Copy the contents of footer.
	6. Stop.
	*/

	//2. Begin
	FILE *fptr = fopen("temp.c", "w");
	if(fptr == NULL) {
		return 1;
	}
	fclose(fptr);
	//2. End

	//3. Begin
    int status = copyFile("temp.c", "header.part.c");
	if(status != 0) {
		return status;
	}
	//3. End

	//4. Begin
    fptr = fopen("temp.c", "a");
	if(fptr == NULL) {
		return 1;
	}
    fprintf(fptr, "\n\ndouble leftExpression(float x) { return %s; }\n", leftExpression);
    fprintf(fptr, "\ndouble rightExpression(float x) { return %s; }\n", rightExpression);
    fprintf(fptr, "\nfloat generalSoulution(int n) { return %s; }\n\n", generalSolution);
    fclose(fptr);
	//4. End

	//5. Begin
    status = copyFile("temp.c", "footer.part.c");
	if(status != 0) {
		return status;
	}
	//5. End

    return 0;
}

int solutionCheck() {
	#ifdef _WIN32
		system("cls");
		system("gcc temp.c -o temp");
		return system("temp.exe");
	#else
		system("clear");
		system("gcc temp.c -o temp");
		return system("./temp");
	#endif
}

int argsHandler(int argc, char **argv) {
	/* Switches for Debugging
	-----------------------------------------------------------------
	1. [-kc]					: keep temp C file
	2. [-ke]					: keep temp executable
	3. [-ke -kc] or [-kc -ke]	: keep C file as well as executable
	4. unknown switches			: delete C file as well as executable
	5. default switch			: delete C file as well as executable
	-----------------------------------------------------------------
	*/
	if(argc == 2) {
			if(strcmp((*argv)+1, "-kc") == 0) {
				#ifdef _WIN32
					remove("temp.exe");
				#else
					remove("temp");
				#endif	
				return 101;
			}
			else if(strcmp((*argv)+1, "-ke") == 0) {
				remove("temp.c");
				return 110;
			}
			else if(strcmp((*argv)+1, "-ka") == 0) {
				return 111;
			}
			else {
				return 100;
			}
		}
	else if(argc == 3) {
		if(strcmp((*argv)+1, "-kc") == 0 && strcmp((*argv)+2, "-ke") == 0) {
			return 111;
		}
		else if(strcmp((*argv)+2, "-kc") == 0 && strcmp((*argv)+1, "-ke") == 0) {
			return 111;
		}
		else {
			return 100;
		}
	}
	else {
		remove("temp.c");
		#ifdef _WIN32
			remove("temp.exe");
		#else
			remove("temp");
		#endif	
		return 0;
	}
}

int main(int argc, char *argv[]) {
    char *LHS = malloc(256), *RHS = malloc(256), *sol = malloc(256);
    
	printf("\nUsing 'x' as the angle of trigonometric functions,\n");
    printf("Enter LHS of the equation: "); scanf("%255s", LHS);
    printf("Enter RHS of the equation: "); scanf("%255s", RHS);

	printf("\nUsing 'n' as variable of general solution,\n");
    printf("Enter the general solution you have: "); scanf("%255s", sol);
    
	int status = generateCheckerFile(strlwr(LHS), strlwr(RHS), strlwr(sol));
	
	if(status == -1) {
		printf("Can't use template files.\n");
		printf("They might have been deleted, placed somewhere else or are being used by another process!\n");
		return -1;
	}    
	else if(status == 1) {
		printf("Can't create necessary files.\n");
		printf("Maybe the program is not running with write previlages!\n");
		return 1;
	}
	else {
		if(solutionCheck() == 0) {
			printf("\"x = %s\" will satisfy \"%s = %s\"\n", sol, LHS, RHS);
			printf("Where,\n");
			printf("     n is an integer whose range is [-100 < n < 100].\n");
		}
		else {
			printf("\"x = %s\" will not satisfy \"%s = %s\"\n", sol, LHS, RHS);
			printf("Where,\n");
			printf("     n is an integer whose range is [-100 < n < 100].\n");
		}
		return argsHandler(argc, argv);
	}
}