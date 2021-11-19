#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main () {
   char str[80] = "A:100";
   const char s[2] = ":";
   char *token;
   
   /* get the first token */
   token = strtok(str, s);
   int cont = 0;
   
   /* walk through other tokens */
   while( token != NULL ) {
      printf( " %s\n", token );
        if (cont == 1){
            int poder = atoi(token);
            poder ++;
            printf("%i\n", poder);
        }
        
       cont ++;
    
      token = strtok(NULL, s);
   }
   
   return(0);
}