#include <bits/stdc++.h>
using namespace std;
static int x = 0, y = 0, minimum = 0, maximum = 0, h = 0, maxi = 0, mini = 0;
typedef struct node {
    int min, max;
    int u;
    int* bit;
    struct node** cluster;

} node;

void print(node* ptr, int u, int v)
{
    if (ptr == NULL)
        return;
    if (u == 2 && ptr != NULL) {
        if (ptr->bit[0] == 1) {
            cout << v << endl;
            x++;
        }
        if (ptr->bit[1] == 1) {
            x++;
            cout << v + 1 << endl;
        }

        return;
    }
    else {
        for (int i = 0; i < ptr->u; i++) {
            int t = v + (ptr->u) * i;
            print(ptr->cluster[i], sqrt(u), t);
        }
        return;
    }
}

struct node* insert(int k, int u, node* ptr)
{

    if (u == 2 && ptr == NULL) {
        ptr = new node;
        ptr->bit = new int[2];
        ptr->u = 2;
        ptr->cluster = NULL;
        for (int i = 0; i < ptr->u; i++) {
            ptr->bit[i] = 0;
        }
        ptr->max = ptr->min = -1;
    }
    else if (u != 2 && ptr == NULL) {
        ptr = new node[(int)sqrt(u)];
        ptr->u = (int)sqrt(u);
        ptr->cluster = (node**)new node[(int)sqrt(u)];
        ptr->bit = new int[(int)sqrt(u)];
        for (int i = 0; i < ptr->u; i++) {
            ptr->bit[i] = 0;
        }
        for (int i = 0; i < ptr->u; i++) {
            ptr->cluster[i] = NULL;
        }
        ptr->max = ptr->min = -1;
    }

    if (u == 2) {
        ptr->bit[k] = 1;

        if (ptr->max == ptr->min && ptr->max == -1) {
            ptr->max = ptr->min = k;
        }
        else if (k < ptr->min) {
            ptr->min = k;
        }
        else if (k > ptr->max) {
            ptr->max = k;
        }
        //cout<<"max "<<ptr->max<<" min "<<ptr->min<<endl;
        return ptr;
    }

    int cluster_no = k / (ptr->u);
    int position = k % (ptr->u);
    if (ptr->min == ptr->max && ptr->min == -1) {
        ptr->min = ptr->max = cluster_no;
    }
    else if (cluster_no < ptr->min) {
        ptr->min = cluster_no;
        // cout<<cluster_no<<endl;
    }
    else if (cluster_no > ptr->max) {
        ptr->max = cluster_no;
    }
    ptr->bit[cluster_no] = 1;
    ptr->cluster[cluster_no] = insert(position, sqrt(u), ptr->cluster[cluster_no]);

    return ptr;
}
void check(node* ptr, int k)
{
    if (ptr->cluster == NULL && ptr != NULL) {
        if (ptr->bit[k] == 1) {
            cout << "found" << endl;
            return;
        }
        else {
            cout << "not found" << endl;
            return;
        }
    }
    if (ptr == NULL) {
        cout << "not found" << endl;
        return;
    }

    int cluster_no = k / ptr->u;
    int position = k % ptr->u;

    check(ptr->cluster[cluster_no], position);
}
void min(node* ptr)
{
    if (ptr->cluster == NULL && ptr != NULL) {
        minimum = minimum + ptr->min;
        return;
    }
    else {
        minimum = minimum + (ptr->min) * (ptr->u);
        //cout<<minimum<<endl;
        min(ptr->cluster[ptr->min]);
    }
}

void max(node* ptr)
{
    if (ptr->cluster == NULL && ptr != NULL) {
        maximum = maximum + ptr->max;
        return;
    }
    else {
        maximum = maximum + (ptr->max) * (ptr->u);
        //cout<<maximum<<endl;
        max(ptr->cluster[ptr->max]);
    }
}
struct node* deleted(node* ptr, int u, int k)
{
    if (ptr == NULL) {
        return (NULL);
    }

    if (u == 2) {
        if (k == ptr->max && ptr->max != ptr->min) {
            ptr->max = ptr->min;
            ptr->bit[k] = 0;
        }

        else if (ptr->max != ptr->min && k == ptr->min) {
            ptr->min = ptr->max;
            ptr->bit[k] = 0;
            cout << "hello" << endl;
        }
        else {
            ptr->max = -1;
            ptr->min = -1;
            ptr->bit[k] = 0;

            delete (ptr->bit);
            delete (ptr->cluster);
            delete (ptr);
            return NULL;
        }
        return (ptr);
    }

    else {
        int postion = k % ptr->u;
        int cluster_no = k / ptr->u;
        ptr->cluster[cluster_no] = deleted(ptr->cluster[cluster_no], ptr->u, postion);
        if (ptr->cluster[cluster_no] == NULL) {
            ptr->bit[cluster_no] = 0;
            if (cluster_no == ptr->max && ptr->max == ptr->min) {
                delete (ptr->bit);
                delete (ptr->cluster);
                delete (ptr);
            }
            else if (cluster_no == ptr->max && ptr->max != ptr->min) {
                for (int i = ptr->max - 1; i >= 0; i--) {
                    if (ptr->bit[i] == 1) {
                        ptr->max = i;
                        break;
                    }
                }
            }
            else if (cluster_no == ptr->min && ptr->min != ptr->max) {
                for (int i = ptr->min + 1; i < ptr->u; i++) {
                    if (ptr->bit[i] == 1) {
                        ptr->max = i;
                        break;
                    }
                }
            }
        }
    }
    return (ptr);
}

int findmax(node* ptr, int maxi)
{
    if (ptr->cluster == NULL && ptr != NULL && ptr->u == 2) {
        maxi = maxi + ptr->max;
        return maxi;
    }
    else {
        maxi = maxi + (ptr->max) * (ptr->u);
        maxi = findmax(ptr->cluster[ptr->max], maxi);
    }
}
int findmin(node* ptr, int mini)
{
    if (ptr->cluster == NULL && ptr != NULL && ptr->u == 2) {
        mini = mini + ptr->min;
        return mini;
    }
    else {
        mini = mini + (ptr->min) * (ptr->u);

        mini = findmin(ptr->cluster[ptr->min], mini);
    }
}

static int cluste_no;
static bool flag = false;
static int a;
static int postion;

void succ(node* ptr, int k, int h, int u)
{
    int t = ptr->u;
    u = sqrt(u);
    cluste_no = k / (ptr->u);
    postion = k % ptr->u;
    cout << k << " kvalue" << endl;
    cout << "ptr->u " << ptr->u << endl;
    cout << cluste_no << " make" << endl;
    cout << postion << " postion" << endl;

    //cout<<k<<" kvalue"<<endl;
    //cout<<"findmax "<<findmax(ptr,0)<<endl;
    //cout<<" u is "<<u<<endl;

    //cout<<"mellow"<<endl;
    if (u == 1 && ptr->cluster == NULL && ptr->u == 2) {
        cout << "gone" << endl;
        cout << h + ptr->max << endl;
        return;
    }

    else if (postion < findmax(ptr->cluster[cluste_no], 0) && ptr != NULL && ptr->cluster != NULL) {

        h = h + (ptr->u) * cluste_no;
        //cout<<"hvalue "<<h<<endl;
        cout << "hello" << endl;
        succ(ptr->cluster[cluste_no], postion, h, u);
    }

    else {
        //cout<<"just checking"<<endl;
        cluste_no = cluste_no + 1;
        //cout<<"cluster_no "<< cluste_no<<endl;
        int p;
        //cout<<"hello "<<ptr->u<<endl;
        for (int i = cluste_no; i < ptr->u; i++) {
            //cout<<"check "<<i<<endl;
            if (ptr->bit[i] == 1) {
                p = i;
                //cout<<"i"<<i<<endl;
                break;
            }
        }
        h = h + (p)*ptr->u;
        //cout<<"k"<<p<<endl;
        //cout<<ptr->u<<endl;
        cout << h + findmin(ptr->cluster[p], 0) << endl;
    }
}
void pre(node* ptr, int k, int h, int u)
{
    int t = ptr->u;
    u = sqrt(u);
    cluste_no = k / t;
    postion = k % ptr->u;
    if (k == 0) {
        cluste_no = 0;
    }
    if (u == 1 && ptr->cluster == NULL && ptr->u == 2) {
        cout << h + ptr->min << endl;
        return;
    }

    else if (postion > findmin(ptr->cluster[cluste_no], 0) && ptr != NULL && ptr->cluster != NULL) {

        h = h + (ptr->u) * cluste_no;
        pre(ptr->cluster[cluste_no], postion, h, u);
    }
    else {
        //cout<<"just checking"<<endl;
        cluste_no = cluste_no - 1;
        int p;
        for (int i = cluste_no; i >= 0; i--) {
            if (ptr->bit[i] == 1) {
                p = i;
                break;
            }
        }
        h = h + (p)*ptr->u;
        cout << h + findmax(ptr->cluster[p], 0) << endl;
    }
}

int main()
{
    int u = 16;
    int k;
    struct node* ptr = NULL;
    for (int i = 0; i < 3; i++) {
        cin >> k;
        ptr = insert(k, u, ptr);
    }
    cout << "before deletion" << endl;
    print(ptr, u, 0);
    cout << x << endl;

    min(ptr);

    cout << minimum << endl;

    max(ptr);
    cout << maximum << endl;
    cout << "after deletion" << endl;

    int l;
    cin >> l;
    pre(ptr, l, h, u);
    return (0);
}