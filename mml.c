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
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlConfigUserEmail") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        if(mkdir(".mmlConfigUserEmail", 0755) != 0)
        {
            perror("An error accurred!\n");
            return 1;
        }
    }

    closedir(dir);
    
    if(chdir(".mmlConfigUserEmail") != 0)
    {
        printf("an error accured!\n");
        return 1;
    }

    FILE * file = fopen("globalEmail.txt", "w");
    fprintf(file, "Email : %s\n", globalEmail);
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
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlConfigUserEmail") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        if(mkdir(".mmlConfigUserEmail", 0755) != 0)
        {
            perror("An error accurred!\n");
            return 1;
        }
    }

    closedir(dir);
    
    if(chdir(".mmlConfigUserEmail") != 0)
    {
        perror("an error accured!\n");
        return 1;
    }

    FILE * file = fopen("globalUsername.txt", "w");
    fprintf(file, "Username : %s\n", globalUsername);
    fclose(file);
    
    chdir(cwd);

    return 0;
}

int globalSymbolicLink()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char home[1024];
    strcpy(home, getenv("HOME"));

    char command[3000];
    sprintf(command, "ln -s %s/.mmlConfigUserEmail %s/.mml/.mmlConfigUserEmail", home, cwd);


    system(command);
}

int create_configs(char *username, char *email)
{
    globalSymbolicLink();

    FILE *file = fopen(".mml/configLastGlobalUsername", "w");
    FILE *file2 = fopen(".mml/.mmlConfigUserEmail/globalUsername.txt", "r");
    char global[2000];
    fgets(global, sizeof(global), file2);
    if (file == NULL || file2 == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    int len = strlen(global);
    if(len > 0 && global[len - 1] == '\n')
        global[len - 1] = '\0';
    fprintf(file, "%s", global);
    fclose(file);
    fclose(file2);

    file = fopen(".mml/configLastGlobalEmail", "w");
    file2 = fopen(".mml/.mmlConfigUserEmail/globalEmail.txt", "r");
    fgets(global, sizeof(global), file2);
    if (file == NULL || file2 == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    
    len = strlen(global);
    if(len > 0 && global[len - 1] == '\n')
        global[len - 1] = '\0';
    fprintf(file, "%s", global);
    fclose(file);
    fclose(file2);

    file = fopen(".mml/configLastCommitID", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    fprintf(file, "last_commit_ID: %d\n", 0);
    fclose(file);
    
    file = fopen(".mml/configCurrntCommitId", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    fprintf(file, "current_commit_ID: %d\n", 0);
    fclose(file);

    file = fopen(".mml/configBranch", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    fprintf(file, "branch: %s", "master");
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

    if (mkdir(".mml/staging", 0755) != 0) 
    {    
        perror("unable to create the repository!\n");
        return 1;
    }
    
    file = fopen(".mml/tracks", "w");
    fclose(file);

    file = fopen(".mml/finalUsername", "w");
    file2 = fopen(".mml/.mmlConfigUserEmail/globalUsername.txt", "r");
    fgets(global, sizeof(global), file2);
    if (file == NULL || file2 == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    len = strlen(global);
    if(len > 0 && global[len - 1] == '\n')
        global[len - 1] = '\0';
    fprintf(file, "%s", global);
    fclose(file);
    fclose(file2);

    file = fopen(".mml/finalEmail", "w");
    file2 = fopen(".mml/.mmlConfigUserEmail/globalEmail.txt", "r");
    fgets(global, sizeof(global), file2);
    if (file == NULL || file2 == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    len = strlen(global);
    if(len > 0 && global[len - 1] == '\n')
        global[len - 1] = '\0';
    fprintf(file, "%s", global);
    fclose(file);
    fclose(file2);
    
    return 0;
}

int localUsernameChanger(char* username)
{
    FILE *file = fopen(".mml/finalUsername", "w");
    if(file == NULL)
    {
        perror("you must be in the main directory of your repository to change your projects username\n");
        return 1;
    }
    fprintf(file, "Username : %s\n", username);
    fclose(file);

    return 0;
}

int localEmailChanger(char* email)
{
    FILE *file = fopen(".mml/finalEmail", "w");
    if(file == NULL)
    {
        perror("you must be in the main directory of your repository to change your projects email\n");
        return 1;

    }
        
    fprintf(file, "Email : %s\n", email);
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

        if (strcmp(tmp_cwd, "/") != 0)// shal be changed
        {
            if (chdir("..") != 0) 
            {
                perror("an error accured while trying to access your files!\n");        
                return 1;
            }
        }

    } while (strcmp(tmp_cwd, "/") != 0);// shall be changed

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

int globalAlias(char command[], char aliasName[])
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
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlAlias") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        if(mkdir(".mmlAlias", 0755) != 0)
        {
            perror("An error accurred!\n");
            return 1;
        }
    }

    closedir(dir);
    
    int flag2 = 1;
    if(chdir(".mmlAlias") != 0)
    {
        printf("an error accured!\n");
        return 1;
    }

    dir = opendir(".");
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, "AliasGlobal") == 0)
        {
            flag2 = 0;
            break;
        }
    }

    if(flag2)
    {
        FILE * file = fopen("AliasGlobal", "w");
        fclose(file);

    }

    FILE * file = fopen("AliasGlobal", "a");
    fprintf(file, "mml %s==%s\n", aliasName, command);
    fclose(file);
    
    chdir(cwd);
    return 0;

}

int localAlias(char command[], char aliasName[])
{
    if(chdir("./.mml") != 0)
    {
        perror("you should be in the main directory of your <<initiallized>> repository\n");
        return 1;
    }

    DIR *dir = opendir(".");
    struct dirent* entry;
    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, "AliasLocal") == 0)
        {
            flag = 0;
            break;
        }
    }
    closedir(dir);

    if(flag)
    {
        FILE * file = fopen("AliasLocal", "w");
        fclose(file);
    }

    FILE * file = fopen("AliasLocal", "a");
    fprintf(file, "mml %s==%s\n", aliasName, command);
    fclose(file);
    chdir("..");
    return 0;

}

typedef struct jaygozin
{
    char replace[1000];
    char initial[1000];

}alias;

int getTheNewestGlobalAliases(alias aliases[] , int* size)
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
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".mmlAlias") == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        chdir(cwd);
        return 1;
    }
    closedir(dir);

    chdir("./.mmlAlias");

    FILE * file = fopen("AliasGlobal", "r");
    char line[1000];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char* tok = strtok(line, "=\n");
        strcpy(aliases[*size].replace, tok);
        tok = strtok(NULL, "=\n");
        strcpy(aliases[*size].initial, tok);

        *size += 1;
    }
    fclose(file);

    chdir(cwd);
    return 0;
}

int getTheNewestLocalAliases(alias aliases[], int* size)
{
    if(chdir("./.mml") != 0)
    {
        return 1;
    }

    DIR *dir = opendir(".");
    struct dirent* entry;
    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, "AliasLocal") == 0)
        {
            flag = 0;
            break;
        }
    }
    closedir(dir);

    if(flag)
    {
        chdir("..");
        return 1;
    }

    FILE * file = fopen("AliasLocal", "r");
    char line[1000];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char* tok = strtok(line, "=\n");
        strcpy(aliases[*size].replace, tok);
        tok = strtok(NULL, "=\n");
        strcpy(aliases[*size].initial, tok);

        *size += 1;
    }

    fclose(file);
    chdir("..");
    return 0;
}

int go_to_mmlrepo()
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

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0) 
            {
                perror("an error accured while trying to accsess your files!\n");        
                return 1;
            }
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    if(exists == 0)
    {
        if (chdir(cwd) != 0)
        {
            perror("an error accured!\n");
            return 1;
        }
        perror("mml repository has not yet been initialized!\n");
        return 1;
    }

    if(chdir("./.mml") != 0)
    {
        return 1;
    }
    
    return 0;
    
}

int add(char path[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    struct stat path_stat;
    stat(path, &path_stat);
    int isDir = 0, isFile = 0, other = 0;
    
    if(S_ISREG(path_stat.st_mode))
    {
            isFile ++;
    }
    else if(S_ISDIR(path_stat.st_mode))
    {
        isDir ++;
    }
    else
    {
        other ++;
    }
    
    return go_to_mmlrepo();

    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        perror("Error opening .mml directory!\n");
        return 1;
    }

    struct dirent* entry;
    int exists = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "staging") == 0)
        {
            exists = 1;
            break;
        }
    }
    closedir(dir);
        
    if (!exists) 
    {
        if (mkdir("staging", 0755) != 0) 
        {
            perror("an error accured while trying to stage your files!\n");
            return 1;
        }
    } 

    if(chdir("./staging") != 0)
    {
        perror("an error accurred while trying to access your staging files !\n");
        return 1;
    }
    
    dir = opendir(".");
    if (dir == NULL)
    {
        perror("Error opening staging directory!\n");
        return 1;
    }

    entry;
    int stagingFileExists = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, "stagingDoc") == 0)
        {
            stagingFileExists = 1;
        }
    }
    closedir(dir);

    if (stagingFileExists == 0)
    {
        FILE* file = fopen("stagingDoc", "w");
        fclose(file);
    }

}

int main(int argc, char * argv[])
{
    if (argc < 2) 
    {
        perror("please enter a valid command\n");
        return 1;
    }
    
    int size = 0;
    alias aliases[500];
    
    getTheNewestGlobalAliases(aliases, &size);
    getTheNewestLocalAliases(aliases, &size);

    for(int i = 0 ; i < size; i++)
    {
        char commandrepalce[1000] = "mml ";
        strcat(commandrepalce, argv[1]);
        if(strcmp(commandrepalce, aliases[i].replace) == 0)
        {
            system(aliases[i].initial);
            return 0;
        }
    }
    char email[1000];
    char username[1000];

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
        if(strncmp(argv[2], "alias", 5) == 0)
        {
            char aliasName[1000];
            sscanf(argv[2], "alias.%s", aliasName);
            
            if(strncmp(argv[3], "mml", 3) != 0)
            {
                perror("Invalid command\n");
                return 1;
            }

            if(globalAlias(argv[3], aliasName))
                return 1;
        }

    }
    if(strcmp(argv[1], "config") == 0)
    {
        if(strcmp(argv[2], "user.name") == 0)
        {
            strcpy(username, argv[3]);
            if(localUsernameChanger(username))
                return 1;
        }
        if(strcmp(argv[2], "user.email") == 0)
        {
            strcpy(email, argv[3]);
            if(localEmailChanger(email))
                return 1;            
        }
        if(strncmp(argv[2], "alias", 5) == 0)
        {
            char aliasName[1000];
            sscanf(argv[2], "alias.%s", aliasName);

            if(strncmp(argv[3], "mml", 3) != 0)
            {
                perror("Invalid command\n");
                return 1;
            }

            if(localAlias(argv[3], aliasName))
                return 1;
        }

    }
    if(strcmp(argv[1], "init") == 0)
    {
        if(init(argc, argv,username , email))
            return 1;
    }

    if(strcmp(argv[1], "add") == 0)
    {
        char path[2000];
        if(getcwd(path, sizeof(path)) == NULL)
            return 1;

        int len = strlen(path);
        path[len] = '/';
        path[len + 1] = '\0';

        strcat(path, argv[2]);

        add(path);
        
    }

}