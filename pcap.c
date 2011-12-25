#include <pcap.h>
#include <stdio.h>

int main()
{
char * device; /* �����������ݰ�������ӿڵ����� */
pcap_t * p; /* �������ݰ����������Ҫ�����ݽṹ */
struct bpf_program fcode; /* BPF ���˴���ṹ */

char errbuf[1024]={0};

/* ��һ�������ҿ��Բ������ݰ����豸 */
device = pcap_lookupdev(errbuf);

/* �ڶ�����������������׼�����в��� */
p = pcap_open_live(device, 8000, 1, 500, errbuf);

/* ������������û������˹��������������Ͱ�װ���˴��� */
pcap_compile(p, &fcode, filter_string, 0, netmask);
pcap_setfilter(p, &fcode);

/* ���Ĳ������루����ѭ���������������ݰ� */
for( ; ; )
{
while((ptr = (char *)(pcap_next(p, &hdr))) == NULL);
		
/* ���岽���Բ�������ݽ�������ת����ת������̫���ݰ����� */
eth = (struct libnet_ethernet_hdr *)ptr;

/* ������������̫ͷ�����з������ж������������ݰ����ͣ�����һ���Ĵ��� */
if(eth->ether_type == ntohs(ETHERTYPE_IP)) 
{
;
}
if(eth->ether_type == ntohs(ETHERTYPE_ARP)) 
{
;
}
	
/* ���һ�����رղ�����,һ���򵥼������ڳ����ʼ��ʱ�����źŴ�������
�Ա��ڳ����˳�ǰִ�б������� */
pcap_close(p);

return 0;
}
