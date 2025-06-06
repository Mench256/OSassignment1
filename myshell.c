// Abraham Menchaca, 100216

#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#define MAX_FILE_NAME 50

int main(void) {
    // Declaring struct of files
    char files[500][MAX_FILE_NAME];
    int sizes[500];
    time_t dates[500];
    struct stat fileStat;   

    pid_t child;
    DIR *d;
    struct dirent *de;
    int i, c, k;
    char s[256], cmd[256];
    time_t t;

    int rm = 0;
    int totalFiles = 0;
    int input = 0;
    int pos = 0;


    while (1) {
        t = time(NULL);
        printf("Time: %s\n", ctime(&t));
        printf("-----------------------------------------------\n");

        getcwd(s, 200);
        printf("\nCurrent Directory: %s\n", s);

        d = opendir(".");
        c = 0;
        while ((de = readdir(d))) {
            if ((de->d_type) & DT_DIR)
                printf(" ( %d Directory: %s ) \n", c++, de->d_name);
            

            totalFiles++;
        }
        int numFiles = c;
        closedir(d);

        printf("-----------------------------------------\n");

        d = opendir(".");
        c = 0;
        // This is what is printing out the files
        while ((de = readdir(d))) {
            if ((de->d_type) & DT_REG){
                // Getting time that each file was last edited
                if (stat(de->d_name, &fileStat) == 0){

                    time_t modTime = fileStat.st_mtime;
                    dates[c] = modTime;
                    c++;

                }

                FILE *newFile = fopen(de->d_name, "r");
                fseek(newFile, 0, SEEK_END);
                pos = ftell(newFile);
                fclose(newFile);
                sizes[c] = pos;

                // Using strcpy to copy files names to array
                strcpy(files[c], de->d_name);
                printf(" ( %d Size:%d File: %s Time: %s) \n", c, pos, de->d_name, ctime(&dates[c]));

            }
            if ((c % 5) == 0) {
                printf("Hit N for Next\n");
               k = getchar();
            }
        }
        closedir(d);

        printf("-----------------------------------------\n");

        c = getchar();
        getchar();

        switch (c) {
            case 'q':
                exit(0); /* quit */

            case 'e':
                printf("Edit what?:");
                scanf("%s", s);
                strcpy(cmd, "pico ");
                strcat(cmd, s);
                system(cmd);
                break;

            case 'r':
                printf("Run what?:");
                scanf("%s", cmd);
                system(cmd);
                break;

            case 'c':
                printf("Change To?:");
                scanf("%s", cmd);
                chdir(cmd);
                break;

            // Implementing remove function
            case 'w':
                for(int i = 0; i < numFiles; i++){
                    printf("File[%d]: %s\n", i, files[i]);
                }
                printf("Please enter which file you would like to delete: ");
                scanf("%d", &rm);

                if(rm >= 0 && rm < numFiles){

                    if(remove(files[rm]) == 0){
                        printf("File deleted successfully!\n");

                    }
                    else{
                        printf("File deletion unsuccessful!\n");
                    }
                
                }
                break;

            case 's':
                // Implementing Sorting
                printf("Enter 1 to sort by file size or 2 to sort by file date: ");
                scanf("%d", &input);

                if(input == 1){

                    for(int i = 0; i < numFiles; i++){
                        for(int j = i + 1; j < numFiles; j++){
                            if(sizes[i] < sizes[j]){
                                char temp[MAX_FILE_NAME];
                                strcpy(temp, files[i]);
                                int temp2 = sizes[j];
                                sizes[i] = sizes[j];
                                strcpy(files[i], files[j]);
                                sizes[j] = temp2;
                                strcpy(files[j], temp);    
                            }
                        }
                    }

                    for(int i = 0; i < numFiles; i++){
                        printf("File[%d]: %s Size: %d Time: %s\n", i, files[i], sizes[i], ctime(&dates[i]));
                    }

                }
                else{

                    for(int i = 0; i < numFiles; i++){
                        for(int j = i + 1; j < numFiles; j++){
                            if(dates[i] < dates[j]){
                                char temp[MAX_FILE_NAME];
                                strcpy(temp, files[i]);
                                time_t temp2 = dates[j];
                                dates[i] = dates[j];
                                strcpy(files[i], files[j]);
                                dates[j] = temp2;  
                                strcpy(files[j], temp);
                            }
                        }
                    }
                    for(int i = 0; i < numFiles; i++){
                        printf("File[%d]: %s Size: %d Time: %s\n", i, files[i], sizes[i], ctime(&dates[i]));
                    }


                }

                

            }
    }
}
