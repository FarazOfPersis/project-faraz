// Faraz Doagooye Tehrani 402105998
// mml stands for: my miserable life 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <wchar.h>

#define red "\x1b[31m"
#define yellow "\x1b[33m"
#define cyan "\x1b[36m"
#define green "\x1b[32m"
#define resetcolor "\x1b[0m"

#ifndef DT_DIR
#define DT_DIR 4
#endif


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
    fprintf(file, "%s", globalEmail);
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
    fprintf(file, "%s", globalUsername);
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

    file = fopen(".mml/currentBranch", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    fprintf(file, "%s", "master");
    fclose(file);

    file = fopen(".mml/branchList", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    fprintf(file, "%s\n", "master");
    fclose(file);

    file = fopen(".mml/head", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    fprintf(file, "%s", "HEAD");
    fclose(file);

    if (mkdir(".mml/commits", 0755) != 0) 
    {    
        perror("unable to create the repository!\n");
        return 1;
    }

    if (mkdir(".mml/staging", 0755) != 0) 
    {    
        perror("unable to create the repository!\n");
        return 1;
    }
    if (mkdir(".mml/branches", 0755) != 0) 
    {    
        perror("unable to create the repository!\n");
        return 1;
    }
    if (mkdir(".mml/tags", 0755) != 0) 
    {    
        perror("unable to create the repository!\n");
        return 1;
    }

    file = fopen(".mml/branches/master", "w");
    fprintf(file, "%d", 0);
    fclose(file);
    
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
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE *file = fopen("finalUsername", "w");
    if(file == NULL)
    {
        perror("you must be in the main directory of your repository to change your projects username\n");
        return 1;
    }    
    fprintf(file, "%s", username);
    fclose(file);

    chdir(cwd);


    return 0;
}

int localEmailChanger(char* email)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE *file = fopen("finalEmail", "w");
    if(file == NULL)
    {
        perror("you must be in the main directory of your repository to change your projects email\n");
        return 1;

    }
        
    fprintf(file, "%s", email);
    fclose(file);

    chdir(cwd);

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

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0) 
            {
                perror("an error accured while trying to access your files!\n");        
                return 1;
            }
        }

    } while (strcmp(tmp_cwd, "/") != 0);

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
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

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
    chdir(cwd);
    return 0;

}

typedef struct jaygozin
{
    char replace[1000];
    char initial[1000];

}alias;

typedef struct mianbor
{
    char oldMessage[1000];
    char newMessage[1000];

}shortcut;

typedef struct vaziat
{
    char path[2000];
    char name[200];
    char staged;
    char state;

}fileState;

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
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

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
        chdir(cwd);
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
    chdir(cwd);
    return 0;
}

int ownersEmailFinder(char email[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* lastGlobal = fopen("configLastGlobalEmail", "r");
    FILE* final = fopen("finalEmail", "r");
    FILE* global = fopen("./.mmlConfigUserEmail/globalEmail.txt", "r");
    
    char lastglobal[2000], finall[2000], globall[2000];

    fgets(lastglobal, sizeof(lastglobal), lastGlobal);
    int len = strlen(lastglobal);
    if(lastglobal[len - 1] == '\n' && len > 0)
        lastglobal[len - 1] = '\0';

    fgets(finall, sizeof(finall), final);
    len = strlen(finall);
    if(finall[len - 1] == '\n' && len > 0)
        finall[len - 1] = '\0';

    fgets(globall, sizeof(globall), global);
    len = strlen(globall);
    if(globall[len - 1] == '\n' && len > 0)
        globall[len - 1] = '\0';

    fclose(lastGlobal);
    fclose(final);
    fclose(global);


    if(strcmp(finall, lastglobal) == 0)
    {
        strcpy(email, globall);
        FILE* lastGlobal = fopen("configLastGlobalEmail", "w");
        FILE* final = fopen("finalEmail", "w");

        fprintf(lastGlobal, "%s", globall);
        fprintf(final, "%s", globall);

        fclose(lastGlobal);
        fclose(final);
    }

    else
    {
        if(strcmp(lastglobal, globall) == 0)
        {
            strcpy(email, finall);
        }

        else
        {
            strcpy(email, globall);
            FILE* lastGlobal = fopen("configLastGlobalEmail", "w");
            FILE* final = fopen("finalEmail", "w");

            fprintf(lastGlobal, "%s", globall);
            fprintf(final, "%s", globall);

            fclose(lastGlobal);
            fclose(final);
        }


    }


    chdir(cwd);
    return 0;

}

int ownersUsernameFinder(char username[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* lastGlobal = fopen("configLastGlobalUsername", "r");
    FILE* final = fopen("finalUsername", "r");
    FILE* global = fopen("./.mmlConfigUserEmail/globalUsername.txt", "r");
    
    char lastglobal[2000], finall[2000], globall[2000];

    fgets(lastglobal, sizeof(lastglobal), lastGlobal);
    int len = strlen(lastglobal);
    if(lastglobal[len - 1] == '\n' && len > 0)
        lastglobal[len - 1] = '\0';

    fgets(finall, sizeof(finall), final);
    len = strlen(finall);
    if(finall[len - 1] == '\n' && len > 0)
        finall[len - 1] = '\0';

    fgets(globall, sizeof(globall), global);
    len = strlen(globall);
    if(globall[len - 1] == '\n' && len > 0)
        globall[len - 1] = '\0';

    fclose(lastGlobal);
    fclose(final);
    fclose(global);


    if(strcmp(finall, lastglobal) == 0)
    {
        strcpy(username, globall);
        FILE* lastGlobal = fopen("configLastGlobalUsername", "w");
        FILE* final = fopen("finalUsername", "w");

        fprintf(lastGlobal, "%s", globall);
        fprintf(final, "%s", globall);

        fclose(lastGlobal);
        fclose(final);
    }

    else
    {
        if(strcmp(lastglobal, globall) == 0)
        {
            struct stat glob;
            struct stat loc;
            if(stat("finalUsername", &loc) == -1)
            {
                perror("an error accured!\n");
                return 1;
            }
            if(stat("./.mmlConfigUserEmail/globalUsername.txt", &glob) == -1)
            {
                perror("an error accured!\n");
                return 1;
            }
            if(glob.st_mtime < loc.st_mtime)
                strcpy(username, finall);

            else        
                strcpy(username, globall);
        }

        else
        {
            strcpy(username, globall);
            FILE* lastGlobal = fopen("configLastGlobalUsername", "w");
            FILE* final = fopen("finalUsername", "w");

            fprintf(lastGlobal, "%s", globall);
            fprintf(final, "%s", globall);

            fclose(lastGlobal);
            fclose(final);
        }


    }


    chdir(cwd);
    return 0;

}

int add(char path[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(access(path, F_OK)== -1)
    {
        perror("Invalid path!\n");
        return 1;
    }

    
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

    if(isDir)
    {
        if(chdir(path) != 0)
            return 1;

        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening you directory!\n");
            return 1;
        }

        struct dirent* entry;
        int flag = 1;
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 &&  strcmp(entry->d_name, "..") != 0)
                flag = 0;
        
        }           
        closedir(dir);

        if(!flag)
        {

            dir = opendir(".");
            while ((entry = readdir(dir)) != NULL) 
            {  
                if(strcmp(entry->d_name, ".") != 0 &&  strcmp(entry->d_name, "..") != 0) 
                {   
                    char newPath[2000];
                    sprintf(newPath, "%s/%s", path, entry->d_name);
                    add(newPath);
                }

            }
            closedir(dir);  
        }  
        chdir(cwd);
        
    }

    if(go_to_mmlrepo())
        return 1;

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
    FILE* file = fopen("stagingDoc", "r");
    char savedPath[2000];
    while(fgets(savedPath, 2000, file) != NULL)
    {
        int len = strlen(savedPath);
        if(savedPath[len - 1] == '\n' && len > 0)
        {
            savedPath[len - 1] = '\0';
        } 

        if (strcmp(savedPath, path) == 0)
        {
            perror("you can't add a file which is already in the staging area unless you commit the changes or reset your file!\n");
            return 1;
        }
    }
    fclose(file);

    file = fopen("stagingDoc", "a");

    fprintf(file, "%s\n", path);
    if(isDir)
    {
        chdir(cwd);
        return 0;
    }
    
    char command[4000], currentPlace[3000];
    getcwd(currentPlace, sizeof(currentPlace));
    sprintf(command, "cp -f %s %s", path, currentPlace);
    system(command);

    if(chdir("..") != 0)
        return 1;

    chdir(cwd);
    return 0;

}

int showCurrentStagings(int depth, int finaldepth)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

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

    if(!exists)
    {
        perror("you haven't staged any changes yet!\n");
        return 1;
    }

    if(chdir("./staging") != 0)
    {
        perror("an error accurred while trying to access your staging files !\n");
        return 1;
    }
    
    dir = opendir(".");
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
        perror("you haven't staged any changes yet!\n");
        return 1;
    }

    FILE* file = fopen("stagingDoc", "r");
    char staged[500][1000];
    int size = 0;
    while(fgets(staged[size], sizeof(staged[size]), file) != NULL)
    {
        int len = strlen(staged[size]);
        if(staged[size][len - 1] == '\n')
        {
            staged[size][len - 1] = '\0';
        }
        size++;
        
    }

    chdir(cwd);

    dir = opendir(".");
    while ((entry = readdir(dir)) != NULL) 
    {
        if(strcmp(entry->d_name, ".") != 0 &&  strcmp(entry->d_name, "..") != 0) 
        { 
            if(entry->d_type == DT_DIR) 
            {
                int par = 1;
                if(finaldepth - depth == 0)
                {
                    par = 0;
                }
                if(par)
                {                        
                    chdir(entry->d_name);
                    for(int i = 0 ; i < depth - 1; i++)
                    {
                        printf("\t\t");
                    }
                    if(depth > 0)
                    {
                        for(int i = 0 ; i < 3; i++)
                            printf("----");
                    }
                    if(entry->d_type == DT_DIR)
                    {
                        printf(green "%s\n" resetcolor, entry->d_name);
                    }

                    showCurrentStagings(depth + 1, finaldepth);
                    chdir(cwd);

                }
                chdir(cwd);
                continue;
            } 
            char newPath[2000];
            sprintf(newPath, "%s/%s", cwd, entry->d_name);
            int flag = 0;
            for(int i = 0 ; i < size; i++)
            {
                if(strcmp(newPath, staged[i]) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            for(int i = 0 ; i < depth - 1; i++)
            {
                printf("\t\t");
            }
            if(depth > 0)
            {
                for(int i = 0 ; i < 3; i++)
                    printf("----");
            }
            if(entry->d_type == DT_DIR)
            {
                printf(green "%s\n" resetcolor, entry->d_name);
            }
            else if(flag)
            {
                printf(cyan "%s : staged\n" resetcolor, entry->d_name);
            }
            else
            {
                printf(red "%s : not staged\n" resetcolor, entry->d_name);

            }
        }
        
    }
    closedir(dir);
    return 0;

}

int nameFinder(char name[], char path[])
{
    char cpy[3000];
    strcpy(cpy, path);

    char * tok = strtok(cpy, "/\n");
    while(tok != NULL)
    {
        strcpy(name, tok);
        tok = strtok(NULL, "/\n");
    }

    return 0;

}

int reset(char path[])
{
    if(access(path, F_OK)== -1)
    {
        perror("Invalid path!\n");
        return 1;
    }
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char name[2000];
    char cpy[3000];
    strcpy(cpy, path);

    char * tok = strtok(cpy, "/\n");
    while(tok != NULL)
    {
        strcpy(name, tok);
        tok = strtok(NULL, "/\n");
    }
    
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

    if(isDir)
    {
        if(chdir(path) != 0)
            return 1;

        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening you directory!\n");
            return 1;
        }

        struct dirent* entry;
        int flag = 1;
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 &&  strcmp(entry->d_name, "..") != 0)
                flag = 0;
        
        }         
        closedir(dir);

        if(!flag)
        {
            dir = opendir(".");
            while ((entry = readdir(dir)) != NULL) 
            {  
                if(strcmp(entry->d_name, ".") != 0 &&  strcmp(entry->d_name, "..") != 0) 
                {   
                    char newPath[2000];
                    sprintf(newPath, "%s/%s", path, entry->d_name);
                    reset(newPath);
                }
            }

            closedir(dir);    
        }
        chdir(cwd);
    }


        if(go_to_mmlrepo())
            return 1;

        if(chdir("./staging") != 0)
        {
            perror("an error accurred while trying to access your staging files !\n");
            return 1;
        }

        remove(name);
        FILE* stagingfile = fopen("stagingDoc", "r");
        if(stagingfile == NULL)
        {
            perror("you haven't staged any changes yet!\n");
            chdir(cwd);
            return 1;
        }
        
        char line[2000];

        int exists = 1;
        int counter = 0;
        while(fgets(line, sizeof(line), stagingfile) != NULL)
        {
            int len = strlen(line);
            if(line[len - 1] == '\n' && len > 0)
                line[len - 1] = '\0';

            counter ++;
            
            if(strcmp(line, path) == 0)
            {           
                counter --;
                exists = 0;
                FILE *stagingfile2 = fopen("stagingDoc", "r");
                FILE *tmp = fopen("tmp", "w");
                char line2[2000];
                while(fgets(line2, sizeof(line2), stagingfile2) != NULL)
                {
                    int len2 = strlen(line2);
                    if(line2[len2 - 1] == '\n' && len2 > 0)
                    line2[len2 - 1] = '\0';
                    if(strcmp(line2, path) != 0)
                    {
                        fprintf(tmp, "%s\n", line2);
                    }

                }
                fclose(stagingfile2);
                fclose(tmp);
                remove("stagingDoc");
                rename("tmp", "stagingDoc");
            }
        }

        if(exists)
        {
            /*perror("your file or directory has not been staged yet!\n");
            chdir(cwd);*/
            return 1;
        }
        if(counter < 1)
        {
            remove("stagingDoc");
        }


        chdir(cwd);
        return 0;
    
}

int commit(char *message, char email[], char username[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* head = fopen("head", "r");
    char headcur[2000];
    fgets(headcur, sizeof(headcur), head);
    int length = strlen(headcur);
    if(headcur[length - 1] == '\n')
        headcur[length - 1] = '\0';
    fclose(head);
    if(strcmp(headcur, "HEAD") != 0)
    {
        perror("you can only commit from the HEAD!\n");
        return 1;
    }

    if(access("staging/stagingDoc", F_OK)== -1)
    {
        perror("You haven't staged any changes!\n");
        return 1;
    }

    FILE* lastCommitId = fopen("configLastCommitID", "r");
    int id;
    fscanf(lastCommitId,"last_commit_ID : %d" , &id);
    fclose(lastCommitId);

    lastCommitId = fopen("configLastCommitID", "w");
    fprintf(lastCommitId, "last_commit_ID : %d", id + 1);
    fclose(lastCommitId);

    FILE* currentBranch = fopen("currentBranch", "r");
    char branch[1000];
    fgets(branch, sizeof(branch), currentBranch);
    fclose(currentBranch);
    int len = strlen(branch);
    if (branch[len - 1] == '\n' && len > 0)
    {
        branch[len - 1] = '\0';
    }

    id += 1;
    if(chdir("./commits") != 0)
    {
        perror("unable to access your commits directory!\n");
        return 1;
    }

    char ID[20];
    sprintf(ID, "%d", id);
    char branchPath[2000];
    sprintf(branchPath, "../branches/%s", branch);
    if(id != 1)
    {    
        FILE* file = fopen(branchPath, "r");
        char command[2000], prevID[20];
        int prevId;
        fscanf(file, "%d", &prevId);
        fclose(file);
        sprintf(prevID, "%d", prevId);
        sprintf(command, "cp -r %s %s", prevID, ID);
        system(command);
        chdir(ID);
        FILE* prevIDD = fopen("prevId", "w");
        fprintf(prevIDD, "%d", prevId);
        fclose(prevIDD);
        chdir("..");
    }
    else
    {

        if (mkdir(ID, 0755) != 0) 
        {    
            perror("unable to create the commit directory!\n");
            return 1;
        }
    }
    
    FILE* new = fopen(branchPath, "w");
    fprintf(new, "%d", id);
    fclose(new);
    if(chdir(ID) != 0)
        return 1;
    
    FILE* owner = fopen("owner", "w");
    fprintf(owner, "%s\n%s", username, email);
    fclose(owner);

    FILE* branchName= fopen("branchName", "w");
    fprintf(branchName, "%s", branch);
    fclose(branchName);

    FILE* timeAndDate = fopen("timeAndDate", "w");
    FILE* timeBin = fopen("timeAndDate.bin", "wb");

    time_t currentTime;
    time(&currentTime);

    fwrite(&currentTime, sizeof(time_t), 1, timeBin);
    fclose(timeBin);

    struct tm *localTime = localtime(&currentTime);
    fprintf(timeAndDate, "%s\n", asctime(localTime));
    fclose(timeAndDate);

    FILE* messagee = fopen("message", "w");
    fprintf(messagee, "%s", message);
    fclose(messagee);

    if(chdir("..") != 0)
        return 1;

    if(chdir("..") != 0)
        return 1;
    
    
    FILE* stagingDoc =fopen("staging/stagingDoc", "r");
    if(stagingDoc == NULL)
    {
        perror("you haven't staged any changes yet!\n");
        return 1;
    }
    char path[2000];
    int flag = 1;
    while(fgets(path, sizeof(path), stagingDoc) != NULL)
    {    
        int len = strlen(path);
        if(path[len - 1] == '\n' && len > 0)
            path[len - 1] = '\0';

        struct stat path_stat;
        stat(path, &path_stat);
        int isFile = 0;
        if(S_ISREG(path_stat.st_mode))
        {
            isFile ++;
        }

        if(isFile == 0)
        {
            continue;
        }

        FILE * tracks2 = fopen("tracks", "r");
        char line[2000];
        int flag = 1;
        while(fgets(line, sizeof(line), tracks2) != NULL)
        {
            len = strlen(line);
            if(len > 0 && line[len - 1] == '\n')
            {
                line[len - 1] = '\0';
            }

            if(strcmp(line, path) == 0)
            {
                flag = 0;
                break;
            }
        }
        fclose(tracks2);
    
        if(flag)
        {
            FILE* tracks = fopen("tracks", "a");
            fprintf(tracks, "%s\n", path);
            fclose(tracks);
        }
    } 
    fclose(stagingDoc);

    if(chdir("staging") != 0)
        return 1;
    

    int num = 0;
    DIR* dir = opendir(".");
    struct dirent* entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name ,".." ) != 0 && strcmp(entry->d_name, "stagingDoc") != 0)
        {
            num ++;
            char command[2000];
            sprintf(command, "mv -f %s ../commits/%s/%s", entry->d_name, ID, entry->d_name);
            system(command);
        }
        
    }
    remove("stagingDoc");
    

    if(chdir("..") != 0)
        return 1;

    FILE* tracks = fopen("tracks" ,"r");
    path[2000];
    while(fgets(path, sizeof(path), tracks) != NULL)
    {
        int len = strlen(path);
        if(len > 0 && path[len - 1] == '\n')
        {
            path[len - 1] = '\0';
        }
        char name[2000];
        nameFinder(name, path);
        char newpath[3000];
        sprintf(newpath, "commits/%s/%s",ID, name);
            
        if(access(path, F_OK) == -1)
        {
            remove(newpath);
            FILE* tracks2 = fopen("tracks", "r");
            FILE* tracksfinal = fopen("tmp", "w");

            char line2[2000];
            while(fgets(line2, sizeof(line2), tracks2) != NULL)
            {
                int len = strlen(line2);
                if(len > 0 && line2[len - 1] == '\n')
                {
                    line2[len - 1] = '\0';
                }

                if(strcmp(line2, path) != 0)
                {
                    int len2 = strlen(line2);
                    if(line2[len2 - 1] == '\n' && len2 > 0)
                        line2[len2 - 1] = '\0';
                    fprintf(tracksfinal, "%s\n", line2);
                }
            }
            fclose(tracks2);
            fclose(tracksfinal);
            remove("tracks");
            rename("tmp", "tracks");
    
        }
    }
    fclose(tracks);

    char newcommand[2000];
    sprintf(newcommand, "cp -f %s ./commits/%s/%s", "tracks", ID, "tracks");
    system(newcommand);

    chdir("commits");
    chdir(ID);
    FILE* number = fopen("numberOfCommitedFiles", "w");
    fprintf(number, "%d", num);
    fclose(number);

    printf("ID : %d\nMessage : %s\nDate And Time of commit : %s", id, message, asctime(localTime));

    chdir(cwd);
    return 0;

}

int addShortcut(char oldMessage[], char newMessage[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    DIR *dir = opendir(".");
    struct dirent* entry;
    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, "shortcutMessage") == 0)
        {
            flag = 0;
            break;
        }
    }
    closedir(dir);

    if(flag)
    {
        FILE * file = fopen("shortcutMessage", "w");
        fclose(file);
    }

    FILE * file = fopen("shortcutMessage", "a");
    fprintf(file, "%s==%s\n", newMessage, oldMessage);
    fclose(file);
    chdir(cwd);
    return 0;

}

int removeShortcut(char newMessage[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    DIR *dir = opendir(".");
    struct dirent* entry;
    int flag = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, "shortcutMessage") == 0)
        {
            flag = 0;
            break;
        }
    }
    closedir(dir);

    if(flag)
    {
        perror("no shortcut message exists!\n");
        return 1;
    }
    
    FILE* file = fopen("shortcutMessage", "r");
        
    char line[2000];

    int exists = 1;
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char linecpy[2000];
        strcpy(linecpy , line);
        char *tok = strtok(linecpy, "=\n");
            
        if(strcmp(tok, newMessage) == 0)
        {           

            exists = 0;
            FILE *file2 = fopen("shortcutMessage", "r");
            FILE *tmp = fopen("tmp", "w");
            char line2[2000];
            while(fgets(line2, sizeof(line2), file2) != NULL)
            {
                if(strcmp(line2, line) != 0)
                {
                    int len2 = strlen(line2);
                    if(line2[len2 - 1] == '\n' && len2 > 0)
                        line2[len2 - 1] = '\0';
                    fprintf(tmp, "%s\n", line2);
                }
                fclose(file2);
                fclose(tmp);
                remove("shortcutMessage");
                rename("tmp", "shortcutMessage");

            }

            break;
        }
    }
    if(exists)
    {
        perror("The given shortcut doesn't exist!\n");
        return 1;
    }

    chdir(cwd);
    return 0;
}

int getTheNewestShortcutMessage(shortcut shortcuts[], int* size)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;
    
    FILE* file = fopen("shortcutMessage", "r");

    if(file == NULL)
    {
        chdir(cwd);
        return 1;
    }

    char line[3000];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char *tok = strtok(line, "=\n");
        strcpy(shortcuts[*size].newMessage, tok);

        tok = strtok(NULL, "=\n");
        strcpy(shortcuts[*size].oldMessage, tok);

        *size += 1;
    }
    fclose(file);
    chdir(cwd);
    return 0;
}

int status()
{    
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    fileState files[500];

    FILE *file = fopen("currentBranch", "r");
    char branch[2000];
    fgets(branch, sizeof(branch), file);
    fclose(file);
    int len = strlen(branch);
    if(branch[len - 1] == '\n' && len > 0)
    {
        branch[len - 1] = '\0';
    }
    chdir("branches");

    file = fopen(branch, "r");
    int id;
    fscanf(file, "%d", &id);
    fclose(file);
    chdir("..");
    
    char ID[20];
    sprintf(ID, "%d", id);
    int size = 0;   

    file = fopen("staging/stagingDoc", "r");
    if(file != NULL)
    {
        char line[2000];
        while(fgets(line, sizeof(line), file) != NULL)
        {
            int len = strlen(line);
            if(line[len - 1] == '\n' && len > 0)
            {
                line[len - 1] = '\0';
            }
            struct stat path_stat;
            stat(line, &path_stat);
            int isFile = 0;
    
            if(S_ISREG(path_stat.st_mode))
            {
                isFile ++;
            }
            if(isFile == 0)
            {
                continue;
            }

            strcpy(files[size].path, line);
            nameFinder(files[size].name, line);
            if(id != 0)
            {
            
                char filename[1000];
                sprintf(filename,"commits/%s/tracks", ID);
                FILE* lasttrack = fopen(filename, "r");
                char khat[1000];
                int exists = 1;
                while(fgets(khat, sizeof(khat), lasttrack) != NULL)
                {
                    int len = strlen(khat);
                    if(khat[len - 1] == '\n' && len > 0)
                    {
                        khat[len - 1] = '\0';
                    }

                    if(strcmp(khat, line) == 0)
                    {
                        exists = 0;
                        break;
                    }
                }
                fclose(lasttrack);
                if(exists)
                {
                    files[size].state = 'A';
                }
                else
                    files[size].state = '.';

            }
            size ++;

        }
        
        fclose(file);
    }
    if(id != 0)
    {
        char filename2[1000];
        sprintf(filename2,"commits/%s/tracks", ID);
        FILE* lasttrack = fopen(filename2, "r");
        char line2[2000];
        while((fgets(line2, sizeof(line2), lasttrack)) != NULL)
        {
            int len2 = strlen(line2);
            if(line2[len2 - 1] == '\n' && len2 > 0)
            {
                line2[len2 - 1] = '\0';
            }
            int flag = 1;
            for(int i = 0 ; i < size; i++)
            {
                if(strcmp(line2, files[i].path) == 0)
                {      
                    flag = 0;

                    if(access(line2, F_OK)== -1)
                    {   
                        files[i].state = 'D';
                        break;
                    }
                }
            }
            if(flag)
            {
                if(access(line2, F_OK)== -1)
                {   
                    files[size].state = 'D';
                }
                else
                {
                    files[size].state = '.';

                }
                strcpy(files[size].path, line2);
                nameFinder(files[size].name, line2);
                size ++;
            }
        }
    }

    if(id == 0)
    {
        FILE* file3 = fopen("staging/stagingDoc", "r");
        if(file3 == NULL)
        {
            perror("you haven't staged any files yet!\n");
            return 1;
        }
        fclose(file3);
        for(int i = 0 ; i < size; i++)
        {
            files[i].state = 'A';
            files[i].staged = '+';
        }
        for(int i = 0 ; i < size ; i++)
        {
            if(files[i].state == 'A')
            {                  
                printf("%s ", files[i].name);
                printf("%c", files[i].staged);
                printf(cyan "A\n" resetcolor);

            }
        }
        for(int i = 0 ; i < size ; i++)
        {
            if(files[i].state == 'D')
            { 
                printf("%s ", files[i].name);      
                printf("%c", files[i].staged);
                printf(red "D\n" resetcolor);
            }
        }
            chdir(cwd);
        
    }
    

    if(chdir("commits") != 0)
        return 1;

    if(chdir(ID) != 0)
        return 1;

    file = fopen("timeAndDate.bin", "rb");

    time_t saved;
    fread(&saved, sizeof(time_t), 1, file);

    fclose(file);
    for(int i = 0 ; i < size; i++)
    {
        if(files[i].state == 'D')
            continue;
        
        struct stat fileinfo;
        if(stat(files[i].path, &fileinfo) == -1)
        {
            perror("an error accured!\n");
            return 1;
        }

        if(fileinfo.st_mtime > saved && files[i].state != 'A')
        {
            files[i].state = 'M';
        }

    }

    if(chdir("..") != 0)
        return 1;

    if(chdir("..") != 0)
        return 1;

    if(chdir("staging") != 0)
        return 1;

    FILE* staging = fopen("stagingDoc", "r");
    if(staging == NULL)
    {
        for(int i = 0 ; i < size; i++)
            files[i].staged = '-';
        
    }

    else
    {
        char linee[2000];
        while(fgets(linee, sizeof(linee), staging) != NULL)
        {
            int len = strlen(linee);
            if(linee[len - 1] == '\n' && len > 0)
            {
                linee[len - 1] = '\0';
            }

            for(int i = 0; i < size; i++)
            {
                if(strcmp(linee, files[i].path) == 0)
                {
                    files[i].staged = '+';
                }
            }
        }
    

        fclose(staging);
    }

    for(int i = 0 ; i < size ; i ++)
        if(files[i].staged != '+')
            files[i].staged = '-';

    for(int i = 0 ; i < size ; i++)
    {
        if(files[i].state == 'M')
        {    
            printf("%s ", files[i].name);
            printf("%c", files[i].staged);
            printf(yellow "M\n" resetcolor);
        }
    }
    for(int i = 0 ; i < size ; i++)
    {
        if(files[i].state == '.')
        {      
            printf("%s ", files[i].name);
            printf("%c", files[i].staged);
            printf(green "#\n" resetcolor);

        }
    }
    for(int i = 0 ; i < size ; i++)
    {
        if(files[i].state == 'A')
        {         
            printf("%s ", files[i].name);
            printf("%c", files[i].staged);
            printf(cyan "A\n" resetcolor);

        }
    }
    for(int i = 0 ; i < size ; i++)
    {
        if(files[i].state == 'D')
        { 
            printf("%s ", files[i].name);      
            printf("%c", files[i].staged);
            printf(red "D\n" resetcolor);
        }
    }

    chdir(cwd);
    return 0;
}

int changeChecker()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    if(chdir("staging") != 0)
    {
        chdir(cwd);
        return 1;
    }

    FILE* file = fopen("stagingDoc", "r");
    if(file == NULL)
    {
        chdir(cwd);
        return 1;
    }

    /*struct stat docinfo;
    if(stat("stagingDoc", &docinfo) == -1)
    {
        perror("an error accured!\n");
        chdir(cwd);
        return 1;
    }*/
    
    char line[2000];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
        {
            line[len - 1] = '\0';
        }

        int deleted = 0;
        if(access(line, F_OK)== -1)
        {
            deleted = 1;
        }
        /*struct stat fileinfo;

        if(deleted == 0)
        {
            if(stat(line, &fileinfo) == -1)
            {
            chdir(cwd);
            return 1;
            }
        }*/

        if(deleted)
        {
            FILE *file2 = fopen("stagingDoc", "r");
            FILE *tmp = fopen("tmp", "w");
            char line2[2000];
            int num = 0;
            while(fgets(line2, sizeof(line2), file2) != NULL)
            {
                int len2 = strlen(line2);
                    if(line2[len2 - 1] == '\n' && len2 > 0)
                        line2[len2 - 1] = '\0';
                
                num ++;
                if(strcmp(line2, line) != 0)
                {               
                    fprintf(tmp, "%s\n", line2); 
                }
            }
            num --;

            fclose(file2);
            fclose(tmp);
            remove("stagingDoc");
            if(num > 0)
                rename("tmp", "stagingDoc");
            if(num == 0)
                remove("tmp");

            if(deleted)
            {
                char name[2000];
                nameFinder(name, line);
                remove(line);
            }
        }

    }
    fclose(file);
    chdir(cwd);
    return 0;
}

int branch(char name[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* commitId = fopen("configLastCommitID", "r");
    int id;
    char ID[20];
    fscanf(commitId, "last_commit_ID : %d", &id);
    sprintf(ID, "%d", id);
    if(id == 0)
    {
        perror("you must commit at least once before creating a branch!\n");
        chdir(cwd);
        return 1;
    }
    char findNew[20];
    
    FILE* head = fopen("head", "r");
    char state[2000];
    fgets(state, sizeof(state), head);
    int ln = strlen(state);
    if(state[ln - 1] == '\n' && ln > 0)
        state[ln - 1] = '\0';
    fclose(head);

    FILE* list = fopen("branchList", "r");
    char line[2000];
    while(fgets(line, sizeof(line), list) != NULL)
    {
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
        {
            line[len - 1] = '\0';
        }
        if (strcmp(line, name) == 0)
        {
            perror("branch already exists!\n");
                chdir(cwd);
            return 1;
        }
    }

    FILE* branchList = fopen("branchList", "a");
    fprintf(branchList, "%s\n", name);
    fclose(branchList);
    if (strcmp(state, "HEAD") != 0)
    {
        int new;
        sscanf(state, "Detached (%d)", &new);
        sprintf(findNew, "%d", new);
    }
    else
    {
        char father[2000];
        FILE* fatherBranch = fopen("currentBranch", "r");
        fgets(father, sizeof(father), fatherBranch);
        int length = strlen(father);
        if(father[length - 1] == '\n' && length > 0)
            father[length - 1] = '\0';
        fclose(fatherBranch);

        chdir("branches");
        FILE* fileFather = fopen(father, "r");
        int findnew;
        fscanf(fileFather, "%d", &findnew);
        fclose(fileFather);
        sprintf(findNew, "%d", findnew);
        chdir("..");
    }
    chdir("commits");

    char command[2000];
    char newId[20];
    sprintf(newId, "%d", id + 1);

    sprintf(command, "cp -r %s %s", findNew, newId);
    system(command);
    
    chdir(newId);

    FILE* branchName =  fopen("branchName", "w");
    fprintf(branchName, "%s", name);
    fclose(branchName);

    chdir("..");
    chdir("../branches");

    FILE* branchlastcommit = fopen(name, "w");
    fprintf(branchlastcommit, "%d", id + 1);
    fclose(branchlastcommit);

    chdir("..");
    FILE* lastId = fopen("configLastCommitID", "w");
    fprintf(lastId, "last_commit_ID : %d", id + 1);
    fclose(lastId);
    chdir(cwd);

}

int showBranches()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE *branchList = fopen("branchList", "r");
    char line[2000];
    while(fgets(line, sizeof(line), branchList) != NULL)
    {
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
        {
            line[len - 1] = '\0';
        }
        printf("%s\n", line);
    }

    chdir(cwd);
}

int mmllog(int n)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* file = fopen("configLastCommitID", "r");
    int num ;
    fscanf(file, "last_commit_ID : %d", &num);
    fclose(file);
    if(n == -1)
        n = num;

    if(n < 1)
    {
        perror("you must enter a positive integer!\n");
        return 1;
    }

    if(num == 0)
    {
        perror("You haven't commited anything yet!\n");
        return 1;
    }
    for(int i = 0; i < n; i++)
    {
        if(go_to_mmlrepo())
        return 1;
        
        int id = num - i;
        char ID[20];
        sprintf(ID, "%d", id);
        chdir("commits");
        chdir(ID);
        
        printf(red "Commit ID : %d\t\t" resetcolor, id);
        
        file = fopen("branchName", "r");
        char branchName[1000];
        fgets(branchName, sizeof(branchName), file);
        int len = strlen(branchName);
        if(branchName[len - 1] == '\n')
        {
            branchName[len - 1] = '\0';
        }
        fclose(file);
        printf(red "Branch : %s\n" resetcolor, branchName);

        file = fopen("owner", "r");
        char username[2000], email[2000];
        fgets(username, sizeof(username), file);
        len = strlen(username);
        if(username[len - 1] == '\n')
            username[len - 1] = '\0';

        fgets(email, sizeof(email), file);
        len = strlen(email);
        if(email[len - 1] == '\n')
            email[len - 1] = '\0';

        fclose(file);

        printf(green "Owners username : %s\tOwners email : %s\n" resetcolor, username, email);
        
        file = fopen("message", "r");
        char message[1000];
        fgets(message, sizeof(message), file);
        len = strlen(message);
        if(message[len - 1] == '\n')
            message[len - 1] = '\0';
        fclose(file);
            
        printf(cyan "Commit message : %s\n" resetcolor, message);

        file = fopen("timeAndDate", "r");
        char time[1000];
        fgets(time, sizeof(time), file);
        len = strlen(time);
        if(time[len - 1] == '\n')
            time[len - 1] = '\0';
        fclose(file);

        printf("Committed on : %s\t\t", time);

        file = fopen("numberOfCommitedFiles", "r");
        char number[1000];
        fgets(number, sizeof(number), file);
        len = strlen(number);
        if(number[len - 1] == '\n')
            number[len - 1] = '\0';
        fclose(file);

        printf("Number of committed files : %s\n", number);

    }

    chdir(cwd);
    return 0;
    
}

int mmllogBranch(char name[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* branches = fopen("branchList", "r");
    char line[2000];
    int flag = 1;
    while(fgets(line, sizeof(line), branches) != NULL)
    {
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
            line[len - 1] = '\0';

        if(strcmp(line, name) == 0)
        {
            flag = 0;
            break;
        }
    }
    fclose(branches);
    if(flag)
    {
        perror("No branch exists with the given name!\n");
        return 1;
    }

    if(chdir("branches") != 0)
        return 1;

    FILE* file = fopen(name, "r");
    int id;
    fscanf(file, "%d", &id);
    fclose(file);

    chdir("../commits");
    while(id > 0)
    {   
        char ID[20];
        sprintf(ID, "%d", id);
        chdir(ID);
        file = fopen("branchName", "r");
        char branchName[2000];
        fgets(branchName, sizeof(branchName), file);
        int len = strlen(branchName);
        fclose(file);
        if(branchName[len - 1] == '\n' && len > 0)
        {
            branchName[len - 1] = '\0';
        }
        if (strcmp(branchName, name) == 0)
        {
            printf(red "Commit ID : %d\t\t" resetcolor, id);

            file = fopen("owner", "r");
            char username[2000], email[2000];
            fgets(username, sizeof(username), file);
            len = strlen(username);
            if(username[len - 1] == '\n')
                username[len - 1] = '\0';

            fgets(email, sizeof(email), file);
            len = strlen(email);
            if(email[len - 1] == '\n')
                email[len - 1] = '\0';

            fclose(file);

            printf(green "Owners username : %s\tOwners email : %s\n" resetcolor, username, email);
        
            file = fopen("message", "r");
            char message[1000];
            fgets(message, sizeof(message), file);
            len = strlen(message);
            if(message[len - 1] == '\n')
                message[len - 1] = '\0';
            fclose(file);
            
            printf(cyan "Commit message : %s\n" resetcolor, message);

            file = fopen("timeAndDate", "r");
            char time[1000];
            fgets(time, sizeof(time), file);
            len = strlen(time);
            if(time[len - 1] == '\n')
                time[len - 1] = '\0';
            fclose(file);

            printf("Committed on : %s\t\t", time);

            file = fopen("numberOfCommitedFiles", "r");
            char number[1000];
            fgets(number, sizeof(number), file);
            len = strlen(number);
            if(number[len - 1] == '\n')
                number[len - 1] = '\0';
            fclose(file);

            printf("Number of committed files : %s\n", number);

        }
        chdir("..");
        id --;
    }

    chdir(cwd);
    return 0;
}

int mmllogAuthor(char name[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* file = fopen("configLastCommitID", "r");
    int id;
    fscanf(file, "last_commit_ID : %d", &id);
    fclose(file);

    chdir("commits");
    while(id > 0)
    {   
        char ID[20];
        sprintf(ID, "%d", id);
        chdir(ID);
        file = fopen("owner", "r");
        char ownersName[2000];
        fgets(ownersName, sizeof(ownersName), file);
        int len = strlen(ownersName);
        fclose(file);

        if(ownersName[len - 1] == '\n' && len > 0)
        {
            ownersName[len - 1] = '\0';
        }
        if (strcmp(ownersName, name) == 0)
        {
            printf(red "Commit ID : %d\t\t" resetcolor, id);

            file = fopen("branchName", "r");
            char branchName[1000];
            fgets(branchName, sizeof(branchName), file);
            int len = strlen(branchName);
            if(branchName[len - 1] == '\n')
            {
                branchName[len - 1] = '\0';
            }
            fclose(file);
            printf(red "Branch : %s\n" resetcolor, branchName);

            file = fopen("owner", "r");
            char username[2000], email[2000];
            fgets(username, sizeof(username), file);
            len = strlen(username);
            if(username[len - 1] == '\n')
                username[len - 1] = '\0';

            fgets(email, sizeof(email), file);
            len = strlen(email);
            if(email[len - 1] == '\n')
                email[len - 1] = '\0';

            fclose(file);

            printf(green "Owners email : %s\n" resetcolor, email);
        
            file = fopen("message", "r");
            char message[1000];
            fgets(message, sizeof(message), file);
            len = strlen(message);
            if(message[len - 1] == '\n')
                message[len - 1] = '\0';
            fclose(file);
            
            printf(cyan "Commit message : %s\n" resetcolor, message);

            file = fopen("timeAndDate", "r");
            char time[1000];
            fgets(time, sizeof(time), file);
            len = strlen(time);
            if(time[len - 1] == '\n')
                time[len - 1] = '\0';
            fclose(file);

            printf("Committed on : %s\t\t", time);

            file = fopen("numberOfCommitedFiles", "r");
            char number[1000];
            fgets(number, sizeof(number), file);
            len = strlen(number);
            if(number[len - 1] == '\n')
                number[len - 1] = '\0';
            fclose(file);

            printf("Number of committed files : %s\n", number);

        }
        chdir("..");
        id --;
    }

    chdir(cwd);
    return 0;

}

int mmllogDate(char date[], int n)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* file = fopen("configLastCommitID", "r");
    int id;
    fscanf(file, "last_commit_ID : %d", &id);
    fclose(file);

    struct tm tmStruct;
    int year, month, hour;
    sscanf(date, "%d %d %d %d:%d:%d", &year, &month, &tmStruct.tm_mday, &hour, &tmStruct.tm_min, &tmStruct.tm_sec);
    tmStruct.tm_year = year - 1900;
    tmStruct.tm_mon = month - 1;
    tmStruct.tm_hour = hour + 1;

    time_t timeval = mktime(&tmStruct);

    chdir("commits");
    while(id > 0)
    {
        char ID[20];
        sprintf(ID, "%d", id);
        chdir(ID);

        file = fopen("timeAndDate.bin", "rb");
        time_t saved;
        fread(&saved, sizeof(time_t), 1, file);
        fclose(file);

        if((saved < timeval) ^ n)
        {
            printf(red "Commit ID : %d\t\t" resetcolor, id);
        
            file = fopen("branchName", "r");
            char branchName[1000];
            fgets(branchName, sizeof(branchName), file);
            int len = strlen(branchName);
            if(branchName[len - 1] == '\n')
            {
                branchName[len - 1] = '\0';
            }
            fclose(file);
            printf(red "Branch : %s\n" resetcolor, branchName);

            file = fopen("owner", "r");
            char username[2000], email[2000];
            fgets(username, sizeof(username), file);
            len = strlen(username);
            if(username[len - 1] == '\n')
                username[len - 1] = '\0';

            fgets(email, sizeof(email), file);
            len = strlen(email);
            if(email[len - 1] == '\n')
                email[len - 1] = '\0';

            fclose(file);

            printf(green "Owners username : %s\tOwners email : %s\n" resetcolor, username, email);
        
            file = fopen("message", "r");
            char message[1000];
            fgets(message, sizeof(message), file);
            len = strlen(message);
            if(message[len - 1] == '\n')
                message[len - 1] = '\0';
            fclose(file);
            
            printf(cyan "Commit message : %s\n" resetcolor, message);

            file = fopen("timeAndDate", "r");
            char time[1000];
            fgets(time, sizeof(time), file);
            len = strlen(time);
            if(time[len - 1] == '\n')
                time[len - 1] = '\0';
            fclose(file);

            printf("Committed on : %s\t\t", time);

            file = fopen("numberOfCommitedFiles", "r");
            char number[1000];
            fgets(number, sizeof(number), file);
            len = strlen(number);
            if(number[len - 1] == '\n')
                number[len - 1] = '\0';
            fclose(file);

            printf("Number of committed files : %s\n", number);
        }

        chdir("..");
        id --;
    }

    chdir(cwd);
    return 0;
}

int mmllogSearch(char** word, int size)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* file = fopen("configLastCommitID", "r");
    int id;
    fscanf(file, "last_commit_ID : %d", &id);
    fclose(file);

    chdir("commits");
    while(id > 0)
    {
        char ID[20];
        sprintf(ID, "%d", id);
        chdir(ID);

        FILE* file = fopen("message", "r");
        char line[2000];
        fgets(line, sizeof(line), file);
        int len = strlen(line);
        fclose(file);
        
        int exists = 0;
        for(int i = 3 ; i < size; i++)
        {
            if(strstr(line, word[i]) != NULL)
            {
                exists = 1;
                break;
            }
        }
        if(exists)
        {
            printf(red "Commit ID : %d\t\t" resetcolor, id);
        
            file = fopen("branchName", "r");
            char branchName[1000];
            fgets(branchName, sizeof(branchName), file);
            int len = strlen(branchName);
            if(branchName[len - 1] == '\n')
            {
                branchName[len - 1] = '\0';
            }
            fclose(file);
            printf(red "Branch : %s\n" resetcolor, branchName);

            file = fopen("owner", "r");
            char username[2000], email[2000];
            fgets(username, sizeof(username), file);
            len = strlen(username);
            if(username[len - 1] == '\n')
                username[len - 1] = '\0';

            fgets(email, sizeof(email), file);
            len = strlen(email);
            if(email[len - 1] == '\n')
                email[len - 1] = '\0';

            fclose(file);

            printf(green "Owners username : %s\tOwners email : %s\n" resetcolor, username, email);
        
            file = fopen("message", "r");
            char message[1000];
            fgets(message, sizeof(message), file);
            len = strlen(message);
            if(message[len - 1] == '\n')
                message[len - 1] = '\0';
            fclose(file);
            
            printf(cyan "Commit message : %s\n" resetcolor, message);

            file = fopen("timeAndDate", "r");
            char time[1000];
            fgets(time, sizeof(time), file);
            len = strlen(time);
            if(time[len - 1] == '\n')
                time[len - 1] = '\0';
            fclose(file);

            printf("Committed on : %s\t\t", time);

            file = fopen("numberOfCommitedFiles", "r");
            char number[1000];
            fgets(number, sizeof(number), file);
            len = strlen(number);
            if(number[len - 1] == '\n')
                number[len - 1] = '\0';
            fclose(file);

            printf("Number of committed files : %s\n", number);


        }

        chdir("..");
        id --;
    }


    chdir(cwd);
    return 0;
}

int checkoutId(char id[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    if(access("staging/stagingDoc", F_OK) != -1)
    {
        perror("You should commit or reset all your staged changes first!\n");
        return 1;
    }

    FILE* file = fopen("currentBranch", "r");
    char branchName[2000];
    fgets(branchName, sizeof(branchName), file);
    fclose(file);

    chdir("commits");
    chdir(id);

    file = fopen("branchName", "r");
    char secName[2000];
    fgets(secName, sizeof(secName), file);
    fclose(file);
    if(strcmp(secName, branchName) != 0)
    {
        perror("You have to checkout to the branch of the given commit first!\n");
        return 1;
    }

    file = fopen("tracks", "r");
    char line[2000];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
            line[len - 1] = '\0';

        char name[2000];
        nameFinder(name, line);

        if(access(name, F_OK) == -1)
        {
            perror("something is wrong with your stored commit\n");
            return 1;
        }

        char command[5000];
        sprintf(command, "cp -f %s %s", name, line);
        system(command);

    }
    fclose(file);

    FILE* file2 = fopen("../../tracks", "r");
    while(fgets(line, sizeof(line), file2) != NULL)
    {
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
            line[len - 1] = '\0';

        file = fopen("tracks", "r");
        char line2[2000];
        int flag = 1;

        while(fgets(line2, sizeof(line2), file) != NULL)
        {
            int len = strlen(line2);
            if(line2[len - 1] == '\n' && len > 0)
                line2[len - 1] = '\0';

            if(strcmp(line2, line) == 0)
            {
                flag = 0; 
                break;
            }
        }

        fclose(file);
        if(flag)
        {
            remove(line);
        }
    }
    fclose(file2);

    char command[3000];
    sprintf(command, "cp -f tracks ../../tracks");
    system(command);

    chdir("../..");

    chdir("branches");
    int len = strlen(branchName);
    if(branchName[len - 1] == '\n' && len > 0)
        branchName[len - 1] = '\0';

    FILE* file3 = fopen(branchName, "r");
    int lastId;
    char ID2[20];
    fscanf(file3, "%d", &lastId);
    fclose(file3);
    sprintf(ID2, "%d", lastId);
    chdir("..");
    if(strcmp(id, ID2) == 0)
    {
        file = fopen("head", "w");
        fprintf(file, "HEAD");
        fclose(file);
        chdir(cwd);
        return 0;
    }

    file = fopen("head", "w");
    fprintf(file, "Detached (%s)", id);
    fclose(file);

    chdir(cwd);
    return 0;
}

int checkoutHead()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE *file = fopen("currentBranch", "r");
    char branch[2000];
    fgets(branch, sizeof(branch), file);
    fclose(file);
    int len = strlen(branch);
    if(branch[len - 1] == '\n' && len > 0)
    {
        branch[len - 1] = '\0';
    }
    chdir("branches");

    file = fopen(branch, "r");
    int id;
    fscanf(file, "%d", &id);
    fclose(file);
    chdir("..");

    char ID[20];
    sprintf(ID, "%d", id);
    
    chdir(cwd);
    return(checkoutId(ID));
}

int checkoutBranch(char branch[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* branches = fopen("branchList", "r");
    char line[2000];
    int flag = 1;
    while(fgets(line, sizeof(line), branches) != NULL)
    {
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
            line[len - 1] = '\0';

        if(strcmp(line, branch) == 0)
        {
            flag = 0;
            break;
        }
    }
    fclose(branches);
    if(flag)
    {
        perror("No branch exists with the given name!\n");
        return 1;
    } 

    FILE *file = fopen("currentBranch", "w");
    fprintf(file, "%s", branch);
    fclose(file);
    chdir(cwd);
    return(checkoutHead());
}

int checkoutHeadn(int n, int id)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;
    
    char ID[20];
    sprintf(ID, "%d", id);
    if(n == 0)
    {
        checkoutId(ID);
        chdir(cwd);
        return 0;
    }

    chdir("commits");
    chdir(ID);

    FILE* previd = fopen("prevId", "r");
    int pre;
    fscanf(previd, "%d", &pre);
    fclose(previd);
    chdir(cwd);

    return (checkoutHeadn(n - 1, pre));

}

int revert(char* message, char id[], int commitExistance, char email[], char username[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    if(checkoutId(id))
        return 1;

    FILE* file = fopen("head", "w");
    fprintf(file, "HEAD");
    fclose(file);

    file = fopen("currentBranch", "r");
    char branch[2000];
    fgets(branch, sizeof(branch), file);
    fclose(file);
    int len = strlen(branch);
    if(branch[len - 1] == '\n' && len > 0)
    {
        branch[len - 1] = '\0';
    }
    chdir("branches");

    file = fopen(branch, "w");
    fprintf(file, "%s", id);
    fclose(file);
    chdir("..");
    
    if(commitExistance)
    {
        if(message == NULL)
        {
            chdir("commits");
            chdir(id);
            FILE* file = fopen("message", "r");
            char new[2000];
            fgets(new, sizeof(new), file);
            fclose(file);
            message = new;
            chdir("../..");
        }
        FILE* tmp = fopen("staging/stagingDoc", "w");
        fclose(tmp);

        if(commit(message, email, username))
            return 1;

        remove("staging/stagingDoc");

    }
    chdir(cwd);
    return 0;
}

int revertx(char* message, int ID, int n, int commitExistance, char email[], char username[])
{

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    if(checkoutHeadn(n, ID))
        return 1;

    char id[20];
    FILE* file = fopen("head", "r");
    int Id;
    fscanf(file, "Detached (%d)", &Id);
    fclose(file);
    sprintf(id, "%d", Id);

    file = fopen("head", "w");
    fprintf(file, "HEAD");
    fclose(file);

    file = fopen("currentBranch", "r");
    char branch[2000];
    fgets(branch, sizeof(branch), file);
    fclose(file);
    int len = strlen(branch);
    if(branch[len - 1] == '\n' && len > 0)
    {
        branch[len - 1] = '\0';
    }
    chdir("branches");

    file = fopen(branch, "w");
    fprintf(file, "%s", id);
    fclose(file);
    chdir("..");
    
    if(commitExistance)
    {
        if(message == NULL)
        {
            chdir("commits");
            chdir(id);
            FILE* file = fopen("message", "r");
            char new[2000];
            fgets(new, sizeof(new), file);
            fclose(file);
            message = new;
            chdir("../..");
        }
        FILE* tmp = fopen("staging/stagingDoc", "w");
        fclose(tmp);

        if(commit(message, email, username))
            return 1;

        remove("staging/stagingDoc");

    }
    chdir(cwd);
    return 0;



}

int createTag(char name[], char *message, char email[], char username[], int force, int id)
{ 
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;
 
    chdir("tags");
    DIR* dir = opendir(".");
    struct dirent* entry;
    int exists = 0;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, name) == 0)
        {
            exists = 1;
            if(force)
            {
                char command[2000];
                sprintf(command, "rm -r %s", name);
                system(command);
            }
            break;
        }
    }
    closedir(dir);
    if(exists && (force == 0))
    {
        perror("This tag already exists!\n");
        return 1;
    }

    if (mkdir(name, 0755) != 0) 
    {    
        perror("unable to create the tag directory!\n");
        return 1;
    }

    chdir(name);

    FILE* owner = fopen("owner", "w");
    fprintf(owner, "%s\n%s", username, email);
    fclose(owner);

    FILE* commitId= fopen("commitId", "w");
    fprintf(commitId, "%d", id);
    fclose(commitId);

    FILE* timeAndDate = fopen("timeAndDate", "w");
    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    fprintf(timeAndDate, "%s\n", asctime(localTime));
    fclose(timeAndDate);

    FILE* messagee = fopen("message", "w");
    fprintf(messagee, "%s", message);
    fclose(messagee);

    chdir(cwd);
    return 0;
}

int showallTags()
{
    char tags[100][1000];
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;
 
    chdir("tags");
    DIR* dir = opendir(".");
    struct dirent* entry;
    int size = 0;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            strcpy(tags[size], entry->d_name);
            size ++;
        }
    }
    closedir(dir);
    for(int i = 0 ; i < size - 1 ; i++)
        for(int j = 0; j < size - i - 1; j++)
        {
            if(strcmp(tags[i], tags[i + 1]) > 0)
            {
                char tmp[2000];
                strcpy(tmp, tags[i]);
                strcpy(tags[i], tags[i + 1]);
                strcpy(tags[i + 1], tmp);
            }
        }
    for(int i = 0 ; i < size; i++)
        printf("%s\n", tags[i]);

    chdir(cwd);
    return 0;
}

int showTagDetailed(char name[])
{
    char tags[100][1000];
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    chdir("tags");
    if(chdir(name) != 0)
    {
        perror("This tag doesn't exist!\n");
        return 1;
    }

    printf(yellow "Tag name : %s\n" resetcolor, name);

    FILE* file = fopen("commitId", "r");
    int id;
    fscanf(file, "%d", &id);
    fclose(file);
    printf(red "Commit ID : %d\n" resetcolor, id);

    file = fopen("owner", "r");
    char email[2000], username[2000];

    fgets(username, sizeof(username), file);
    int len = strlen(username);
    if(username[len - 1] == '\n' && len > 0)
        username[len - 1] = '\0';

    fgets(email, sizeof(email), file);
    len = strlen(email);
    if(email[len - 1] == '\n' && len > 0)
        email[len - 1] = '\0';  

    fclose(file);
    printf(green "Author : %s <%s>\n", username, email);

    file = fopen("message", "r");
    char message[2000];
    fgets(message, sizeof(message), file);
    len = strlen(message);
    if(message[len - 1] == '\n' && len > 0)
        message[len - 1] = '\0';
    fclose(file); 
    printf(cyan "Message : %s\n" resetcolor , message);

    file = fopen("timeAndDate", "r");
    char date[2000];
    fgets(date, sizeof(date), file);
    len = strlen(date);
    if(date[len - 1] == '\n' && len > 0)
        date[len - 1] = '\0';
    fclose(file); 
    printf("Date : %s\n", date);

    chdir(cwd);
    return 0;
}

typedef struct gholab
{
    char hookname[100];
    int status;
}hook;

typedef struct pish
{
    char name[1000];
    hook hooks[10];

}precommit;

int todo_hook(char name[])
{
    FILE* file = fopen(name, "r");
    char line[2000];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char* pointer;
        if((pointer = strstr(line, "//")) != NULL && (strstr(name, ".c") != NULL || strstr(name, ".cpp") != NULL))
        {
            if(strstr(pointer , "TODO") != NULL)
            {
                fclose(file);
                return 0;
            }
        }
        if(strstr(line, "TODO") != NULL && strstr(name, ".txt") != NULL)
        {
            fclose(file);
            return 0;
        }
    }
    return 1;
}

int balance_braces(char name[])
{

    FILE *file = fopen(name, "r");
    int brace;
    int num = 0;
    int numb = 0;
    int numbe = 0;
    int place = 0;
    while((brace = fgetc(file)) != EOF)
    {
        place ++;
        int num2 = num;
        if(brace == '(')
        {         
            num ++;
            
            FILE* file2 = fopen(name, "r");
            int brace2;
            int flag = 1;
            int num3 = num;
            int place2 = 0;
            while((brace2 = fgetc(file2)) != EOF)
            {
                place2 ++;
                if(brace2 == ')')
                {
                    if(num3 == 1)
                    {
                        if(place2 < place)
                            return 0;
                            
                        flag = 0;
                        break;
                    }
                    else
                    {
                        num3 --;
                        continue;
                    }
                }
            }
            fclose(file2);
            if(flag)
            {
                return 0;
            }
        }
        if(brace == '{')
        {
            numb ++;
            
            FILE* file2 = fopen(name, "r");
            int brace2;
            int flag = 1;
            int num3 = numb;
            int place2 = 0;
            while((brace2 = fgetc(file2)) != EOF)
            {
                place2 ++;
                if(brace2 == '}')
                {
                    if(num3 == 1)
                    {
                        if(place2 < place)
                            return 0;
                        flag = 0;
                        break;
                    }
                    else
                    {
                        num3 --;
                        continue;
                    }
                }
            }
            fclose(file2);
            if(flag)
            {
                return 0;
            }
        }
        if(brace == '[')
        {
            numbe ++;
        
            FILE* file2 = fopen(name, "r");
            int brace2;
            int flag = 1;
            int num3 = numbe;
            int place2 = 0;
            while((brace2 = fgetc(file2)) != EOF)
            {
                place2 ++;
                if(brace2 == ']')
                {
                    
                    if(num3 == 1)
                    {
                        if(place2 < place)
                        {
                            return 0;
                        }
                        flag = 0;
                        break;
                    }
                    else
                    {
                        num3 --;
                        continue;
                    }
                }
            }
            fclose(file2);
            if(flag)
            {
                return 0;
            }
        }
    }
    fclose(file);
    return 1;
}

int charLimit(char name[])
{
    FILE* file = fopen(name, "r");
    int character;
    int num = 0;
    while((character = fgetc(file)) != EOF)
    {
        num ++;
        if(num > 20000)
            return 0;
    }
    fclose(file);
    return 1;
}

int sizeCheck(char name[])
{
    FILE* file = fopen(name, "rb");

    fseek(file, 0, SEEK_END);
    long long size = ftell(file);
    fclose(file);

    if(size > 5000000)
        return 0;

    return 1;
}

int whitespace(char name[])
{
    FILE* file = fopen(name, "r");
    int character;
    char save;
    while((character = fgetc(file)) != EOF)
    {
        save = character;
    }
    if(save == ' ' || save == '\n' || save == '\t')
        return 0;

    return 1;
}

int pre_commit()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* file = fopen("hooks", "r");
    chdir("staging");
    DIR* dir = opendir(".");
    struct dirent* entry;
    int size = 0;
    precommit elements[500];
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "stagingDoc") != 0)
        {
            strcpy(elements[size].name , entry->d_name);
            size ++;
        }
    }
    closedir(dir);

    char line[2000];
    int num = 0;
    while(fgets(line, sizeof(line), file) != NULL)
    {
        num ++;
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
        {
            line[len - 1] = '\0';
        }
        if(strcmp(line, "todo-check") == 0)
        {
            for(int i = 0 ; i < size; i++)
            {
                if(strstr(elements[i].name, ".c") != NULL || strstr(elements[i].name, ".cpp") != NULL || strstr(elements[i].name, ".txt") != NULL)
                {
                    strcpy(elements[i].hooks[0].hookname, "todo-check");
                    elements[i].hooks[0].status = todo_hook(elements[i].name);
                }
                else
                {   
                    strcpy(elements[i].hooks[0].hookname, "todo-check");
                    elements[i].hooks[0].status = -1;
                }
            }
        }
        if(strcmp(line, "format-check") == 0)
        {
            for(int i = 0 ; i < size; i++)
            {
                if(strstr(elements[i].name, ".c") != NULL || strstr(elements[i].name, ".cpp") != NULL || strstr(elements[i].name, ".txt") != NULL || strstr(elements[i].name, ".mp3") != NULL|| strstr(elements[i].name, ".mp4") != NULL|| strstr(elements[i].name, ".exe") != NULL|| strstr(elements[i].name, ".wav") != NULL)
                {
                    strcpy(elements[i].hooks[1].hookname, "format-check");
                    elements[i].hooks[1].status = 1;
                }
                else 
                {
                    strcpy(elements[i].hooks[1].hookname, "format-check");
                    elements[i].hooks[1].status = 0;
                }
            }
        }
        if(strcmp(line, "balance-braces") == 0)
        {
            for(int i = 0 ; i < size; i++)
            {
                if(strstr(elements[i].name, ".c") != NULL || strstr(elements[i].name, ".cpp") != NULL || strstr(elements[i].name, ".txt") != NULL)
                {
                    strcpy(elements[i].hooks[2].hookname, "balance-braces");
                    elements[i].hooks[2].status = balance_braces(elements[i].name);
                }
                else
                {   
                    strcpy(elements[i].hooks[2].hookname, "balance-braces");
                    elements[i].hooks[2].status = -1;
                }
            }
        }
        if(strcmp(line, "character-limit") == 0)
        {
            for(int i = 0 ; i < size; i++)
            {
                if(strstr(elements[i].name, ".c") != NULL || strstr(elements[i].name, ".cpp") != NULL || strstr(elements[i].name, ".txt") != NULL)
                {
                    strcpy(elements[i].hooks[3].hookname, "character-limit");
                    elements[i].hooks[3].status = charLimit(elements[i].name);
                }
                else
                {   
                    strcpy(elements[i].hooks[3].hookname, "character-limit");
                    elements[i].hooks[3].status = -1;
                }
            }
        }
        if(strcmp(line, "file-size-check") == 0)
        {
            for(int i = 0 ; i < size; i++)
            {
                strcpy(elements[i].hooks[4].hookname, "file-size-check");
                elements[i].hooks[4].status = sizeCheck(elements[i].name);
                
            }
        }
        if(strcmp(line, "eof-blank-space") == 0)
        {
            for(int i = 0 ; i < size; i++)
            {
                if(strstr(elements[i].name, ".c") != NULL || strstr(elements[i].name, ".cpp") != NULL || strstr(elements[i].name, ".txt") != NULL)
                {
                    strcpy(elements[i].hooks[5].hookname, "eof-blank-space");
                    elements[i].hooks[5].status = whitespace(elements[i].name);
                }
                else
                {   
                    strcpy(elements[i].hooks[5].hookname, "eof-blank-space");
                    elements[i].hooks[5].status = -1;
                }
            }
        }

    }

    for(int i = 0 ; i < size; i++)
    {
        printf(yellow "\"%s\"\n" resetcolor, elements[i].name);
        for(int j = 0 ; j < num; j++)
        {
            printf("%s", elements[i].hooks[j].hookname);
            int dots = 60 - strlen(elements[i].hooks[j].hookname) - 6;
            for(int k = 0 ; k < dots; k++)
                printf(".");
            
            if(elements[i].hooks[j].status == 1)
                printf(green "PASSED\n" resetcolor);

            else if(elements[i].hooks[j].status == 0)
                printf(red "FAILED\n" resetcolor);

            else
                printf(cyan "SKIPPED\n" resetcolor);
        }
    }

    chdir(cwd);
    return 0;
}

int grep(char word[], char path[], char commit[], int showline)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE *file;

    if(strcmp(commit, "current") == 0)
    {
        file = fopen(path, "r");
    }
    else
    {
        chdir("commits");
        chdir(commit);
        char name[2000];
        nameFinder(name, path);    
        chdir(commit);
        file = fopen(name, "r");
        chdir("../..");

    }
    
    char line[2000];
    int num = 0;
    while(fgets(line, sizeof(line), file) != NULL)
    {
        num ++;
        int len = strlen(line);
        if(line[len - 1] == '\n' && len > 0)
            line[len - 1] = '\0';

        char* pointer = line;
        char* string = line;
        int flag = 0;
        int flag2 = 1;
        while((pointer = strstr(pointer, word)) != NULL)
        {
            flag = 1;
            if(showline && flag2)
            {
                printf(red "%d." resetcolor, num);
                flag2 = 0;
            }

            printf(cyan "%.*s" resetcolor, (int)(pointer - string), string);
            printf(yellow "%s" resetcolor, word);
            pointer += strlen(word);
            string = pointer;
        } 
        if(flag)
        {
            printf(cyan "%s" resetcolor, string);         
            printf("\n");
        }

    }
    fclose(file);

    chdir(cwd);
    return 0;
}

int main(int argc, char * argv[])
{
    if (argc < 2) 
    {
        perror("please enter a valid command\n");
        return 1;
    }

    char email[1000];
    char username[1000];

    int size = 0;
    int shortcutsSize = 0;
    alias aliases[500];
    shortcut shortcuts[500];
    if(strcmp(argv[1], "config") != 0 || strcmp(argv[2], "-global") != 0)
    {
        getTheNewestGlobalAliases(aliases, &size);
        if(strcmp(argv[1] , "init") != 0)
        {
            getTheNewestLocalAliases(aliases, &size);
            ownersEmailFinder(email);         
            ownersUsernameFinder(username); 
            getTheNewestShortcutMessage(shortcuts, &shortcutsSize); 
            changeChecker();        
        }
    }
    

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


    if(strcmp(argv[1], "whoami") == 0)
    {
        printf("owners username : %s\n", username);
        printf("owners email : %s\n", email);
    }

    if(strcmp(argv[1], "config") == 0 && strcmp(argv[2], "-global") == 0)
    {
        if(strcmp(argv[3], "user.name") == 0)
        {
            strcpy(username, argv[4]);
            globalUsernameChanger(username);
        }
        if(strcmp(argv[3], "user.email") == 0)
        {
            strcpy(email, argv[4]);
            globalEmailChanger(email);            
        }
        if(strncmp(argv[3], "alias", 5) == 0)
        {
            char aliasName[1000];
            sscanf(argv[3], "alias.%s", aliasName);
            if(strncmp(argv[4], "mml", 3) != 0)
            {
                perror("Invalid command\n");
                return 1;
            }

            if(globalAlias(argv[4], aliasName))
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
        if(strcmp(argv[2], "-f") == 0)
        {
            for(int i = 3; i < argc; i++)
            {
                
                char* path = realpath(argv[i], NULL);
                add(path);
            }
        }
        else if(strcmp(argv[2], "-n") == 0)
        {
            if (argc == 3)
                return (showCurrentStagings(0, 1));
            int depth;
            sscanf(argv[3], "%d", &depth);
            if(showCurrentStagings(0, depth))
                return 1;
        }
        else
        {
            for(int i = 2; i < argc; i++)
            {
                char* path = realpath(argv[i], NULL);
                add(path);
            }
        }
        
    }

    if(strcmp(argv[1], "reset") == 0)
    {
        if(strcmp(argv[2], "-f") == 0)
        {
            for(int i = 3; i < argc; i++)
            {
                char* path = realpath(argv[i], NULL);
                if(reset(path))
                    return 1;
            }
            return 0;
        }

        for(int i = 2; i < argc; i++)
        {
            char* path = realpath(argv[i], NULL);
            if(reset(path))
                return 1;
        }
    }

    if(strcmp(argv[1], "commit") == 0)
    {
        if(argc < 4)
        {
            perror("You should add a message!\n");
            return 1;
        }
        if(strcmp(argv[2], "-m") == 0)
        {
            if(strlen(argv[3]) > 72)
            {
                perror("your commit message should not be more than 72 characters!\n");
                return 1;
            }
            if(argv[3] == NULL)
            {
                perror("you should add a message!\n");
                return 1;
            }
            if(commit(argv[3], email, username))
                return 1;

        }

        if(strcmp(argv[2], "-s") == 0)
        {
            for(int i = 0 ; i < shortcutsSize; i++)
            {
                if(strcmp(shortcuts[i].newMessage, argv[3]) == 0)
                {
                    char command[3000];
                    sprintf(command, "mml commit -m \"%s\"", shortcuts[i].oldMessage);
                    system(command);
                    return 0;
                }
            }
            perror("There is no shortcut with the given name!\n");
            return 1;

        }
    }
    if(strcmp(argv[1], "set") == 0 && strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
    {
        if(addShortcut(argv[3], argv[5]))
            return 1;

    }

    if(strcmp(argv[1] ,"remove") == 0 && strcmp(argv[2] ,"-s") == 0)
    {
        if(removeShortcut(argv[3]))
            return 1;
    }

    if(strcmp(argv[1], "replace") == 0 && strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
    {
        if(removeShortcut(argv[5]))
            return 1;

        if(addShortcut(argv[3], argv[5]))
            return 1;
    }

    if(strcmp(argv[1], "status") == 0)
    {
        if(status())
            return 1;
    }

    if(strcmp(argv[1], "branch") == 0)
    {
        if(argc == 3)
        {
            if(argv[2][0] >= '0' && argv[2][0] <= '9')
            {
                perror("The first character of you branch name can't be a digit!\n");
                return 1;
            }
            if(branch(argv[2]))
                return 1;
        }
        if(argc == 2)
        {
            if(showBranches())
                return 1;
        }
    }

    if(strcmp(argv[1], "log") == 0)
    {
        if(argc == 2)
        {
            if(mmllog(-1))
                return 1;
        }
        else if(strcmp(argv[2], "-n") == 0)
        {
            int n;
            sscanf(argv[3], "%d", &n);
            if(mmllog(n))
                return 1;
        }
        else if(strcmp(argv[2], "-branch") == 0 && argc < 5)
        {
            return(mmllogBranch(argv[3]));
        }
        else if(strcmp(argv[2], "-author") == 0 && argc < 5)
        {
            return(mmllogAuthor(argv[3]));
        }
        else if(strcmp(argv[2], "-before") == 0 && argc < 5)//year month date hour:min:sec
        {
            return(mmllogDate(argv[3], 0));
        }
        else if(strcmp(argv[2], "-since") == 0 && argc < 5)//year month date hour:min:sec
        {
            return(mmllogDate(argv[3], 1));
        }
        else if(strcmp(argv[2], "-format") == 0 && argc < 5)
        {
            printf("for -before and -since use the format \"year month(in numbers) day(in month in numbers) hour:min:sec\"\n");
            return 0;
        }
        else if(strcmp(argv[2], "-search") == 0 && argc > 3)
        {
            return(mmllogSearch(argv, argc));
        }


    }

    if(strcmp(argv[1], "checkout") == 0)
    {
        if(strcmp(argv[2], "HEAD") == 0)
        {
            if(argc == 3)
                return(checkoutHead());

            if(argc == 4)
            {
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    return 1;

                if(go_to_mmlrepo())
                    return 1;
                    
                FILE *file = fopen("currentBranch", "r");
                char branch[2000];
                fgets(branch, sizeof(branch), file);
                fclose(file);
                int len = strlen(branch);
                if(branch[len - 1] == '\n' && len > 0)
                {
                    branch[len - 1] = '\0';
                }
                chdir("branches");

                file = fopen(branch, "r");
                int id;
                fscanf(file, "%d", &id);
                fclose(file);
                chdir("..");

                int n;
                sscanf(argv[3], "-%d", &n);
                chdir(cwd);
                return(checkoutHeadn(n, id));
            }
        }
        else if(argv[2][0] >= '0' && argv[2][0] <= '9')
        {
            return(checkoutId(argv[2]));
        }
        else
        {
            return(checkoutBranch(argv[2]));
        }
    }

    if(strcmp(argv[1], "revert") == 0)
    {
        if(strcmp(argv[2], "-m") == 0)
        {
            if(strcmp(argv[4], "HEAD") == 0)
            {
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    return 1;

                if(go_to_mmlrepo())
                    return 1;
                    
                FILE *file = fopen("currentBranch", "r");
                char branch[2000];
                fgets(branch, sizeof(branch), file);
                fclose(file);
                int len = strlen(branch);
                if(branch[len - 1] == '\n' && len > 0)
                {
                    branch[len - 1] = '\0';
                }
                chdir("branches");

                file = fopen(branch, "r");
                int id;
                fscanf(file, "%d", &id);
                fclose(file);
                chdir("..");

                int n;
                sscanf(argv[5], "-%d", &n);
                chdir(cwd);
                return(revertx(argv[3], id, n, 1, email, username));
            }
            return (revert(argv[3], argv[4], 1, email, username));
        }
        if(strcmp(argv[2], "-n") == 0)
        {
            return (revert(NULL, argv[3], 0, email, username));
        }
        else
        {
            if(strcmp(argv[2], "HEAD") == 0)
            {
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    return 1;

                if(go_to_mmlrepo())
                    return 1;
                    
                FILE *file = fopen("currentBranch", "r");
                char branch[2000];
                fgets(branch, sizeof(branch), file);
                fclose(file);
                int len = strlen(branch);
                if(branch[len - 1] == '\n' && len > 0)
                {
                    branch[len - 1] = '\0';
                }
                chdir("branches");

                file = fopen(branch, "r");
                int id;
                fscanf(file, "%d", &id);
                fclose(file);
                chdir("..");

                int n;
                sscanf(argv[3], "-%d", &n);
                chdir(cwd);
                return(revertx(NULL, id, n, 1, email, username));
            }
            return (revert(NULL, argv[2], 1, email, username));
        }

    }

    if(strcmp(argv[1], "tag") == 0)
    {
        if(argc == 2)
        {
            return(showallTags());
        }
        if(strcmp(argv[2], "show") == 0)
        {
            return (showTagDetailed(argv[3]));
        }
        if(strcmp(argv[2], "-a") == 0)
        {
            char message[2000] = " ";
            int id = 0;
            int force = 0;
            if(argc == 4)
            {
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    return 1;

                if(go_to_mmlrepo())
                    return 1;

                FILE* file = fopen("head", "r"); 
                char state[2000];
                fgets(state, sizeof(state), file);
                fclose(file);

                if(state[0] == 'D')
                {
                    sscanf(state, "Detached (%d)", &id);
                }

                else
                {
                    file = fopen("currentBranch", "r");
                    char branch[2000];
                    fgets(branch, sizeof(branch), file);
                    fclose(file);
                    int len = strlen(branch);
                    if(branch[len - 1] == '\n' && len > 0)
                    {
                        branch[len - 1] = '\0';
                    }

                    chdir("branches");
                    file = fopen(branch, "r");
                    fscanf(file, "%d", &id);
                    fclose(file);
                }
                chdir(cwd);

                return(createTag(argv[3], " ", email, username, 0, id));
            }
            if(argc > 4)
            {
                if(strcmp(argv[4], "-m") == 0)
                {
                    strcpy(message, argv[5]);
                }
                
                if(strcmp(argv[4], "-c") == 0)
                {
                    sscanf(argv[5], "%d", &id);
                }
                if(strcmp(argv[4], "-f") == 0)
                {
                    force = 1;
                }
            }
            if(argc > 6)
            {
                if(strcmp(argv[6], "-c") == 0)
                {
                    sscanf(argv[7], "%d", &id);
                }
                if(strcmp(argv[6], "-f") == 0)
                {
                    force = 1;
                }
            }
            if(argc > 8)
            {
                if(strcmp(argv[8], "-f") == 0)
                {
                    force = 1;
                }
            }
            
            if(id == 0)
            {
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    return 1;

                if(go_to_mmlrepo())
                    return 1;

                FILE* file = fopen("head", "r"); 
                char state[2000];
                fgets(state, sizeof(state), file);
                fclose(file);

                if(state[0] == 'D')
                {
                    sscanf(state, "Detached (%d)", &id);
                }

                else
                {
                    file = fopen("currentBranch", "r");
                    char branch[2000];
                    fgets(branch, sizeof(branch), file);
                    fclose(file);
                    int len = strlen(branch);
                    if(branch[len - 1] == '\n' && len > 0)
                    {
                        branch[len - 1] = '\0';
                    }

                    chdir("branches");
                    file = fopen(branch, "r");
                    fscanf(file, "%d", &id);
                    fclose(file);
                }
                chdir(cwd);

            }
            return(createTag(argv[3] , message, email, username, force , id));

        }
    }

    if(strcmp(argv[1], "pre-commit") == 0)
    {
        if(argc == 2)
        {
            return (pre_commit());
        }
        if(strcmp(argv[2], "hooks") == 0 && strcmp(argv[3], "list") == 0)
        {
            printf(yellow "ID : todo-check\n" resetcolor);
            printf(cyan "Description : There should not be any TODO commnets in c or cpp files and no TODO s in txt files\n" resetcolor);
            printf(red "File formats : .c & .cpp & .txt\n");
            printf(yellow "ID : format-check\n" resetcolor);
            printf(cyan "Description : Checks if the format of the file is one of the 7 accepted formats\n" resetcolor);
            printf(red "File formats : .c & .cpp & .txt & .mp3 & .mp4 & .wav & .exe\n");
            printf(yellow "ID : balance-braces\n" resetcolor);
            printf(cyan "Description : Checks if for every opening brace \"([{\" there exists a closeing one\n" resetcolor);
            printf(red "File formats : .c & .cpp & .txt\n");
            printf(yellow "ID : character-limit\n" resetcolor);
            printf(cyan "Description : Checks if the number of charecters in the file are less than 20000\n" resetcolor);
            printf(red "File formats : .c & .cpp & .txt\n");
            printf(yellow "ID : file-size-check\n" resetcolor);
            printf(cyan "Description : Checks if the size of the file is less than 5 MB\n" resetcolor);
            printf(red "File formats : all formats\n");
            printf(yellow "ID : eof-blank-space\n" resetcolor);
            printf(cyan "Description : Checks for any blank spaces at the end of the file\n" resetcolor);
            printf(red "File formats : .c & .cpp & .txt\n");
            return 0;
        }
        if(strcmp(argv[2], "applied") == 0 && strcmp(argv[3], "hooks") == 0)
        {
            char tags[100][1000];
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL)
                return 1;

            if(go_to_mmlrepo())
                return 1;

            FILE* file = fopen("hooks", "r");
            if(file == NULL)
            {
                perror("You haven't added any hooks yet!\n");
                return 1;
            }

            char line[2000];
            while(fgets(line, sizeof(line), file) != NULL)
            {
                int len = strlen(line);
                if(line[len - 1] == '\n' && len > 0)
                {
                    line[len - 1] = '\0';
                }
                printf("%s\n", line);
            }
            fclose(file);

            chdir(cwd);
            return 0;
        }
        if(strcmp(argv[2], "add") == 0 && strcmp(argv[3], "hook") == 0)
        {
            char tags[100][1000];
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL)
                return 1;

            if(go_to_mmlrepo())
                return 1;
            
            FILE* file = fopen("hooks", "a");
            if(file == NULL)
            {
                FILE* file = fopen("hooks", "w");
            }
            fprintf(file, "%s\n", argv[4]);
            fclose(file);
            chdir(cwd);
            return 0;
        }
        if(strcmp(argv[2], "remove") == 0 && strcmp(argv[3], "hook") == 0)
        {
            char tags[100][1000];
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL)
                return 1;

            if(go_to_mmlrepo())
                return 1;

            FILE* file = fopen("hooks", "r");
            if(file == NULL)
            {
                perror("You haven't added any hooks yet!\n");
                chdir(cwd);
                return 1;
            }
            FILE* file2 = fopen("tmp", "w");
            char line[2000];
            int flag = 1;
            while(fgets(line, sizeof(line), file) != NULL)
            {
                int len = strlen(line);
                if(line[len - 1] == '\n' && len > 0)
                {
                    line[len - 1] = '\0';
                }
                if(strcmp(argv[4], line) != 0)
                {
                    fprintf(file2, "%s\n", line);
                }
                else
                {
                    flag = 0;
                }
            }
            fclose(file2);
            fclose(file);
            if(flag)
            {
                remove("tmp");
                perror("You have not added this hook yet!\n");
                chdir(cwd);
                return 1;
            }
            remove("hooks");
            rename("tmp", "hooks");
            chdir(cwd);
            return 0;
        }
    }

    if(strcmp(argv[1], "grep") == 0 && strcmp(argv[2], "-f") == 0 && strcmp(argv[4], "-p") == 0)
    {
        int commitExistance = 0;
        int lineShower = 0;
        char* path = realpath(argv[3], NULL);
        char commit[2000] = "current";
        if(argc > 6)
        {
            if(strcmp(argv[6], "-c") == 0)
            {
                strcpy(commit, argv[7]);
                commitExistance = 1;
            }
            if(strcmp(argv[6], "-n") == 0)
            {
                lineShower = 1;
            }
        }
        if(argc > 8)
        {
            if(strcmp(argv[8], "-n") == 0)
            {
                lineShower = 1;
            }
        }
        return (grep(argv[5], path, commit, lineShower));

    }

}