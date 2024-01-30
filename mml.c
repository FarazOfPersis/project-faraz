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

    file = fopen(".mml/configBranch", "w");
    if (file == NULL) 
    {
        perror("an error accurred!\n");
        return 1;
    }
    fprintf(file, "branch:%s", "master");
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
            strcpy(username, finall);
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
    if(access(path, F_OK)== -1)
    {
        perror("Invalid path!\n");
        return 1;
    }
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
    sprintf(command, "cp %s %s", path, currentPlace);
    system(command);

    if(chdir("..") != 0)
        return 1;

    FILE * tracks = fopen("tracks", "r");
    char line[2000];
    int flag = 1;
    while(fgets(line, sizeof(line), tracks) != NULL)
    {
        int len = strlen(line);
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
    fclose(tracks);

    if(flag)
    {
        FILE* tracks = fopen("tracks", "a");
        fprintf(tracks, "%s\n", path);
        fclose(tracks);
    }

    chdir(cwd);
    return 0;

}

int showCurrentStagings()
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

            if(flag)
            {
                printf("%s : staged\n", entry->d_name);
            }
            else
            {
                printf("%s : not staged\n", entry->d_name);

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
        
        char line[2000];

        int exists = 1;
        while(fgets(line, sizeof(line), stagingfile) != NULL)
        {
            int len = strlen(line);
            if(line[len - 1] == '\n' && len > 0)
                line[len - 1] = '\0';
            
            if(strcmp(line, path) == 0)
            {           

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

        chdir(cwd);
        return 0;
    
}

int commit(char message[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    if(go_to_mmlrepo())
        return 1;

    FILE* lastCommitId = fopen("configLastCommitID", "r");
    int id;
    fscanf(lastCommitId,"last_commit_ID : %d" , &id);
    fclose(lastCommitId);

    lastCommitId = fopen("configLastCommitID", "w");
    fprintf(lastCommitId, "last_commit_ID : %d", id + 1);
    fclose(lastCommitId);


    id ++;
    if(chdir("./commits") != 0)
    {
        perror("unable to access your commits directory!\n");
        return 1;
    }

    char ID[20];
    sprintf(ID, "%d", id);
    if(id != 1)
    {
        char command[2000], prevID[20];
        sprintf(prevID, "%d", id - 1);
        sprintf(command, "cp -r %s %s", prevID, ID);
        system(command);
    }
    else
    {

        if (mkdir(ID, 0755) != 0) 
        {    
            perror("unable to create the commit directory!\n");
            return 1;
        }
    }
    
    if(chdir(ID) != 0)
        return 1;

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

    char cwd2[1024];
    if (getcwd(cwd2, sizeof(cwd)) == NULL)
        return 1;
    //printf("%s", cwd2);

    if(chdir("..") != 0)
        return 1;

    if(chdir("..") != 0)
        return 1;
    
    if(chdir("staging") != 0)
        return 1;
    
    //printf("...");

    DIR* dir = opendir(".");
    struct dirent* entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name ,".." ) != 0 && strcmp(entry->d_name, "stagingDoc") != 0)
        {
            char command[2000];
            sprintf(command, "mv -f %s ../commits/%s/%s", entry->d_name, ID, entry->d_name);
            system(command);
        }
        
    }
    remove("stagingDoc");

    if(chdir("..") != 0)
        return 1;

    char newcommand[2000];
    sprintf(newcommand, "cp -f %s ./commits/%s/%s", "tracks", ID, "tracks");
    system(newcommand);

    chdir("commits");
    chdir(ID);

    FILE* tracks = fopen("tracks" ,"r");
    char path[2000];
    while(fgets(path, sizeof(path), tracks) != NULL)
    {
        int len = strlen(path);
        if(len > 0 && path[len - 1] == '\n')
        {
            path[len - 1] = '\0';
        }
        char name[2000];
        nameFinder(name, path);
            
        if(access(path, F_OK) == -1)
        {
            remove(name) == 0;
    
        }
    }
    fclose(tracks);

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
        getTheNewestLocalAliases(aliases, &size);
        ownersEmailFinder(email);         
        ownersUsernameFinder(username); 
        getTheNewestShortcutMessage(shortcuts, &shortcutsSize);         
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
            sscanf(argv[4], "alias.%s", aliasName);
            
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
                char path[2000];
                if(getcwd(path, sizeof(path)) == NULL)
                    return 1;

                int len = strlen(path);
                path[len] = '/';
                path[len + 1] = '\0';

                char new[2000];
                if(argv[i][0] == '.' && argv[i][1] == '/')
                {
                    int len = strlen(argv[i]);

                    for(int j = 0 ; j < len - 2; j++)
                    {
                        new[j] = argv[i][j + 2];
                    }
                }
                else
                {
                    strcpy(new, argv[i]);
                }

                strcat(path, new);
                add(path);
            }
        }
        else if(strcmp(argv[2], "-n") == 0)
        {
            if(showCurrentStagings())
                return 1;
        }
        else
        {
            for(int i = 2; i < argc; i++)
            {
                char path[2000];
                if(getcwd(path, sizeof(path)) == NULL)
                    return 1;

                int len = strlen(path);
                path[len] = '/';
                path[len + 1] = '\0';

                char new[2000];
                if(argv[i][0] == '.' && argv[i][1] == '/')
                {
                    int len = strlen(argv[i]);

                    for(int j = 0 ; j < len - 2; j++)
                    {
                        new[j] = argv[i][j + 2];
                    }
                }
                else
                {
                    strcpy(new, argv[i]);
                }

                strcat(path, new);
                add(path);
            }
        }
        
    }

    if(strcmp(argv[1], "reset") == 0)
    {
        for(int i = 2; i < argc; i++)
        {
            char path[2000];
            if(getcwd(path, sizeof(path)) == NULL)
                return 1;

            int len = strlen(path);
            path[len] = '/';
            path[len + 1] = '\0';

            char new[2000];
            if(argv[i][0] == '.' && argv[i][1] == '/')
            {
                int len = strlen(argv[i]);

                for(int j = 0 ; j < len - 2; j++)
                {
                    new[j] = argv[i][j + 2];
                }
            }
            else
            {
                strcpy(new, argv[i]);
            }

            strcat(path, new);
            reset(path);
        }
    }

    if(strcmp(argv[1], "commit") == 0)
    {
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
            if(commit(argv[3]))
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

}