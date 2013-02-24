#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
   char buffer[20]; /* /command/ff0027 */
   char *path_info;
   char *command;
   char *color;
   char hex_red[3] = "ff";
   char hex_green[3] = "ff";
   char hex_blue[3] = "ff";
   printf("Content-type: text/plain\n\n");
   printf("Hello, World!\n");
   printf("QUERY_STRING=%s\n", getenv("QUERY_STRING"));

   path_info = getenv("PATH_INFO");
   if (path_info) {
     printf("PATH_INFO=%s\n", path_info);
     strncpy(buffer, getenv("PATH_INFO"), sizeof(buffer));
     buffer[sizeof(buffer)-1] = '\0';
     printf("buffer=%s\n", buffer);
  
     command = strtok(buffer, "/");
     if (command) {
       printf("command=%s\n", command);
    
       color = strtok(NULL, "/");
       printf("color=%s\n", color);
       if (color && (strlen(color) == 6)) {
         strncpy(hex_red, color, 2);
         hex_red[2] = '\0';
         strncpy(hex_green, color+2, 2);
         hex_green[2] = '\0';
         strncpy(hex_blue, color+4, 2);
         hex_blue[2] = '\0';
         printf("RGB: %s %s %s\n", hex_red, hex_green, hex_blue);
         printf("RGB: %d %d %d\n", strtol(hex_red, NULL, 16), strtol(hex_green, NULL, 16), strtol(hex_blue, NULL, 16));
       }
     } else {
       printf("No command\n");
     }
   } else {
     printf("No PATH_INFO\n");
   }

   return 0;
}