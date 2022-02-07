#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>

#include "types.h"
#include "sock.h"

int d=0,g=0;
char oper[1024],key[1024],value[1024],tmps[1024],a[1024];
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;

typedef struct s
{
    char key[1024];
    char value[1024];
    struct s* next;
} db;
db* head=NULL;

void insert(void)
{
    db *temp=malloc(sizeof(db));
    strcpy(temp->key,key);
    strcpy(temp->value,value);
    temp->next=NULL;

    if(head==NULL)
    {
        head=temp;
    }
    else
    {
        db *tmp=head;
        while(tmp->next!=NULL)  tmp=tmp->next;
        tmp->next=temp;
    }
    //printf("%s->%s\n",head->key,head->value);
}

void printlist(void)
{
    db *temp=head;
    while(temp!=NULL)
    {
        printf("%s %s\n",temp->key,temp->value);
        temp=temp->next;
    }
}

int delete(void)
{
    db *temp=head,*pre=NULL;
    if(temp==NULL) return-1;
    while(temp!=NULL)
    {
        if(strcmp(temp->key,key)==0) break;
        pre=temp;
        temp=temp->next;
    }
    if(temp==NULL) return -1;

    if(temp==head)
    {
        head=temp->next;
        free(temp);
        temp=NULL;
        return 1;
    }
    else
    {
        pre->next=temp->next;
        free(temp);
        temp=NULL;
        return 1;
    }
    return -1;
}

int get(void)
{
    db *temp=head,*pre=NULL;
    if(temp==NULL) return-1;
    while(temp!=NULL)
    {
        if(strcmp(temp->key,key)==0) break;
        pre=temp;
        temp=temp->next;
    }
    if(temp==NULL) return -1;


    strcpy(tmps,temp->value);
    return 1;

}


void* serverthread(void *arg)
{
    int fd=*((int*)arg);
    free(arg);
    printf("[thread]created, serving client %d\n",fd);
    char input[1024];

    pthread_detach(pthread_self());
    while(1)
    {

        recv(fd,input,1024,0);
        pthread_mutex_lock(&lock);
//			printf("2");
        bzero(key,1024);
        bzero(value,1024);
        //	bzero(input,1024);
        bzero(oper,1024);
//			printf("3");
        sscanf(input,"%s %s %s",oper,key,value);
        //printf("%s=oper %s=key %s=value\n",oper,key,value);
        //send(fd,input,1024,0);
        if(strcmp(oper,"SET")==0)
        {
            insert();
            send(fd,"[ok] kvpair is set\n", 200, 0);
            printf("[ok] kvpair is set\n");
        }
        else if(strcmp(oper,"DELETE")==0)
        {
            d=delete();
            if(d>0)
            {
                send(fd,"[ok] kvpair is deleted\n", 200, 0);
                printf("[ok] kvpair is deleted\n");
            }
            else
            {
                send(fd,"[error] kvpair not found\n", 200, 0);
                printf("[error] kvpair not found\n");
            }
            //printlist();
        }
        else if(strcmp(oper,"GET")==0)
        {
//printf("4");
            g=get();
            strcpy(a,"[ok] the value is : ");
            if(g==1)
            {
                strcat(a,tmps);
                strcat(a,"\n");
                send(fd, a, 1024, 0);
                printf("%s",a);
            }
            if(g==-1)
            {
                send(fd, "[error] key not found\n", 200, 0);
                printf("[error] key not found\n");

            }
        }
        else
        {
            send(fd, "[error] invalid command\n",200,0);
            printf("[error] invalid command\n");

        }

        //printf("111111111111");
        //		bzero(oper,1024);
        //		bzero(key,1024);
        bzero(input,1024);

        pthread_mutex_unlock(&lock);
    }

    sleep(10);
    close(fd);
    return NULL;

}



int main(int argc, char **argv)
{
    char *server_port = 0;
    int opt = 0;
    /* Parsing args */
    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            server_port = malloc(strlen(optarg) + 1);
            strncpy(server_port, optarg, strlen(optarg));
            break;
        case '?':
            fprintf(stderr, "Unknown option \"-%c\"\n", isprint(optopt) ?
                    optopt : '#');
            return 0;
        }
    }

    if (!server_port)
    {
        fprintf(stderr, "Error! No port number provided!\n");
        exit(1);
    }

    /* Open a listen socket fd */
    int listenfd __attribute__((unused)) = open_listenfd(server_port);
    printf("listening ...\n");
    /* Start coding your server code here! */
    int *clientfd;
    char input[1024];

    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size = sizeof(serverStorage);
    pthread_t tid;
    //int i=0;
    while(1)
    {
        clientfd=malloc(sizeof(int));
        *clientfd= accept(listenfd, (struct sockaddr*)&serverStorage, &addr_size);
        if(clientfd<0) printf("error");
        pthread_create(&tid,NULL,&serverthread,clientfd);
        /*	if(i>=50)
            {
                for(i=0; i<50; ++i)
                {
                    pthread_join(tid[i],NULL);
                }
                i = 0;
            }
        */
    }

    close(listenfd);
    return 0;
}
