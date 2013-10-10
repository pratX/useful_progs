#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

void accept_args(int, char**, char*, char*, char*);
void print(int, off_t, long);

int main(int argc, char *argv[]){
  char file_name[100], db_file_name[100];
  char  lines_to_print[50];
  accept_args(argc, argv, file_name, db_file_name, lines_to_print);
  //printf("file: %s\tdb_file: %s\tlines_to_print: %s\n", file_name, db_file_name, lines_to_print);
      
  int file, db_file;
  off_t offset=0;
  FILE *s_file, *s_db_file;
  struct stat file_stat, db_file_stat; 
  ino_t new_inode, old_inode;  
  if((file = open(file_name, O_RDONLY)) == -1){
    printf("Could not open file %s for reading\n", file_name);  
    return 0;
  }
  fstat(file, &file_stat);
  new_inode = file_stat.st_ino;

  //Read old_onode if db_file exists
  char s_old_inode[50];
  char *c = s_old_inode;
  char s_offset[20];
  if((db_file = open(db_file_name, O_RDWR)) >= 0){
    //s_db_file = fdopen(db_file, "r+")      
    //fscanf(s_db_file, "%ld", &old_inode);
    do{
      while ( (read(db_file, c, 1)) !=1 );
      c+=1;
    }while(*(c-1)!='\n');
    
    old_inode = atoll(s_old_inode);
    c = s_offset;
    if(old_inode == new_inode){
      //Read the num
      while(read(db_file, c++, 1) != 0);
      offset = atoll(s_offset);    
    }
  }
  else {
    if((db_file = open(db_file_name, O_WRONLY|O_CREAT)) == -1){
      printf("Could not open file %s\n",db_file_name);  
    }
  }
   
 // printf("inode:%ld\toffset:%ld\tnum_of_line:%ld\n", new_inode, offset, atoll(lines_to_print)); 
  
  //Now that we have the offset start printing 
  long l_to_p = atol(lines_to_print);
  print(file, offset, l_to_p);
  
  //Now that the printing has been done, overwrite the db_file
  lseek(db_file, 0, SEEK_SET);
  char buf[200];
  sprintf(buf,"%ld\n%ld", new_inode, lseek(file, 0, SEEK_CUR));
  int buf_size=strlen(buf); 
  int num=0;
  c = buf;
  do{
  while(write(db_file, c, 1)!=1);
  ++c;
  ++num;
  }while(num < buf_size);

  //THE END
  close(file);
  close(db_file);
}

void accept_args(int argc, char *argv[], char *file_name, char *db_file_name, char *lines_to_print){
  if(argc < 4){
      printf("%d\n",argc);
      printf("Invalid arguments\n");
      exit(0);
  }
  strncpy(file_name, argv[1], 99);
  strncpy(db_file_name, argv[2], 99);
  strncpy(lines_to_print, argv[3], 49);

}

void print(int file, off_t offset, long l_to_p){
  char c;
  long num=0;
  lseek(file, offset, SEEK_SET);
  while(read(file, &c, 1)!=0){
    printf("%c",c); 
    if(l_to_p > 0){
      if(c == '\n')
        ++num;
      if(num >= l_to_p)
        break; 
    }
  }
}
