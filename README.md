# socketProgrammingStudy
Study about Socket Programming

## Work Flow about Socket Programming
#### Server Socket
- Socket 생성
- bind(IP, Port등을 설정)
- listen
- accept

#### Client Socket
- Socket 생성
- connect

## Socket Protocol
#### Socket 생성
**int socket(int domain, int type, int protocol);**<br>
socket Parameters
- domain: 소켓이 사용할 프로토콜 체계(Protcol Family)정보 전달
- type: 소켓의 데이터 전송방식에 대한 정보 전달
- protocol: 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달

#### 프로토콜 체계(Protocol Family)
- PF_INET: IPv4 인터넷 프로토콜 체계
- PF_INET6: IPv6 인터넷 프로토콜 체계
- PF_LOCAL: 로컬 통신을 위한 UNIX 프로토콜 체계
- PF_PACKET: Low Level 소켓을 위한 프로토콜 체계
- PF_IPX: IPX노벨 프로토콜 체계

#### 소켓의 타입(Type)
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

#### 프로토콜의 최종선택
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

