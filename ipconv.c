#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

int is_valid_input(char* ip)
{
    int regres;
    regex_t regex;

    if (!ip)
        return 1;

    regres = regcomp(&regex, 
            "^([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))."
            "([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))."
            "([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))."
            "([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))"
            "(/[0-9]+)?"
            "$", REG_EXTENDED);
    if (regres)
    {
        fprintf(stderr, "Could not compile...\n");
        return 1;
    }
    regres = regexec(&regex, ip, 0, NULL, 0);

    if( regres == REG_NOMATCH )
    {
        puts("Invalid IP address");
        return 2;
    }
    // last check: subnet is correct...
    char *p = strchr(ip, '/');
    int sn;
    if (p)
    {
        sn = atoi(++p);
        if (sn > 32)
        {
            fprintf(stderr, "WTF is this subnet? %d\n", sn);
            return 4;
        }
    }
    return 0;
}

uint32_t ipaddr_2_bits(char* ip)
{
    char* token;
    char orig[1024];
    uint32_t out = 0;

    if (!ip)
        return 0;

    strcpy(orig, ip);

    // First token
    token = strtok(orig, ".");
    out |= ((uint32_t)atoi(token) << 24);
    // Second token
    token = strtok(NULL, ".");
    out |= ((uint32_t)atoi(token) << 16);
    // Third token
    token = strtok(NULL, ".");
    out |= ((uint32_t)atoi(token) << 8);
    // Fourth token
    token = strtok(NULL, ".");
    out |= (uint32_t)atoi(token) ;

    return out;
}

int bits_2_ipaddr(uint32_t ipaddr_bits, char *ip)
{
    uint8_t first = ipaddr_bits >> 24;
    uint8_t second = ipaddr_bits >> 16;
    uint8_t third = ipaddr_bits >> 8;
    uint8_t last = ipaddr_bits;

    if (!ip)
        return 1;

    sprintf(ip, "%d.%d.%d.%d", first, second, third, last);
    strcat(ip, "\0");

    return 0;
}

void enter_test()
{
    srand(time(NULL));
    int i,j,k,l,m;
    char ip[1024];
    uint32_t num;
    char res[1024];
    for (i = 0; i < 100; ++i)
    {
        j = rand() % 255;
        k = rand() % 255;
        l = rand() % 255;
        m = rand() % 255;
        sprintf(ip, "%d.%d.%d.%d", j, k, l, m);
        // Got a random IP => convert it and back
        num = ipaddr_2_bits(ip);
        bits_2_ipaddr(num, res);
            // Print result
        if (strcmp(ip, res) == 0)
            printf("%-20s => 0x%-20x => %s\n", ip, num, res);
        else
            fprintf(stderr, "Wrong conversion!!!\n");
    }
}

int main(int argc, char* argv[])
{
    char ip[32];
    char result[1024];
    uint32_t nip;

    if (argc == 1)
    {
        // Run test mode
        enter_test();
    }
    else
    {
        strcpy(ip, argv[1]);
        if (is_valid_input(ip) != 0)
        {
            fprintf(stderr, "Wrong format\n");
            return 1;
        }

        nip = ipaddr_2_bits(ip);

        printf("Numeric: %u (%#x)\n", nip, nip);

        bits_2_ipaddr(nip, result);

        printf("Conversion: %s\n", result);
    }

    return 0;
}

