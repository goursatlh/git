#if 1 // 3.13
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main(void)
{
    vector<int> num, sum;
    int tmp = 0;
    int count = 0;
    int index = 0;
    int i = 0;

    cout<<"please input numbers: "<<endl;
    while (cin>>tmp)
    {
        num.push_back(tmp);
        if ((count >= 2) && ((count % 2) == 0))
        {
            sum[index++] = num[count-2] + num[count-1];
        }
        cin>>num[count++];
    }
    --count;

    if ((count % 2) != 0)
    {
        cout<<"Total number is "<<count<<" the last one is "<<num[count-1]<<endl; 
    }

    for (i = 0; i< count; i++)
    {
        cout<<num[i]<<" ";
    }

    cout<<endl;
    
    cout<<"use iterator to walk vector<int> type: "<<endl;
    for (vector<int>::iterator iter = num.begin(); iter != num.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    
    for (i = 0; i < index; i++)
    {
        cout<<sum[i]<<"   ";
    }
    cout<<endl;
    
    return 0;
}
#endif
