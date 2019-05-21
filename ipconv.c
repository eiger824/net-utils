#include <stdio.h>
#include <regex.h>
#include <stdint.h>
#include <string.h>

int is_valid_ip(char* ip)
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

uint32_t ip_to_numeric(char* ip)
{
    char* token;
    uint32_t current = 0;;
    uint32_t out = 0;

    if (!ip)
        return 0;

    // First token
    token = strtok(ip, ".");
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

int number_to_ip(uint32_t number, char* out)
{
    if (!out)
        return 1;

    //TODO: implement
    return 0;
}

int main(int argc, char* argv[])
{
    char ip[32];
    uint32_t nip;

    if (argc == 1)
    {
        printf("Enter ip/range: ");
        scanf("%s", ip);
    }
    else
    {
        strcpy(ip, argv[1]);
    }
    
    if (is_valid_ip(ip) != 0)
    {
        fprintf(stderr, "Wrong format\n");
        return 1;
    }

    nip = ip_to_numeric(ip);

    printf("Numeric: %u (%#x)\n", nip, nip);

    return 0;
}

