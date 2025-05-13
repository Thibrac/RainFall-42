#include <unistd.h>
#include <cstring>

#include <cstring>

class N {
    public :
        int nb;
        int (N::*func)(N &);
        char annotation[];
        N(int val)
        {
            nb = val;
        }
        int operator+(N& n){
            return(nb +n.nb);
        }
        int operator-(N& n){
            return(nb -n.nb);
        }
        void setAnnotation(char *val)
        {
            size_t n = strlen(val);
            memcpy(annotation, val, n);
            return ;
        }

};


int main(int ac, char**av)
{
    if (ac <= 1)
        _exit(1);
    N *first = new N(5);
    N *second = new N(6);
    first->setAnnotation(av[1]);
    return (second->*(second->func))(*first);
}

