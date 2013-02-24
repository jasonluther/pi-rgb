#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rgbcmd.h"

int main(void)
{
   rgbcmd_t *rgbcmd;
   char buffer[20]; /* /command/ff0027 */
   char jsonbuffer[100];
   char *path_info;
   char *command;
   int want_color = 0;
   char *color;
   char hex_red[3] = "ff";
   char hex_green[3] = "ff";
   char hex_blue[3] = "ff";
   printf("Content-type: text/plain\n\n");
   printf("QUERY_STRING=%s\n", getenv("QUERY_STRING"));

   rgbcmd = open_rgbcmd(0);
   rgbcmd_json(rgbcmd, jsonbuffer, sizeof(jsonbuffer));
   printf("Original: %s\n", jsonbuffer);

   path_info = getenv("PATH_INFO");
   if (path_info) {
     printf("PATH_INFO=%s\n", path_info);
     strncpy(buffer, getenv("PATH_INFO"), sizeof(buffer));
     buffer[sizeof(buffer)-1] = '\0';
     printf("buffer=%s\n", buffer);
  
     command = strtok(buffer, "/");
     if (command) {
       printf("command=%s\n", command);

       if (!strcmp(command, "on")) {
         rgbcmd->mode = RGB_MODE_ON;
         want_color = 1;
       } else if (!strcmp(command, "off")) {
         rgbcmd->mode = RGB_MODE_OFF;
       } else if (!strcmp(command, "blink")) {
         want_color = 1;
         rgbcmd->mode = RGB_MODE_BLINK;
       } else if (!strcmp(command, "random")) {
         rgbcmd->mode = RGB_MODE_RANDOM;
       } else if (!strcmp(command, "rainbow")) {
         rgbcmd->mode = RGB_MODE_RAINBOW;
       } else {
         /* XXX return? */
       }
    
       if (want_color) {
         color = strtok(NULL, "/");
         printf("color=%s\n", color);
         if (color && (strlen(color) == 6)) {
           strncpy(hex_red, color, 2); hex_red[2] = '\0';
           strncpy(hex_green, color+2, 2); hex_green[2] = '\0';
           strncpy(hex_blue, color+4, 2); hex_blue[2] = '\0';
           rgbcmd->red = (unsigned char)strtol(hex_red, NULL, 16);
           rgbcmd->green = (unsigned char)strtol(hex_green, NULL, 16);
           rgbcmd->blue = (unsigned char)strtol(hex_blue, NULL, 16);
         }
       }
     } else {
       printf("No command\n");
     }
   } else {
     printf("No PATH_INFO\n");
   }
   rgbcmd_json(rgbcmd, jsonbuffer, sizeof(jsonbuffer));
   printf(jsonbuffer);
   printf("\n");

   return 0;
}