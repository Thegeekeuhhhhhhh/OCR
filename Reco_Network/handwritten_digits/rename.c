#include <dirent.h> 
#include <stdio.h> 
#include <string.h>

int main(void)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) 
    {
        char str[150];
        int i = 1;
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_name[0] == 'i' || (dir->d_name[0] >= '0' && dir->d_name[0] <= '9'))
            {
                sprintf(str, "%i.jpg", i);
                //printf("%s\n", dir->d_name);
                rename(dir->d_name, str);
                i += 1;
            }
        }
        closedir(d);
    }
    return(0);
}
