
/********************************************* C ***************************************************/
#include <stdio.h>

/*  ttcp option:
 *  -r
 *  -t
 *  -l
 *
*/
typedef struct option {
    int type;
    uint16_t port;
    bool transmit;
    bool receive;
    int length;
    int number;
}Options;

void receiver(Options *opt)
{
}

void sender(Options *opt)
{

}

bool parse_opt(int argc, char **argv, Options *opt)
{
    int i = 0;
    while (argc-- > )
    {
        ++i;    
    }
}

int main(int argc, char **argv)
{
    Options opt = {0};
    if (parse_opt(argc, argv, &opt))
    {
        if (opt.receive)
            receive(&opt);
        if (opt.transmit)
            transmit(&opt);
    }

    return 0;
}
