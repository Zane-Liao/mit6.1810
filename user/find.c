#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
find(char *path, char *target_file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        close(fd);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
        }

        switch(st.type){
            case T_FILE:
                if(strcmp(target_file, de.name) == 0)
                    printf("%s\n", buf);
                break;
            case T_DIR:
                if((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0))
                    find(buf, target_file);           
        }
    }
    close(fd);
    return;
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "ERROR!!! Please input 2 arguments\n");
        exit(1);
    }
    char *path = argv[1];
    char *target_file = argv[2];
    find(path, target_file);
    exit(0);
}