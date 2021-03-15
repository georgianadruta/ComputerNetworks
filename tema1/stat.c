-#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


/*Afisarea informatiilor precum:
 - numele directorului/fisierului,
 - Tipul,
 - i-node-ul;
 - proprietarul (uid, gid)
 - numarul de block-uri alocate
*/

int afisare_fileinfo(char* nume)
{
    struct stat st;
    int result = 0;

    if(stat(nume, &st) != 0 )
    {
        fprintf(stderr,"Eroare la stat pentru %s .\t", nume);  
        perror("Cauza este");
        return -1;   
    }

    printf("Nume: %s\n",nume);
    printf("\tTipul acestui fisier: ");
    switch(st.st_mode & S_IFMT)
    {
        case S_IFDIR : printf("Director\n"); break;
        case S_IFREG : printf("Fisier obisnuit\n"); break;
        case S_IFLNK : printf("Link\n"); break;
        case S_IFIFO : printf("FIFO\n"); break;
        case S_IFSOCK: printf("Socket\n"); break;
        case S_IFBLK : printf("Block device\n"); break;
        case S_IFCHR : printf("Character device\n"); break;
        default: printf("Unknown file type");
    }
    printf("\tAre i-node-ul: %ld\n", (long) st.st_ino);
    printf("\tProprietarul: UID = %ld  GID = %ld\n", (long) st.st_uid, (long) st.st_gid);
    printf("\tBlock-uri alocate: %lld\n", (long long) st.st_blocks);
}


int main(int argc, char *argv[])
{
    struct stat st;

    if(argc == 1)
    {
         fprintf(stderr,"Usage: %s precizati directorul/fisierul de procesat!\n", argv[0]);
         exit(1);
    }

    if( stat(argv[1], &st) != 0 )
    {
        fprintf(stderr,"Eroare la stat pentru %s .\t", argv[1]);  
        perror("Cauza este");
        exit(2);
    }

    afisare_fileinfo(argv[1]);

    return 0;
}





