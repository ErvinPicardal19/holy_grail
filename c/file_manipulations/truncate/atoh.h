#ifndef ATOH_H
#define ATOH_H

/*
* Convert ASCII to hexadecimal format.
* ----  
*
* This function converts the characters of len LEN in the 
* text TEXT buffer to their hexadecimal format as an array 
* of characters.
* ----
* 
* DO NOT FORGET TO FREE THE RETURN
*/
char* atoh(unsigned char const *text, unsigned int len);

#endif //ATOH_H
