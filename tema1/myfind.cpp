#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctime>     
#include <errno.h>   
#define PATH_MAX 1064

void find(char* path, char* target)
{
    DIR* dir = opendir(path); //deschidem directorul cu ajutorul tipului de date DIR
    if (NULL == dir) { fprintf(stderr, "Eroare deschidere director %s .\t", path);  perror("Cauza este"); exit(2); }

    struct dirent* de;    //structura folosita pentru parcurgerea directorului curent dir
    char nume[PATH_MAX];  //variabila auxiliara folosita ulterior

    while (NULL != (de = readdir(dir)))   //se citeste fiecare element
    {
        struct stat st;
        if (strcmp(target, de->d_name) == 0)
        {
            printf("Am gasit fisierul cautat la calea: %s\n", nume);
            stat(nume, &st);
            printf("Cateva informatii despre fisierul gasit:\ninode number: %d\nuser ID of owner: %d\ntotal size, in bytes: %d\ntime of last access: %s\n\n", st.st_ino, st.st_uid, st.st_size, ctime(&st.st_atime));
        }
        else
            if (strcmp(de->d_name, ".") && strcmp(de->d_name, ".."))  /* ignorăm intrările implicite "." și ".." din orice director */
            {

                if (strcmp(path, "./") == 0)
                    sprintf(nume, "%s%s", path, de->d_name);  /* construim calea nouă, ca și concatenare de string-uri */
                else
                    sprintf(nume, "%s/%s", path, de->d_name);  /* construim calea nouă, ca și concatenare de string-uri */

                stat(nume, &st);
                if (S_ISDIR(st.st_mode)) //ne folosim de macroul S_ISDIR pentru a verifica daca fisierul este un director 
                    find(nume, target);     //caz in care apelam iarasi functia find pentru acel director simuland comportamentul recursiv al comenzii find
            }
    }
    closedir(dir);
    return;
}

int main(int argc,char ** argv)
{
if(argc<2){fprintf(stderr,"Numar invalid de arugmente.   ");perror("Cauza este:  ");exit(1);}

find(".",argv[1]);
return 0;
}

