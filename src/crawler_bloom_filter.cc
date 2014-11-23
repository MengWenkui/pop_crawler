#include "crawler_bloom_filter.h"
using namespace std;

void crawler_bloom_filter::_constructor(const unsigned long long max_ele_num,const double false_positive)
{
    ele_counts = 0;
    //布隆过滤器位图大小计算公式：2*最大的元素个数*ln(1/假正率)
    _size = (long long)(2 * max_ele_num * log((long double)1 / false_positive));
    //布隆过滤器哈希函数个数计算公式：0.7*(位图大小/最大元素个数)
    _hash_num = (int)(0.7 * (_size / max_ele_num));
    //位图块size
    _bit_block = sizeof(unsigned int)*8;
    //动态创建位图
    int bit_map_size = (int)ceil((double)(_size / _bit_block));
    _bit_map = new unsigned int[bit_map_size];
    memset(_bit_map,0,bit_map_size*sizeof(unsigned int)); 
    //创建种子，公式：(n+1)*(n+1)+1，一个简单的公式
    _seeds = new int[_hash_num];
    for(int i = 0;i < _hash_num; i++)
    {
        _seeds[i] = (i+1)*(i+1)+1;
        printf("种子%d=%d",i,_seeds[i]);
    }
    //printf("最大元素个数【%d】,假正率【%0.2f】,位图大小【%d】,哈希函数个数【%d】,位图块大小【%d】\n",max_ele_num,false_positive,_size,_hash_num,_bit_block);

}

//带有参数的构造函数，可以设置最大元素数和假正率，默认为10000和0.01
crawler_bloom_filter::crawler_bloom_filter(const unsigned long long max_ele_num,const double false_positive)
{
    _constructor(max_ele_num,false_positive);
}

//析构函数
crawler_bloom_filter::~crawler_bloom_filter()
{
    delete [] _bit_map;
    delete [] _seeds;
    _bit_map = NULL;
    _seeds = NULL;
}

int crawler_bloom_filter::bloom_filter_init(const unsigned long long max_ele_num,const double false_positive)
{
    _constructor(max_ele_num,false_positive);
}

//判断元素是否在布隆过滤器中存在
//返回值：0不存在，1存在，-1传入的值为空
int crawler_bloom_filter::bloom_exist(const string& jurge_str)
{
    //判断是否为空
    if(jurge_str.empty())
    {
        return -1;
    }

    //查看bitmap中元素是否存在
    for(int i = 0;i < _hash_num; i++)
    {
        int hash_value = bloom_hash_value(jurge_str,i);
        if(!bloom_bit_jurge(hash_value))
        {
            return 0; 
        }
    }

    return 1; 
}

//查看位图中相应位是否为1
//返回值：true是1,false不是1
bool crawler_bloom_filter::bloom_bit_jurge(const unsigned long long bit_pos)
{
    //计算位图块位置
    int array_pos = bit_pos / _bit_block;
    //计算偏移量
    int offset = bit_pos % _bit_block;
    //查找到相应的位置，判断是否为1
    if(_bit_map[array_pos] >> (_bit_block-offset) & 0x01)
    {
        return true;
    }
    //printf("位图位置【%d】查看块位置【%d】偏移位置【%d】,结果【不是1】\n",bit_pos,array_pos,offset);
    //printf("该块的二进制位图【%x】",_bit_map[array_pos]);

    return false;
}

//计算hash值，这是一种简单的算法
//返回值：hash值
int crawler_bloom_filter::bloom_hash_value(const string& str,const int n)
{
    int res = 0;
    //迭代每个字符计算hash值
    for(int i = 0;i < str.size()-1;i++)
    {
        res = _seeds[n] * res + (int)str[i];
        if(res > _size)
        {
            res %= _size;
        }
    }
    //printf("传入的字符串【%s】，计算的hash值为【%d】",str.c_str(),res);

    return res;
}

//向布隆过滤器中添加元素
//返回值：
int crawler_bloom_filter::bloom_add(const string& element)
{
    //传入字符串是否为空
    if(element.empty())
    {
        return -1;
    }
    //传入的字符串是否已经在布隆过滤器中
    if(bloom_exist(element))
    {
        return -2;
    }
    //传入的字符串不在布隆过滤器中
    //向位图中添加hash掩码
    for(int i = 0;i < _hash_num;i++)
    {
        int hash_value = bloom_hash_value(element,i);
        bloom_bit_set(hash_value);
    }
    return 0;
}

//在位图的相应位置设为1
//返回值：true设置成功，false设置失败
bool crawler_bloom_filter::bloom_bit_set(const unsigned long long  bit_pos)
{
    if(bit_pos >= _size)
    {
        return false;
    }

    int array_pos = bit_pos / _bit_block;
    int offset = bit_pos % _bit_block;
    _bit_map[array_pos] = _bit_map[array_pos] | (1 << (_bit_block-offset));
    
    //printf("位图位置,更改块位置【%d】偏移位置【%d】,结果【成功】\n",bit_pos,array_pos,offset);
    //printf("该块的二进制位图【%x】",(1<<_bit_block-offset));
    return true;
}
