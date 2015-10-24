#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

#include <netinet/ip.h>
#include <netinet/udp.h>

#include <linux/if_ether.h>
#include <linux/filter.h>

static struct sock_filter code[] = {
	{ 0x28,  0,  0, 0x0000000c },
	{ 0x15,  0, 45, 0x00000800 },
	{ 0x30,  0,  0, 0x00000017 },
	{ 0x15,  0, 43, 0x00000011 },
	{ 0x20,  0,  0, 0x0000001a },
	{ 0x15,  0, 41, 0xc0a80001 },
	{ 0x28,  0,  0, 0x00000014 },
	{ 0xb1,  0,  0, 0x0000000e },
	{ 0x48,  0,  0, 0x00000010 },
	{ 0x15,  0, 37, 0x000087f7 },
	{ 0x48,  0,  0, 0x0000000e },
	{ 0x15,  0, 35, 0x000043c8 },
	{ 0x48,  0,  0, 0x00000012 },
	{ 0x15,  0, 33, 0x00000028 },
	{ 0x40,  0,  0, 0x00000016 },
	{ 0x04,  0,  0, 0x15248511 },
	{ 0x14,  0,  0, 0x7657e746 },
	{ 0x15,  0, 29, 0000000000 },
	{ 0x40,  0,  0, 0x0000001a },
	{ 0x04,  0,  0, 0x12756448 },
	{ 0x14,  0,  0, 0x73ae9580 },
	{ 0x15,  0, 25, 0000000000 },
	{ 0x40,  0,  0, 0x0000001e },
	{ 0x04,  0,  0, 0x43885712 },
	{ 0x14,  0,  0, 0x77eabb47 },
	{ 0x15,  0, 21, 0000000000 },
	{ 0x40,  0,  0, 0x00000022 },
	{ 0x04,  0,  0, 0x97116482 },
	{ 0x14,  0,  0, 0xd047cab5 },
	{ 0x15,  0, 17, 0000000000 },
	{ 0x40,  0,  0, 0x00000026 },
	{ 0x04,  0,  0, 0x71558943 },
	{ 0x14,  0,  0, 0xa288bea9 },
	{ 0x15,  0, 13, 0000000000 },
	{ 0x40,  0,  0, 0x0000002a },
	{ 0x04,  0,  0, 0x21448653 },
	{ 0x14,  0,  0, 0x577dea88 },
	{ 0x15,  0,  9, 0000000000 },
	{ 0x40,  0,  0, 0x0000002e },
	{ 0x04,  0,  0, 0x00231920 },
	{ 0x14,  0,  0, 0x34565282 },
	{ 0x15,  0,  5, 0000000000 },
	{ 0x40,  0,  0, 0x00000032 },
	{ 0x04,  0,  0, 0x71349586 },
	{ 0x14,  0,  0, 0xa166cab7 },
	{ 0x15,  0,  1, 0000000000 },
	{ 0x06,  0,  0, 0x0000ffff },
	{ 0x06,  0,  0, 0000000000 },
};

static struct sock_fprog fprog = {
	.len = 48,
	.filter = code
};

#define BUFFER_SIZE 1024

static int create_bpf_socket(struct sock_fprog *fprog)
{
	int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock == -1) {
		perror("failed to create socket");
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, fprog, sizeof(*fprog)) == -1) {
		perror("failed to set socket option");
		return -1;
	}

	return sock;
}

int main(int argc, char **argv)
{
	uint8_t buffer[BUFFER_SIZE];
	int sock = create_bpf_socket(&fprog);
	if (sock == -1)
		return 1;
	fprintf(stderr, "[+] Transmission channel inited! Waiting for connections ...\n");
	memset(buffer, 0, BUFFER_SIZE);
	if (read(sock, buffer, BUFFER_SIZE) == -1)
		return 1;
	fprintf(stderr, "[+] Good job!\n");
	close(sock);
	return 0;
}
