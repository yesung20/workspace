#include "Common.h"
#include <thread>
#include <iostream>
char *SERVERIP = (char *)"127.0.0.1"; //서버 IP주소를 나타내는 포인트 변수를 선언. 기본값으로 127.0.0.1 사용

#define SERVERPORT 9000     //서버 포트 번호를 나타내는 상수 정의
#define BUFSIZE    512      //버퍼의 크기를 나타내는 상수 정의

void receive_data(int sock){
	char buf[BUFSIZE + 1];
    int retval;

	while(1){
		printf("waitting data ...\n");
		retval = recv(sock, buf, 512, 0);  //함수를 호출하여 소켓으로부터 데이터를 수신
		printf("recv peek data len=%d\n", retval);
		//retval = recv(sock, buf, retval, MSG_WAITALL);  //함수를 호출하여 소켓으로부터 데이터를 수신
		//printf("recv data len=%d\n", retval);
		if (retval == SOCKET_ERROR) {   //소켓 통신에 오류 발생시 오류 메시지 출력 및 루프 종료
			err_display("recv()");
			break;
		}
		else if (retval == 0)   //상대방이 연결을 종료했을때 루프 종료
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[받은 데이터] %s\n", buf);
	}
	pthread_exit(NULL);

}
int main(int argc, char *argv[]){

	int retval;

    //프로그램을 실행할 때 서버 IP주소를 지정하지 않으면 프로그램에서 정의된 기본 IP주소(SERVERIP)가 사용되고
    //서버 IP 주소를 지정하면 그 값이 SERVERIP 변수에 설정된다
	if (argc > 1) SERVERIP = argv[1];

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);    //TCP 소켓 생성
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;  //소켓 주소 정보를 저장하기 위한 구조체
	memset(&serveraddr, 0, sizeof(serveraddr));     //구조체를 0으로 초기화 
    //(초기화 하는 이유는 구조체의 일부 멤버만 설정하고 나머지는 기본값으로 남겨두는 경우가 많아 오류 발생 가능성이 있다.)

	serveraddr.sin_family = AF_INET;    //주소 체계 지정
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);   //문자열 형태의 SERVERIP를 IPv4주소로 변환하여 serceraddr.sin_addr에 할당
    //소켓 연결을 위한 서버의 IP주소 설정
    //serveraddr.sin_addr.s_addr = inet_addr(SERVERIP); 이렇게 수동으로 해도 됨
    /*
    구조체 초기화 시에 바로 하는 것도 가능
    struct sockaddr_in serveraddr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVERPORT),
        .sin_addr.s_addr = inet_addr(SERVERIP)
    };
    */

	serveraddr.sin_port = htons(SERVERPORT);    //서버 포트 번호를 네트워크 바이트 순서로 설정
	retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));     //서버에 연결시도
	if (retval == SOCKET_ERROR) err_quit("connect()");  //소켓 연결이 성공적으로 이루어졌는지 검사

	std::thread receive_thread(receive_data, sock);

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	// 서버와 데이터 통신
	while (1) {
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)     //데이터를 읽어봐서 buf에 저장. 입력 도중 오류가 발생했을 때 루프 종료
			break;

		// 사용자가 입력한 데이터에서 개행 문자를 제거 '\n'
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = send(sock, buf, (int)strlen(buf), 0);   //send()함수를 사용하여 소켓을 통해 데이터를 보낸다.
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		/*
		retval = recv(sock, buf, retval, MSG_WAITALL);  //함수를 호출하여 소켓으로부터 데이터를 수신
		if (retval == SOCKET_ERROR) {   //소켓 통신에 오류 발생시 오류 메시지 출력 및 루프 종료
			err_display("recv()");
			break;
		}
		else if (retval == 0)   //상대방이 연결을 종료했을때 루프 종료
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[받은 데이터] %s\n", buf);
		*/

	}

	//receive_thread.join();

	// 소켓 닫기
	close(sock);
	return 0;
}
