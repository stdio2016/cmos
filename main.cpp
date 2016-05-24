#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include <vector>

using namespace std;

string in;
int getLine()
{
    stringstream a;
    int ch;
    ch=getchar();
    while(ch!='\n'&& ch!=EOF)
    {
        a.put(ch);
        ch=getchar();
    }
    in=a.str();
    if(ch==EOF) return 001;
    return 000;
}

int totalId=0;

class wire;

struct drive
{
    int id;
    drive()
    {
        id=++totalId;
    }
    virtual const char *name() const{
        return "drive";
    }
};

ostream &operator<<(ostream &ooo,const drive *a)
{
    ooo << a->name() << ' ' << a->id << '\n';
    return ooo;
}

struct mos: public drive
{
    wire *source,*gate,*drain;
};

struct vdd: public drive
{
    virtual const char *name() const{
        return "VDD";
    }
};

struct gnd: public drive
{
    virtual const char *name() const{
        return "GND";
    }
};

struct port: public drive
{
    wire *connection;
};

struct wire
{
    vector<drive*> connection;
    int id;
    wire()
    {
        id=++totalId;
    }
    virtual const char *name() const{
        return "wire";
    }
};

ostream &operator<<(ostream &ooo,const mos *a)
{
    ooo << a->name() << ' ' << a->id << " connect to "
        << a->source->id << ", "
        << a->gate->id << ", "
        << a->drain->id << '\n';
    return ooo;
}

ostream &operator<<(ostream &ooo,const port *a)
{
    ooo << a->name() << ' ' << a->id << " connect to " << a->connection->id;
    return ooo;
}

ostream &operator<<(ostream &ooo,const wire *a)
{
    ooo << a->name() << ' ' << a->id << " connect to ";
    if(a->connection.size()>0)
        ooo << a->connection[0]->id;
    for(unsigned int i=1;i<a->connection.size();i++)
    {
        ooo << ", " << a->connection[i]->id;
    }
    ooo << '\n';
    return ooo;
}

struct module
{
    vector<mos> moses;
    vector<wire> wires;
    vector<port> ports;
    vdd high;
    gnd low;
    string name;
};

int main()
{
    module abc;
    int mode=0;
    int y=getLine();
    unsigned int modwidth;
    while(y==0)
    {
        if(in.length()>0)
        {
            if(in[0]=='{' || (mode==2 && in[0]=='C'))
            {
                if(mode==2 && in.length()>=modwidth)
                {
                    char ch=in[modwidth-1];
                    if(ch=='}'||ch=='C')
                    {

                    }
                    else
                    {
                        cout << "Syntax error!";
                        return 123;
                    }
                }
                else
                {
                    cout << "Syntax error!";
                    return 123;
                }
            }
            else if(in[0]=='-')
            {
                if(mode==1||mode==2){
                    unsigned int i;
                    for(i=0;i<in.length();i++)
                    {
                        if(in[i]=='-'||in[i]=='C')
                        {

                        }
                        else
                        {
                            break;
                        }
                    }
                    if(mode==2)
                    {
                        if(in.length()>=modwidth)
                            mode=0;
                        else
                        {
                            cout << "Syntax error!";
                            return 123;
                        }
                    }
                    else
                        mode=2;
                    modwidth=i;
                }
                else
                {
                    cout << "Syntax error!";
                    return 123;
                }
            }
            else if(in[0]!=';')
            {
                if(mode==0){
                    abc.name=in;
                    mode=1;
                }
                else
                {
                    cout << "Syntax error!";
                    return 123;
                }
            }
        }
        else
        {
            if(mode==0)
            {
                mode=0;
            }
            else
            {
                cout << "Syntax error!";
                return 123;
            }
        }
        y=getLine();
    }
    return 0;
}

/*
; declare a module not
not
------- this is a comment
{   $ }  $ means supply (1)
{   o }  o means wire to connect source/drain
{ ==P }  P means PMOS
C== ooC  C means input/output
{ ==N }  N means NMOS
{   o }  = means wire to connect gate
{   # }  # means ground (0)
------- end of not

; this is also a comment

*/
