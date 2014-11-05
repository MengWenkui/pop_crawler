#include <queue>
#include <iostream>
using namespace std;

struct job
{
    void* (*callback_func);
    void *arg;
    struct job *next;
    int level;
};

struct cmp
{
    bool operator()(job a,job b)
    {
        return b.level > a.level;
    }
};

int main(int argc,char **argv)
{
    priority_queue<job,vector<*job>,cmp> q;
    struct job *m1;
    struct job *m2;
    m1.level = 2;
    m2.level = 3;
    q.push(m1);
    q.push(m2);
    cout << q.top().level << endl << q.top().level << endl;
    
    return 0;
}
