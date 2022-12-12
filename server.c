#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
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
//display_Candidates function returns array of candidate list
void display_Candidates(const char* file,char *data){
    FILE *fp;
    long size =512;
    char *string;
    char temp[2000];
    fp=fopen(file,"r");

    int bytes_read = getdelim (&data, &size, EOF, fp);
    fclose(fp);
    // printf("%d\n",bytes_read);
    // printf("%s\n",data);
    
}
int output_to_file(const char * file,char * name,char *cnic,char* symbol){
    FILE * fp;
    if(fp<0)return -1;
    fp=fopen(file,"w+");
    fputs(name,fp);
    fputs(" ",fp);
    fputs(cnic,fp);
    fputs(" ",fp);
    fputs(symbol,fp);
    fputc('\n',fp);
    fclose(fp);
    return 0;

}
int cater_client(void * arg){
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000], NAME[2000],CNIC[2000],candidates[2000];
    FILE * fp;
    char buffer[2000];
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    
    // Listen for clients:

        if(listen(socket_desc, 1) < 0){
            printf("Error while listening\n");
            return -1;
        }
        printf("\nListening for incoming connections.....\n");

    
        // Accept an incoming connection:
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
        
        if (client_sock < 0){
            printf("Can't accept\n");
            return -1;
        }
        printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        // Receive client's message:
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        
        strcpy(NAME,client_message);
        NAME[strlen(NAME)]='\0';
        printf("Msg from client: %s\n", NAME);
        
        memset(client_message, 0, sizeof(client_message));
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        strcpy(CNIC,client_message);

        CNIC[strlen(CNIC)]='\0';

        printf("Msg from client: %s\n", CNIC);
        memset(client_message, 0, sizeof(client_message));
        
        strcpy(server_message,"Wait Processing\n");
        
        if (send(client_sock, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return -1;
        }
        // verify client in database/file 
        int check = check_in_file("voters_list.txt",NAME,CNIC);
        if (check==1){
            printf("Found\n");
            memset(server_message,'\0',sizeof(server_message));
            strcpy(server_message,"Welcome, Choose poll symbol to vote\n");
        } 
        else {
            printf("Not found\n");
            memset(server_message,'\0',sizeof(server_message));
            strcpy(server_message,"Cannot vote\n");
         

        }
        //send response to client
        
        if (send(client_sock, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return -1;
        }
        //display_Candidates
        if(check==1){
            display_Candidates("candidates_list.txt",candidates);
            memset(server_message, 0, sizeof(server_message));
            strcpy(server_message,candidates);
            if (send(client_sock, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return -1;
            }

        }
        else{
            memset(server_message, 0, sizeof(server_message));
            strcpy(server_message, "Closing Connection. . .");
            if (send(client_sock, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return -1;
            }
            close(client_sock);
        }
        //recieve symbol
        memset(client_message, 0, sizeof(client_message));
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        //record output into outfile
        if(client_message!=NULL){
            //do something with the message
            if(output_to_file("output_data.txt",NAME,CNIC,client_message)<0)perror("Error writing output to file\n");
            printf("Output Done\n");
        }
        // Closing the socket:
        close(client_sock);
    close(socket_desc);
}

int main(void)
{
    pthread_t t1,t2,t3,t4,t5;
    pthread_create(&t1, NULL,(void*)cater_client,NULL);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL,(void*)cater_client,NULL);
    pthread_join(t2, NULL);
    pthread_create(&t3, NULL,(void*)cater_client,NULL);
    pthread_join(t3, NULL);
    pthread_create(&t4, NULL,(void*)cater_client,NULL);
    pthread_join(t4, NULL);
    pthread_create(&t5, NULL,(void*)cater_client,NULL);
    pthread_join(t5, NULL);
    pthread_exit(NULL);



    return 0;
}