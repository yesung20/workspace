#include "Common.h"
#include <pthread.h>
#include <iostream>

#define SERVERPORT 9000 //서버 포트 번호를 나타내는 상수 정의
#define BUFSIZE    512  //버퍼의 크기를 나타내는 상수 정의

typedef struct{
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
} thread_arg;

void* threadFunction(void* arg){
	thread_arg* args = (thread_arg *)arg;
	int client_sock = args->client_sock;
	struct sockaddr_in* clientaddr = &(args->clientaddr);

	// 접속한 클라이언트 정보 출력
	char addr[INET_ADDRSTRLEN];     
	inet_ntop(AF_INET, &clientaddr->sin_addr, addr, sizeof(addr));   
	//클라이언트의 IP 주소를 네트워크 주소에서 표준 IP 주소로 변환하여, addr 배열에 저장함
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",  //변환된 IP 주소와 클라이언트의 포트 번호를 출력함
		addr, ntohs(clientaddr->sin_port));

	// 클라이언트와 데이터 통신
	while (1) {
		int retval;
		char buf[BUFSIZE + 1];
		// 데이터 받기
		retval = recv(client_sock, buf, BUFSIZE, 0); //클라이언트로부터 데이터를 수신
		if (retval == SOCKET_ERROR) {
			err_display("recv()");  // 수신 중 오류가 발생한 경우 오류 메시지 출력
			break;
		}
		else if (retval == 0)  //클라이언트가 연결을 종료한 경우 반복문 종료
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';  //받은 데이터를 문자열로 만들기 위해 끝에 NULL 문자 추가
		printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr->sin_port), buf);  //받은 데이터와 클라이언트의 정보를 출력

		// 데이터 보내기
		retval = send(client_sock, buf, retval, 0);  // 받은 데이터를 클라이언트에게 다시 송신
		if (retval == SOCKET_ERROR) {   //송신 중 오류가 발생한 경우 오류 메시지 출력
			err_display("send()");
			break;
		}
	}

		// 소켓 닫기
		close(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr->sin_port));
		pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	pthread_t thread1;

	int retval;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);   
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");  
	
	// bind()
    struct sockaddr_in serveraddr;      
	memset(&serveraddr, 0, sizeof(serveraddr));     
	serveraddr.sin_family = AF_INET;    
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);     
	serveraddr.sin_port = htons(SERVERPORT);    

	retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));  
	if (retval == SOCKET_ERROR) err_quit("bind()"); 

	thread_arg arg;

	socklen_t addrlen;

	// listen()
	while(1){
		//listen()
		retval = listen(listen_sock, SOMAXCONN);    
		if (retval == SOCKET_ERROR) err_quit("listen()");  

		//accept()
		addrlen = sizeof(arg.clientaddr);   
		arg.client_sock = accept(listen_sock, (struct sockaddr *)&arg.clientaddr, &addrlen); 
		if (arg.client_sock == INVALID_SOCKET) {    
			err_display("accept()");
			break;
		}

		//스레드 생성 후 인자로 값 전달
		if (pthread_create(&thread1, NULL, threadFunction, (void *)&arg) != 0) {
			perror("pthread_create");
			return 1;
		}
    }

	pthread_join(thread1, NULL);

	// 소켓 닫기
	close(listen_sock);
	return 0;
}
