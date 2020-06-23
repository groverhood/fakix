/**
 *  tools/ethboot/main.c -- Kaijiboot over ethernet
 **/

#include <linux/if_packet.h>
#include <linux/ethtool.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stddef.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <fcntl.h>
#include <stdio.h>

#include "bootstruct.h"

#define verbose_printf(format, ...) if (mode_verbose) { printf(format, ## __VA_ARGS__); } 

static bool mode_verbose = false;

/* ethboot [-v] [-i <image file or device>] <ethernet device file> */
int main(int argc, char *argv[])
{
    static size_t machid_table[BOOTMAXMACHID + 1];
    
    int ind;
    int opt;
    char *ethdev;
    char *image = NULL;
    
    static struct option longopts[] = {
        {"verbose", no_argument, NULL, 'v'},
        {"image", required_argument, NULL, 'i'},
        {NULL, 0, 0, 0}
    };

    while (true) {
        opt = getopt_long(argc, argv, "vi:", longopts, NULL);
        switch (opt) {
        case 'v':
            mode_verbose = true;
        break;
        case 'i':
            image = optarg;
        break;
        case '?':
            printf("bad argument \"%s\"\n", optarg);
            return EXIT_FAILURE;
        }
        
        if (opt == -1) {
            break;
        }
    }
    
    if (optind >= argc) {
        printf("no device file provided\n");
        return EXIT_FAILURE;
    }

    ethdev = argv[optind];
    verbose_printf("opening socket\n");
    
    int dev = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (dev < 0) {
        perror("socket: ");
        return EXIT_FAILURE;
    }

    struct ifreq ifidx, ifmac;
    memset(&ifidx, 0, sizeof ifidx);
    strncpy(ifidx.ifr_name, ethdev, IFNAMSIZ);
    if (ioctl(dev, SIOCGIFINDEX, &ifidx) < 0) {
        perror("ioctl(SIOCGIFINDEX): ");
    }

    memset(&ifmac, 0, sizeof ifmac);
    strncpy(ifmac.ifr_name, ethdev, IFNAMSIZ);
    if (ioctl(dev, SIOCGIFHWADDR, &ifmac) < 0) {
        perror("ioctl(SIOCFGIHWADDR): ");
    }
    
    struct bootstruct bootstub;
    struct bootstruct *bootstruct;
    struct ether_header *ethhdr;
    bootword_t magic;    
    
    verbose_printf("waiting for device... ");
    struct sockaddr addr;
    socklen_t addr_len;
    if (recvfrom(dev, &bootstub, sizeof bootstub, 0, &addr, &addr_len) < 0) {
        perror("recvfrom: ");
        return EXIT_FAILURE;
    }
    
    if (bootstub.magic == BOOTMAGIC) {
        verbose_printf("found device!\n");
    } else {
        verbose_printf("\n");
        printf("device sent invalid magic\n");
        return EXIT_FAILURE;
    }

    int imagefd = -1;
    if (image != NULL) {
        imagefd = open(image, O_RDWR);
        verbose_printf("opening image file\n");
        if (imagefd < 0) {
            perror("open: ");
            return EXIT_FAILURE;
        }

        struct stat stat;
        verbose_printf("statting image file\n");
        if (fstat(imagefd, &stat)) {
            perror("stat: ");
            return EXIT_FAILURE;
        }

        verbose_printf("recording image file size\n");
        bootstub.size = stat.st_size;
    } else {
        bootstub.size = 0;
    }
    
    verbose_printf("allocating boot packet\n");
    void *packet = calloc(1, sizeof *bootstruct + bootstub.size + sizeof *ethhdr);
    if (packet == NULL) {
        printf("failed to allocate boot packet\n");
    }

    verbose_printf("filling ethernet header");
    ethhdr = packet;
    memcpy(ethhdr->ether_shost, &ifmac.ifr_hwaddr.sa_data, sizeof ethhdr->ether_shost);
    memset(ethhdr->ether_dhost, 0, sizeof ethhdr->ether_dhost);
    ethhdr->ether_type = htons(ETH_P_IP);
    
    bootstruct = (struct bootstruct *)(ethhdr + 1);
    verbose_printf("copying stub into bootstruct");
    memcpy(bootstruct, &bootstub, sizeof bootstub);
    verbose_printf("reading image into bootstruct, if provided");
    if (image != NULL && read(imagefd, bootstruct->ramdisk, bootstub.size) < 0) {
        perror("read: ");
        return EXIT_FAILURE;
    }

    if (sendto(dev, packet, sizeof *bootstruct + bootstub.size + sizeof *ethhdr,
               MSG_DONTWAIT, &addr, addr_len) < 0) {
        perror("sendto: ");
        return EXIT_FAILURE;
    }

    close(dev);

    return EXIT_SUCCESS;
}
