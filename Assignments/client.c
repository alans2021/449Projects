#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BLOCK_SIZE 1024

FILE *f; 
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER; //Mutex for the shared file resource
struct thread_args{ //Struct storing the arguments that goes to the function
	int chunk; //"index" of the thread
	int num; //total number of threads
	int port; //Specifies port number
	char * ip; //IP address
};

void *fileThreading(void * ptr); //Function that does the file IO and server-client handshakes 

int main(int argc, char ** argv){
	int numports = argc / 2;
	unsigned short ports[numports]; //Stores ports provided to command line
	char * ipAdr[numports]; //Stores IP addresses provided to command line
	struct thread_args Structthread[numports]; //Stores struct arguments
	pthread_t threads[numports]; //Array of pthreads

	if(argc < 2) {
		printf("Must provide port numbers");
		return -1;
	}
	
	if(argc % 2 == 0){
		printf("Must have port and ip address pairs");
		return -1;
	}

	for(int i = 0; i < numports; i++){
		ports[i] = atoi(argv[(i + 1) * 2]);
		ipAdr[i] = argv[2 * i + 1];
	}
	
	f = fopen("output.txt", "w"); //Open file
	if(f == NULL){
		printf("Error opening file\n");
		exit(1);
	}

	for(int i = 0; i < numports; i++){ //Provide argument values for each Structthread
		Structthread[i].chunk = i;
		Structthread[i].num = numports;	
		Structthread[i].port = ports[i];
		Structthread[i].ip = ipAdr[i];
	}
	for(int i = 0; i < numports; i++) //Create thread with appropriate arguments
		pthread_create(&threads[i], NULL, fileThreading, (void *) &(Structthread[i]));

	for(int i = 0; i < numports; i++) //Ensure all threads complete execution before closing file
		pthread_join(threads[i], NULL);
	fclose(f);
	return 0;
}

void * fileThreading(void * ptr){
	char buffer[1024]; //String that will store file contents sent by server
	struct sockaddr_in serv_addr;
	struct thread_args *args = (struct thread_args *)ptr; //Get the arguments from the struct

	int con_fd = socket(PF_INET, SOCK_STREAM, 0); //Create connection
	if(con_fd == -1){
		perror("Socket Error\n");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(struct sockaddr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(args->port);
	serv_addr.sin_addr.s_addr = inet_addr(args->ip);

	int ret = connect(con_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
	if(ret < 0){
		perror("Connect error\n");
		exit(1);
	}

	char msg[1]; //Chunk will be stored here
	int recret; 
	int chunk = args->chunk;
	int num = args->num;
	
	while(1){
		sprintf(msg, "%d", chunk); //Convert chunk int to msg string
		int i = send(con_fd, msg, strlen(msg), 0); //Send chunk index to server
		recret = recv(con_fd, buffer, BLOCK_SIZE, 0); //Receive file contents from server
		
		pthread_mutex_lock(&fileMutex); //Place mutex on file
		fseek(f, chunk * BLOCK_SIZE, SEEK_SET); //Go to correct offset of file
		fwrite(buffer, 1, recret, f); //Only print out number of bytes from buffer based on return value	
		pthread_mutex_unlock(&fileMutex);
		
		close(con_fd); //Close connection to socket
		chunk = chunk + num; //Increment chunk by num
		if(recret < 1024) //Means no more data to read
			break;

		memset(&serv_addr, 0, sizeof(struct sockaddr)); //Setting serv_addr attributes once again
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(args->port);
		serv_addr.sin_addr.s_addr = inet_addr(args->ip);

		con_fd = socket(PF_INET, SOCK_STREAM, 0); //Create new connection to socket
		if(con_fd == -1){
			perror("Socket Error\n");
			exit(1);
		}	

		ret = connect(con_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
		if(ret < 0){
			perror("Connect error\n");
			exit(1);
		}
	}
}
