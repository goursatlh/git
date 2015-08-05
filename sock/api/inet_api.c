#include <iostream>
#include <arpa/inet.h>

using namespace std;

int main()
{
	char addr[] = "192.168.1.1";
	in_addr addr_b;
	unsigned short buf[32] = {0};
	int ret = 0;
	
	cout<<"test inet_addr"<<endl;
	cout<<addr<<endl;
	cout<<hex<<inet_addr(addr);

	ret = inet_aton(addr, &addr_b);
	cout<<ret<<endl;
	cout<<"test inet_aton"<<endl;
	cout<<hex<<addr_b.s_addr<<endl;

	cout<<"test inet_ntoa"<<endl;
	cout<<inet_ntoa(addr_b)<<endl;

	cout<<"test inet_pton"<<endl;
	inet_pton(AF_INET6, "fe80::1", buf);
	cout<<buf[0]<<" "<<buf[1]<<endl;
	return 0;

}



