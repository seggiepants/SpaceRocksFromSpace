#include <stdio.h>
 
int main()
{
    char buffer[50];
    const char* s = "geeksforgeeks";
 
    // Counting the character and storing
    // in buffer using snprintf
    //int j = snprintf(buffer, 6, "%s\n", s);
	//int j = sprintf(buffer, "%s\n", s);
	int j = sprintf_s(buffer, 6, "%s\n", s);
 
    // Print the string stored in buffer and
    // character count
    printf("string:\n%s\ncharacter count = %d\n",
                                     buffer, j);
 
    return 0;
}