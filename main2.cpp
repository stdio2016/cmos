#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

vector<string> board;

char testdata[][100]={
    "-------",
    "{   $ }",
    "{   o }",
    "{ ==P }",
    "C== ooC",
    "{ ==N }",
    "{   o }",
    "{   # }",
    "-------",
    ""};

void convertToVecStr()
{
    int i;
    for(i=0;testdata[i][0]!=0;i++)
    {
        board.push_back(testdata[i]);
    }
}

class SyntaxError: public runtime_error
{
public:
    SyntaxError(): runtime_error("I give up!"){}
};

int objCount=0;

struct wire
{
    int id;
    wire()
    {
        id=++objCount;
    }
    // vector<drive * > connect;
};

class module
{
public:
    module()
    {
        routee=routeo=0;
    }
    void parse(vector<string> &board)
    {
        height=board.size();
        width=board[0].size();
        routee=new wire*[height*width];
        for(int i=0;i<height*width;i++) routee[i]=0;
        routeo=new wire*[height*width];
        for(int i=0;i<height*width;i++) routeo[i]=0;
        for(int i=1;i<height-1;i++)
        {
            for(int j=1;j<width-1;j++)
            {
                wireProcessor(i,j);
            }
        }
    }
    void printModule()
    {
        for(int i=1;i<height-1;i++)
        {
            for(int j=1;j<width-1;j++)
            {
                cout.width(2);
                if(routee[posof(i,j)])
                    cout << routee[posof(i,j)]->id;
                else
                    cout << "  ";
                cout<<',';
                cout.width(2);
                if(routeo[posof(i,j)])
                    cout << routeo[posof(i,j)]->id;
                else
                    cout << "  ";
                cout<<'|';
            }
            cout << '\n';
        }
    }
    ~module()
    {
        delete[]routee;
        delete[]routeo;
        for(unsigned int i=0 ; i!=wires.size() ; ++i)
        {
            delete wires[i];
        }
    }
private:
    vector<wire*> wires;
    wire **routeo;
    wire **routee;
    int height;
    int width;
    int posof(int i,int j){return i*width+j;}
    void wireProcessor(int i,int j)
    {
        wire *a;
        switch(board[i][j])
        {
            case 'o':
                if(routeo[posof(i,j)]!=0)
                    return;
                a=new wire();
                wires.push_back(a);
                wireProcessor2(i,j,'o',a,0);
                break;
            case '=':
                if(routee[posof(i,j)]!=0)
                    return;
                a=new wire();
                wires.push_back(a);
                wireProcessor2(i,j,'=',a,0);
                break;
            case 'P':
            case 'N':
            case '+':
                break;
            case '$':
            case '#':
            case '*':
                break;
            case ' ':
                break;
            case '[':
            case ']':
                break;
            default:
                throw SyntaxError();
        }
    }
    void wireProcessor2(int i,int j,char wireType,wire *a,int dir)
    {
        if(i<1 || i>=height-1 || j<1 || j>=width-1) return;
        switch(board[i][j])
        {
            case 'o':
                if(wireType=='=') return;
                if(routeo[posof(i,j)]!=0) return;
                routeo[posof(i,j)]=a;
                wireProcessor2(i+1,j,'o',a,1);
                wireProcessor2(i-1,j,'o',a,2);
                wireProcessor2(i,j+1,'o',a,3);
                wireProcessor2(i,j-1,'o',a,4);
                break;
            case '=':
                if(wireType=='o') return;
                if(routee[posof(i,j)]!=0) return;
                routee[posof(i,j)]=a;
                wireProcessor2(i+1,j,'=',a,1);
                wireProcessor2(i-1,j,'=',a,2);
                wireProcessor2(i,j+1,'=',a,3);
                wireProcessor2(i,j-1,'=',a,4);
                break;
            case 'P':
            case 'N':
                if(wireType=='o')
                    break;
                wireType='=';
                // fall through
            case '+':
                if(wireType!='o')
                    routee[posof(i,j)]=a;
                if(wireType!='=')
                    routeo[posof(i,j)]=a;
                switch(dir)
                {
                    case 1:
                        wireProcessor2(i+1,j,wireType,a,1);
                        break;
                    case 2:
                        wireProcessor2(i-1,j,wireType,a,2);
                        break;
                    case 3:
                        wireProcessor2(i,j+1,wireType,a,3);
                        break;
                    case 4:
                        wireProcessor2(i,j-1,wireType,a,4);
                        break;
                }
                break;
            case '$':
            case '#':
            case '*':
                if(routee[posof(i,j)]!=0) return;
                if(routeo[posof(i,j)]!=0) return;
                routee[posof(i,j)]=a;
                routeo[posof(i,j)]=a;
                wireProcessor2(i+1,j,0,a,1);
                wireProcessor2(i-1,j,0,a,2);
                wireProcessor2(i,j+1,0,a,3);
                wireProcessor2(i,j-1,0,a,4);
                break;
            case ' ':
                break;
            case '[':
            case ']':
                cout << "Not yet implemented\n";
                break;
            default:
                throw SyntaxError();
        }
    }
};

int main()
{
    convertToVecStr();
    module a;
    try{
        a.parse(board);
        a.printModule();
    }
    catch(int y)
    {
        cout << y << "! you are " << y << "!";
    }
    catch(SyntaxError se)
    {
        cout << se.what();
    }
    return 0;
}
