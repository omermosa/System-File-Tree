#include <sys/types.h>
#include <sys/param.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include<vector>
#include<string>

using namespace std;


struct node
{
    string name;
    long long file_size=0;
    string dir;
    long int numc=0; // num of children
   std:: vector<struct node *> children;// max nodes

};



long long listdir(struct node *);

long int getsize(string path)
{
    struct tm dt;
    struct stat stats;
    // returns 0 on successful operation, otherwise  -1
    if (stat(path.c_str(), &stats) == 0)
    {
        return stats.st_size;;
    }
    else
    {
        return 0;
    }

}

void TraverseTree (struct node *rt)
{

    if(rt==NULL) return;

    for (int i=0;i< rt->numc;i++)
    {
        printf("%ld %s\n", rt->children[i]->file_size,(rt->children[i]->dir).c_str());
        TraverseTree(rt->children[i]);
    }

}


int main()
{
    // Directory path to list files
    
    struct node *rt=new node;   
    rt->dir="C:\";
   
    long int parent_dir_siz=listdir(rt);
    printf("dir is %s, size is %lld \n", rt->dir.c_str(), parent_dir_siz);
    printf("..................................................\n");
    
   // TraverseTree(rt);
    return 0;
}


/**
 * Lists all files and sub-directories recursively 
 * considering path as base path.
 */
long long listdir(struct node *rt)
{
    string path;
  
    DIR *dir = opendir((rt->dir).c_str());
  
  if(dir){
        struct dirent *dp;

    while ((dp = readdir(dir)) != NULL)
    {
       
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type != DT_LNK)
        {
          
          //  printf("%s\n",  strcat(path, dp->d_name));
              // create new node
                 
            struct node* temp= new node;
          //  printf("%s\n", rt->dir);
	         path =(rt->dir)+"/"+string(dp->d_name);
          
            temp->dir=path;
            
           rt->numc++;
           temp->name=dp->d_name;
           
            temp->file_size=getsize(path);
             rt->children.push_back(temp);

           if(dp->d_type==DT_REG)
            {
                rt->file_size +=getsize(path);
               
           }
                      
           if(dp->d_type==DT_DIR) rt->file_size+=listdir(temp);
           
        
        }
        
    }
   
    closedir(dir);
  }
 // printf("rt %s\n",rt->file_size);
    return rt->file_size;
}