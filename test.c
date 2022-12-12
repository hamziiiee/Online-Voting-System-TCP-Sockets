#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int check_in_file(const char *file,char* name,char *cnic){
    FILE *f;
    char data[512]; int _name =0, _cnic =0;

    f = fopen(file,"r");
    while (fgets(data,512,f)!=NULL)
    {
        if(strstr(data,name)!=NULL) _name=1;
        if(strstr(data,cnic)!=NULL) _cnic=1;

    }
    if (_name==1 && _cnic==1) return 1;
    else return -1;
    // printf("data: %s\n",data);
}

// void display_Candidates(const char* file,char *data){
//     FILE *fp;int i=0;
//     char buf[512];
//     fp=fopen(file,"r");
//     while(fscanf(fp,"%s",&buf[i])!=EOF){
//         i++;
//     }
//     fclose(fp);
//     // for(int i=0;i<strlen(buf);i++){
//         printf("%s",buf);
//     // }
// }
void display_Candidates(const char* file,char *data){
    FILE *fp;
    long size =512;
    char *string;
    char temp[2000];
    fp=fopen(file,"r");

    int bytes_read = getdelim (&data, &size, EOF, fp);
    fclose(fp);
    printf("%d\n",bytes_read);
    printf("%s\n",data);
    
}



int main(){
    FILE *f;
    char data;
    char line[512];
    char name[2000];
    char fdata[2000];
    // printf("enter name: %s\n",name);
    // gets(name);
    // f= fopen("voters_list.txt", "r");



    // if(check_in_file("voters_list.txt",name,"43201-7448574-9")==1) printf("Found\n");
    // else printf("Not Found\n");
    
    display_Candidates("candidates_list.txt",fdata);
    

}