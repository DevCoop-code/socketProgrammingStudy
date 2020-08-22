# socketProgrammingStudy
Study about Socket Programming

## Work Flow about Socket Programming
### Server Socket
- Socket 생성
- bind(IP, Port등을 설정)
- listen
- accept

### Client Socket
- Socket 생성
- connect

## Socket Protocol
### Socket 생성
**int socket(int domain, int type, int protocol);**<br>
socket Parameters
- domain: 소켓이 사용할 프로토콜 체계(Protcol Family)정보 전달
- type: 소켓의 데이터 전송방식에 대한 정보 전달
- protocol: 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달

### 프로토콜 체계(Protocol Family)
- PF_INET: IPv4 인터넷 프로토콜 체계
- PF_INET6: IPv6 인터넷 프로토콜 체계
- PF_LOCAL: 로컬 통신을 위한 UNIX 프로토콜 체계
- PF_PACKET: Low Level 소켓을 위한 프로토콜 체계
- PF_IPX: IPX노벨 프로토콜 체계

### 소켓의 타입(Type)
소켓의 데이터 전송방식 
1. 연결지향형 소켓(SOCK_STREAM)
Socket 함수의 두번째 인자로 SOCK_STREAM을 전달하면 연결지향형 소켓이 생성
   - 중간에 데이터가 소멸되지 않고 목적지로 전송
   - 전송 순서대로 데이터가 수신
   - 전송되는 데이터의 경계가 존재하지 않음
   - 연결 지향형 소켓은 다른 연결지향형 소켓 하나와만 연결 가능

2. 비 연결지향형 소켓(SOCK_DGRAM)
Socket 함수의 두번째 인자로 SOCK_DGRAM을 전달하면 비 연결지향형 소켓이 생성
   - 전송된 순서에 상관없이 가장 빠른 전송을 지향
   - 전송된 데이터는 손실의 우려가 있ㅅ고, 파손의 우려가 존재
   - 전송되는 데이터의 경계가 존재
   - 한번에 전송할 수 있는 데이터의 크기가 제한

### 프로토콜의 최종선택
최종적으로 소켓이 사용하게 될 프로토콜 정보를 전달하는 목적으로 존재, 즉, 소켓의 데이터 전송방식은 같지만 그 안에서도 프로토콜이 나뉘는 상황이 존재할 수 있어 프로토콜 정보를 조금 더 구체화함 <br>
Ex]<br> 
TCP 소켓: socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); <br>
UDP 소켓: socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); 

## 주소정보의 표현
```
struct sockaddr_in
{
    sa_family_t     sin_family; // 주소체계(Address Family)
    uint16_t        sin_port;   // 16비트 TCP/UDP PORT 번호
    struct in_addr  sin_addr;   // 32비트 IP주소
    char            sin_zero[8] // 사용되지 않음
}
```
```
struct in_addr
{
    in_addr_t   s_addr; //32bit IPv4 인터넷 주소
}
```

### 구조체 sockaddr_in의 멤버에 대한 분석
- 멤버 sin_family
  - AF_INET: IPv4 인터넷 프로토콜에 적용하는 주소체계
  - AF_INET6: IPv6 인터넷 프로토콜에 적용하는 주소 체계
  - AF_LOCAL: 로컬 통신을 위한 유닉스 프로토콜의 주소 체계
- 멤버 sin_port
  - 16bit PORT 번호를 저장, 단 '네트워크 바이트 순서'로 저장해야 함
- 멤버 sin_addr
  - 32bit IP 주솟정보를 저장, 단 '네트워크 바이트 순서'로 저장해야 함
- 멤버 sin_zero
  - 특별한 의미를 지니지 않는 멤버, 단순히 구조체 sockaddr_in의 크기를 구조체 sockaddr와 일칫시키기 위해 삽입된 멤버 But, 반드시 0으로 채워야 함

## 네트워크 바이트 순서와 인터넷 주소 변환
CPU에 따라 4바이트 정수 1을 메모리 공간에 저장하는 방식이 달라질 수 있음. 이러한 부분을 고려하지 않고서 데이터를 송수신하면 문제가 발생할 수 있음

### 바이트 순서(Order)와 네트워크 바이트 순서
CPU가 데이터를 메모리에 저장하는 방식은 다음과 같이 2가지로 나뉨
- 빅 엔디안(Big Endian): 상위 바이트의 값을 작은 번지수에 저장
- 리틀 엔디안(Little Endian): 상위 바이트의 값을 큰 번지수에 저장

![byteOrdering](images/byteOrdering.png)

데이터 저장 방식은 CPU마다 다름 그래서 CPU의 데이터 저장방식을 의미하는 호스트 바이트 순서(Host Byte Order)는 CPU에 따라 차이가 남(ex] Intel계열 CPU는 리틀 엔디안 방식으로 데이터를 저장)

데이터를 저장하는 방식의 문제점 때문에 네트워크를 통해 데이터를 전송할 때는 통일된 기준으로 데이터를 전송하기로 약속했으면 이 약속을 가리켜 네트워크 바이트 순서(Network Byte Order)라 함. 네트워크 바이트 순서의 약속은 **빅 엔디안 방식으로의 통일**, 즉 네트워크 상으로 데이터를 전송할 때는 데이터의 배열을 빅 엔디안 기준으로 변경해서 송수신 해야함

### 바이트 순서의 변환(Endian Conversions)
sockaddr_in 구조체 변수에 값을 채우기 앞서 네트워크 바이트 순서로 변환해서 저장해야 함
```
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long htonl(unsigned long);
unsigned long ntohl(unsigned long);
```
h: h는 호스트(host) 바이트 순서를 의미 <br>
n: n은 네트워크(network) 바이트 순서를 의미 <br>
s: short, l: long 을 의미 <br>

ex] <br>
htons: short형 데이터를 호스트 바이트 순서에서 네트워크 바이트 순서로 변환<br>
ntohs: short형 데이터를 네트워크 바이트 순서에서 호스트 바이트 순서로 변환

### 인터넷 주소의 초기화와 할당
inet_aton 함수도 기능상으로는 inet_addr 함수와 동일. 즉, 문자열 형태의 IP주소를 32비트 정수, 그것도 네트워크 바이트 순서로 정렬해서 반환
```
int inet_aton(const char* string, struct in_addr* addr)
```
- Parameter <br>
string: 변환할 IP주소 정보를 담고 있는 문자열의 주소 값 전달 <br>
addr: 변환된 정보를 저장할 in_addr 구조체 변수의 주소 값 전달 <br>
- Return <br>
1: Success / 0: Fail

```
char* inet_ntoa(struct in_addr adr);
```
inet_aton과 반대로 작동하는 함수로써  인자로 전달된 정수형태의 IP정보를 참조하여 문자열 형태의 IP정보로 변환함. 변환된 문자열의 주소 값을 반환
- Return <br>
성공 시 변환된 문자열의 주소 값, 실패 시 -1 반환

## TCP기반 서버/클라이언트
### 연결요청 대기상태로의 진입
```
int listen(int sock, int backlog);
```
- Parameter <br>
sock: 연결요청 대깃상태에 두고자 하는 소켓의 file descriptor <br>
backlog: 연결요청 대기 큐(Queue)의 크기정보 전달, 5가 전달되면 큐의 크기가 5가 되어 클라이언트의 연결요청을 5개까지 대기시킬 수 있음
- Return <br>
0: Success / -1: Fail

### 클라이언트의 연결요청 수락
```
int accept(int sock, struct sockaddr* addr, socklen_t* addrlen);
```
- Parameter <br>
sock: 서버 소켓의 file descriptor <br>
addr: 연결요청 한 클라이언트의 주소정보를 담을 변수의 주소 값 전달, 함수 호출이 완료되면 인자로 전달된 주소의 변수에는 클라이언트의 주소정보가 채워짐
addrlen: 두 번째 매개변수 addr에 전달된 주소의 변수 크기를 바이트 단위로 전달
- Return <br>
socket의 file descriptor number: Success / -1: Fail

accept 함수는 연결요청 대기 큐에서 대기중인 클라이언트의 연결요청을 수락하는 기능의 함수. accept 함수는 호출성공 시 내부적으로 데이터 입출력에 사용할 소켓을 생성하고, 그 소켓의 file desriptor를 반환함

### TCP 클라이언트 기본 함수
```
int connect(int sock, struct sockaddr* servaddr, socklen_t addrlen);
```
- Parameter <br>
sock: 클라이언트 소켓의 파일 디스크립터 전달 <br>
servaddr: 연결요청 할 서버의 주소정보를 담은 변수의 주소 값 전달 <br>
addrlen: 두 번째 매개변수 servaddr에 전달된 주소의 변수 크기를 바이트 단위로 전달 <br>
- Return <br>
0: Success / -1: Fail

<img src="images/tcpWorkflow.png" width="50%">