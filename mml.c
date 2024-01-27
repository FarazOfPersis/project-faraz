// Faraz Doagooye Tehrani 402105998
// mml stands for: my miserable life 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int globalEmailChanger(char globalEmail[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char home[1024];
    strcpy(home, getenv("HOME"));

    if(chdir(home) != 0)
        return 1;

    DIR* dir = opendir(".");
    struct dirent* entry;

    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlConfig") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        if(mkdir(".mmlConfig", 0755) != 0)
        {
            perror("An error accurred!\n");
            return 1;
        }
    }

    closedir(dir);
    
    if(chdir(".mmlConfig") != 0)
    {
        printf("an error accured!\n");
        return 1;
    }

    FILE * file = fopen("globalEmail.txt", "w");
    fprintf(file, "(global)email : %s\n", globalEmail);
    fclose(file);
    
    chdir(cwd);
    return 0;
}

int globalEmailFinder(char globalEmail[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char home[1024];
    strcpy(home, getenv("HOME"));

    if(chdir(home) != 0)
        return 1;

    DIR* dir = opendir(".");
    struct dirent* entry;

    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlConfig") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        perror("You haven't yet set an email!\n");
        return 1;
    }

    closedir(dir);
    
    if(chdir(".mmlConfig") != 0)
    {
        printf("an error accured!\n");
        return 1;
    }

    FILE * file = fopen("globalEmail.txt", "r");
    fscanf(file, "(global)email : %s", globalEmail);
    fclose(file);
    
    chdir(cwd);
    return 0;


}

int globalUsernameChanger(char globalUsername[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char home[1024];
    strcpy(home, getenv("HOME"));

    if(chdir(home) != 0)
        return 1;

    DIR* dir = opendir(".");
    struct dirent* entry;

    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlConfig") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        if(mkdir(".mmlConfig", 0755) != 0)
        {
            perror("An error accurred!\n");
            return 1;
        }
    }

    closedir(dir);
    
    if(chdir(".mmlConfig") != 0)
    {
        printf("an error accured!\n");
        return 1;
    }

    FILE * file = fopen("globalUsername.txt", "w");
    fprintf(file, "(global)Username : %s\n", globalUsername);
    fclose(file);
    
    chdir(cwd);
    return 0;
}

int globalUsernameFinder(char globalUsername[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char home[1024];
    strcpy(home, getenv("HOME"));

    if(chdir(home) != 0)
        return 1;

    DIR* dir = opendir(".");
    struct dirent* entry;

    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlConfig") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        perror("You haven't yet set a Username!\n");
        return 1;
    }

    closedir(dir);
    
    if(chdir(".mmlConfig") != 0)
    {
        printf("an error accured!\n");
        return 1;
    }

    FILE * file = fopen("globalUsername.txt", "r");
    fscanf(file, "(global)Username : %s", globalUsername);
    fclose(file);
    
    chdir(cwd);
    return 0;


}

int create_configs(char *username, char *email)
{
    FILE *file = fopen(".mml/config", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    
    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "last_commit_ID: %d\n", 0);
    fprintf(file, "current_commit_ID: %d\n", 0);
    fprintf(file, "branch: %s\n", "master");

    fclose(file);
    
    if (mkdir(".mml/commits", 0755) != 0) 
    {    
        perror("unable to create the repository!\n");
        return 1;
    }

    if (mkdir(".mml/files", 0755) != 0) 
    {
        perror("unable to create the repository!\n");
        return 1;
    }

    file = fopen(".mml/staging", "w");
    fclose(file);

    file = fopen(".mml/tracks", "w");
    fclose(file);

    return 0;
}

int init(int argc, char * const argv[], char username[], char email[]) 
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char tmp_cwd[1024];
    int exists = 0;
    struct dirent *entry;
    
    do 
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening current directory!\n");
            return 1;
        }
        int flag = 0;
        while ((entry = readdir(dir)) != NULL) 
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mml") == 0)
            {
                exists = 1;
                flag = 1;
                break;
            }
        }
        closedir(dir);
        if(flag)
            break;

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
        {
            perror("an error accured!\n");
            return 1;
        }

        if (strcmp(tmp_cwd, "/mnt") != 0)
        {
            if (chdir("..") != 0) 
            {
                perror("an error accured while trying to accsess your files!\n");        
                return 1;
            }
        }

    } while (strcmp(tmp_cwd, "/mnt") != 0);

    if (chdir(cwd) != 0)
    {
        perror("an error accured!\n");
        return 1;
    }

    if (!exists) 
    {
        if (mkdir(".mml", 0755) != 0) 
        {
            perror("unable to initialize the repository!\n");
            return 1;
        }

        return create_configs(username, email);
        return 0;
    } 
    else 
    {
        perror("mml repository has already been initialized!\n");
    }
    return 0;
}



int main(int argc, char * argv[])
{
    if (argc < 2) 
    {
        fprintf(stdout, "please enter a valid command\n");
        return 1;
    }

    char email[1000];
    char username[1000];

    int globalUsernameExistance = 0;
    int globalEmailExistance = 0;
    if(strcmp(argv[1], "config-global") == 0)
    {
        if(strcmp(argv[2], "user.name") == 0)
        {
            strcpy(username, argv[3]);
            globalUsernameChanger(username);
        }
        if(strcmp(argv[2], "user.email") == 0)
        {
            strcpy(email, argv[3]);
            globalEmailChanger(email);            
        }

    }
    if(strcmp(argv[1], "init") == 0)
    {
        globalEmailFinder(email);
        globalUsernameFinder(username);
        init(argc, argv,username , email);
    }

}