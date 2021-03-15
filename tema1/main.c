#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LENGTH 50
#define MAX_BUFFER 1000

void redirect_stdout()
{
	int fd = open("/dev/tty", O_WRONLY);
	close(1);
	dup2(fd, 1);
	close(fd);
}

void reset_array(char * msg)
{
	for (int i = 0; i < strlen(msg); ++i)
		msg[i] = 0;
}

void extract_arg(char *comanda, int numar, char *result)
{	
	int pos = 1;
	int i = 0;
	int len = strlen(comanda);
	int flagSpace = 0;

	while (pos < numar && i < len)
	{
		if (comanda[i] == ' ')
			flagSpace = 1;
		else
		{
			if (flagSpace == 1)
			{
				pos++;
			}
			flagSpace = 0;
		}
		i++;	
	}

	int k = 0;
	while (comanda[i] != ' ' && i < len)
	{
		result[k++] = comanda[i++];
	}
	result[k] = 0;
}

int no_args(char* comanda)
{
	int i = 0;
    int args = 1;

    while(comanda[i] != '\0')
    {
        if(comanda[i] == ' ' || comanda[i] == '\n' || comanda[i] == '\t')
        {
            args++;
        }

        i++;
    }
    return args;
}

void parsing(char message[], char parsed[][MAX_LENGTH])
{
	char *pch = strtok(message, " ");
	int i = 0;
	while(pch != NULL)
	{
		strcpy(parsed[i++], pch);
		pch = strtok(NULL, " ");
	} 
	parsed[i][0] = 0;
}

/*ALTE FUNCTII CARE AR MAI FI DE IMPLEMENTAT
(INAFARA DE cele pt erori: 
fork, read_pipe, write_pipe, 
init_pipe, init_fifo, init_sockp )
remove_extra_spaces, parsing)
*/

int main()
{
	
	char message[MAX_LENGTH];
	char result[MAX_BUFFER]; 

	int login;
	int quit;
	int l; //lungimea sirului

	pid_t pid1, pid2; // fork

	int pipe_p_sc[2], pipe_sc_p[2]; // socketpair

	if( pipe(pipe_p_sc) == -1)
	{
		fprintf(stderr,"Eroare: nu pot crea canalul anonim, errno=%d\n",errno);  
		perror("Cauza erorii");  
		exit(1);
	}
	if( pipe(pipe_sc_p) == -1)
	{
		fprintf(stderr, "Eroare: nu pot crea canalul anonim, errno=%d\n",errno);  
		perror("Cauza erorii");  
		exit(2);
	}

	//login-ul

	if((pid1 = fork()) == -1)
	{
		perror("Eroare la crearea fork.\n"); 
	    exit(4);
	}

	if(pid1 != 0)
	{	//in parinte
		login = 0;
		quit = 0;
		close(pipe_p_sc[0]);
		close(pipe_sc_p[1]);

		while(quit != 1)
		{
			reset_array(message);
			reset_array(result);
	    	//printf("Hello! Introduceti username-ul, dupa formatul login [username]: ");

	    	fgets(message, MAX_LENGTH, stdin);
	    	
	    	char *newline;
	    	if((newline = strchr(message, '\n')) != NULL)
	    		*newline ='\0';

	    	l = strlen(message);

	    	//se scrie lungimea mesajului
	    	if(write(pipe_p_sc[1], &l, sizeof(int)) == -1)
	    	{
	    		perror("Eroare la scriere");
	    		exit(5);
	    	} 
	    	//se scrie mesajul
	    	if(write(pipe_p_sc[1], &message, l) == -1)
	    	{
	    		printf("Eroare la scriere");
	    		exit(5);
	    	}

	    	l = strlen(result);
	    	//se citeste lungimea mesajului
	    	if(read(pipe_sc_p[0], &l, sizeof(int)) == -1)
	    	{
	    		printf("Eroare la citire");
	    		exit(6);
	    	}
	    	//se citeste mesajul
	    	if(read(pipe_sc_p[0], &result, l) == -1)
	    	{
	    		printf("Eroare la citire");
	    		exit(7);
	    	}

	    	result[l] = '\0';
	    	printf("%s\n", result);

	    	if(strcmp(message, "exit") == 0) 
	    		quit = 1;
	    	else
	    	{
	    		if(login == 0 && strstr(result, "Bine ati venit!") != 0)
	    			login = 1;
	    		else

    			if(strcmp(result, "Logout") == 0)
    				login = 0;
    			reset_array(result);

			}

		}

		strcpy(result, "quit\0");
		l = strlen(result);
		if(write(pipe_p_sc[1], &l, sizeof(int)) == -1)
    	{
    		perror("Eroare la scriere");
    		exit(12);
    	}
    	if(write(pipe_p_sc[1], &result, l) == -1)
    	{
    		printf("Eroare la scriere");
    		exit(13);
    	}
    	int codTerm;
    	wait(&codTerm);
    	close(pipe_p_sc[1]);
		close(pipe_sc_p[0]);

	}

	else
	{ // in spawner child
		close(pipe_p_sc[1]);
		close(pipe_sc_p[0]);

		l = 0;
		login = 0;
		quit = 0;

		while(quit != 1)
		{
			reset_array(message);
			//se citeste lungimea mesajului
			if(read(pipe_p_sc[0], &l, sizeof(int) ) == -1)
			{
				perror("Eroare la citire");
				exit(14);
			}
			//se citeste mesajul
			if(read(pipe_p_sc[0], &message, l ) == -1)
			{
				perror("Eroare la citire");
				exit(15);
			}
			message[l] = '\0';

			//eliminarea spatiilor suplimentare
			int noArgs = no_args(message);
			char firstArg[MAX_LENGTH / 4];
			extract_arg(message, 1, firstArg);
			reset_array(result);

			if(strcmp(firstArg, "quit") == 0)
			{
				strcpy(result, "Exit\0");
				quit = 0;
			}

			else

			if(login == 0)
			{
				if(strcmp(firstArg, "login") == 0)
				{
					if(noArgs != 2)
						strcpy(result, "Pentru logare: tastati login urmat de username \0");
					else
					{
						FILE *fd = fopen("usernames.txt", "r");
						if(fd == NULL)
						{
							perror("Eroare la deschiderea fisierului usernames.txt\n"); 
							exit(16);
						}
						char currUser[MAX_LENGTH / 4];
						char user[MAX_LENGTH / 4];
						extract_arg(message, 2, user);
						int found = 0;

						while (fgets(currUser, MAX_LENGTH, fd) > 0 && found == 0)
						{	
							if(currUser[strlen(currUser) - 1] == '\n')
								currUser[strlen(currUser) - 1] = 0;

							if(strcmp(user, currUser) == 0)
								found = 1;
						}

						if(found == 1)
						{
							strcpy(result, "Bine ati venit, ");
							strcat(result, user);
							printf("Am ajuns aici");
							login = 1;
						}
						else
						{
							strcpy(result, "Username invalid: ");
							strcat(result, user);
						}
					}
				}
			//se mai poate adauga si logout-ul
			else
				strcpy(result, "Trebuie sa va logati ca sa puteti executa comenzi!");

			}

			//s-a realizat logarea
			else
			{
				l = MAX_BUFFER;
				if(strcmp(firstArg, "myfind") == 0 || strcmp(firstArg, "mystat") == 0)
				{
					int sockp[2];
					int codTerm;
					if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0) 
    				{ 
      					perror("Eroare la crearea socketpair."); 
      					exit(17); 
    				}
			    	if((pid2 = fork()) == -1)
			        {
			            perror("Eroare la crearea fork.\n"); 
			            exit(18);
			        }
			        if(pid2 == 0)
			        {
			        	//copil
			        	close(pipe_p_sc[0]);
			            close(pipe_p_sc[1]);
			            close(pipe_sc_p[0]); 
			            close(pipe_sc_p[1]);
			            
			            //redirectez stdout in /dev/tty
			            close(1);
			            dup2(sockp[1], 1);
			            close(sockp[1]);

			           	if(strcmp(firstArg, "mfind") == 0)
			           	{
			           		const char pathToMyFind[MAX_LENGTH / 2] = "./myfind.bin";
			           		if(noArgs > 3)
			           		{
			           			strcpy(result, "Numar invalid de argumente\0");
			           			write(1, &result, strlen(result));
			           		}
			           		else
			           		if(noArgs == 1)
			           			execlp(pathToMyFind, pathToMyFind, NULL);
			           		else
			           		if(noArgs == 2)
			           		{
			           			char filename[MAX_LENGTH/4];
			           			extract_arg(message, 2, filename);
			           			execlp(pathToMyFind, pathToMyFind, filename, NULL);
			           		}
			           		else
			           		{
			           			char filename[MAX_LENGTH /4];
			           			extract_arg(message, 3, filename);
			           			char path[MAX_LENGTH/4];
			           			extract_arg(message, 2, path);
			           			execlp(pathToMyFind, pathToMyFind, path, filename, NULL);
			           		}
			           	}
			           	else
			           	{
			           		const char pathToMyStat[MAX_LENGTH / 4] = "./mystat.bin" ;
			           		if(noArgs != 2)
			           			execlp(pathToMyStat, pathToMyStat, NULL);
			           		else
		           			{
		           				char filename[MAX_LENGTH / 4];
		           				extract_arg(message, 2, filename);
		           				execlp(pathToMyStat, pathToMyStat, filename, NULL);
		           			}
			           	}
			           	return -1;
			        }
			        else
			        {
			        	close(sockp[1]);
			        	wait(&codTerm);
			        	if(read(sockp[0], &result, l) < 0)
			        	{
			        		perror("Eroare la citire din sockp");
			        		exit(19);
			        	}
			        	close(sockp[0]);
			        }
			        redirect_stdout();

				}
				else
				//alte comenzi (cd, logout)
				{
					int codTerm;
					if(-1 == mkfifo("fifo_exec_sc", 0600) )
			        {
			            if(errno != EEXIST)   // errno=17 for "File already exists"
			            {
			                perror("Eroare la crearea canalului fifo. Cauza erorii");  
			                exit(20);
			            }
			        }
			        if(pid2 == 0)
			        {
			        	//exec child
				        close(pipe_p_sc[0]);
			            close(pipe_p_sc[1]);
			            close(pipe_sc_p[0]); 
			            close(pipe_sc_p[1]);

			            int fout = open("fifo_exec_sc", O_WRONLY);

			            //redirectez stdout in fifo
			            close(1);
			            dup2(fout, 1);
			            close(fout);

			            char parsed[MAX_LENGTH][MAX_LENGTH];
			            parsing(message, parsed);
			            execlp(parsed[0], (const char*) parsed, NULL);

			            return -1;
			        }
			        else
			        {
			        	//spawner child
			        	int fin = open("fifo_exec_sc", O_RDONLY);
			        	wait(&codTerm);
			        	if(read(fin, &result, l) < 0)
			        	{
			        		perror("Eroare la citire");
			        		exit(21);
			        	}
			        	close(fin);
			        }
			        redirect_stdout();

				}

			}
		
			l = strlen(result);
			if(write(pipe_sc_p[1], &l, sizeof(int)) == -1)
	    	{
	    		printf("Eroare la scriere");
	    		exit(22);
	    	}
			
			if(write(pipe_sc_p[1], &result, l) == -1)
	    	{
	    		printf("Eroare la scriere");
	    		exit(23);
    		}
		}

		int l;
		if(read(pipe_p_sc[0], &l, sizeof(int)) == -1)
		{
			printf("Eroare la scriere");
			exit(25);
		}
		if(read(pipe_p_sc[0], &result, l) == -1)
		{
			printf("Eroare la scriere");
			exit(26);
		}
		close(pipe_p_sc[0]);
		close(pipe_sc_p[1]);
	}
	return 0;
}
