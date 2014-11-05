#################################
类名：crawler_config
功能：配置文件的读写
作者：张春阳
创建时间：2014-10-28
#################################
测试项：

⑴ 配置文件的读取
        情景1：正常格式的key = value格式                         OK
        情景2：'#'注释标识行忽略                                 OK
        情景3：空行忽略                                          OK
        情景4：key = value混合'#'注释                            OK
        情景5：中文key和value的读取                              OK
        情景6：非正常格式的key = value
        （key不在首个位置，key/value之间有不定数量的空格)        OK 
        情景7：相同的key值                                       不支持（会存储最后的赋值）

⑵ 配置文件的写入
        情景1：正常写入key和value
        （key和value都不是中文,key在配置文件中不存在）           OK
        情景2：key为中文                                         OK
        情景3：value为中文                                       OK
        情景4：key在配置文件已存在（修改value）                  OK
        情景5：写入注释                                          不支持（暂未添加该接口）
        情景6：有空行的准确写入                                  OK 
        情景7：配置文件项和注释混合行                            暂不支持（写完文件中文出现乱码）

接口项：
        方法1（构造函数）：
        crawler_config(string file_name);
        说明：生成配置对象,并进行初始化

        方法2（构造函数）：
        crawler_config();
        说明：生成配置文件对象

        方法3（配置文件对象初始化）：
        int config_init(string file_name);
        返回值：0成功
        说明：舒适化配置文件对象

        方法4（写入配置文件）：
        int config_write(string key,string value);
        返回值：0添加配置项，1更新配置项
        说明：写入配置文件

例子项：

        #include <iostream>
        #include "crawler_config.h"
        #include <map>
        using namespace std;

        int main(int argv,char** argc)
        {
            crawler_config m_conf("配置文件路径");
           
            map<string,string>::iterator iter = m_conf.config_map.begin();
            for(;iter != m_conf.config_map.end();iter++)
            {
                cout << iter->first << "\t" << iter->second << endl;
            }
            
            cout << m_conf["key"] << endl;

            return 0;
        }
        
#################################
类名：crawler_log
功能：配置文件的读写
作者：张春阳
创建时间：2014-10-26
#################################
测试项：
    
⑴ 日志文件的写入：
        情景1：写入流程日志                                   OK
        情景2：写入错误日志                                   OK
        情景3：写入系统日志                                  
        （可以写入但是暂时没有实际意义，将来会通过系统日志
        进行一些延续的操作）                                  OK
        情景4：根据写入日志类别不同，实现多种颜色的写入       NO

⑵ 日志的自动切换功能:
        情景1：根据配置文件中的切换间隔时间自动切换日志       OK
        情景2：多个日志对象维护不同的切换时间进行切换         未测试

接口项：
        方法1（构造函数）:  crawler_log();
        无参构造函数
        使用该构造函数创建的实例需要调用log_init()进行初始化

        方法2（构造函数）； crawler_log(const string&);
        有参数构造函数
        使用该构造函数创建的实例不需要再进行初始化

        方法3： LOG_WRITE_PROCESS(const string&,...);
        打印流程日志

        方法4： LOG_WRITE_ERROR(const string&,...);
        打印错误日志

        方法5： LOG_WRITE_SYS(const string&,...);
        打印系统日志
        为了实现延续操作


例子项：
        #include "crawler_log.h"
        #include <unistd.h>
        using namespace std;

        int main(int argc,char **argv)
        {
            crawler_log m_log("../config/log.config");
            string path = "../config/log.config";
            string func = "main";
            while(1)
            {
                m_log.LOG_WRITE_PROCESS("这是一个很长的测试语句，为了测试，只能很长%s%s",path.c_str(),func.c_str());
                sleep(5);
            }

            return 0;
        }
        
#################################
类名：crawler_time_alarm
功能：爬虫定时器类
作者：张春阳
创建时间：2014-11-5
#################################
测试项：

⑴ 定时器设置：
        情景1：一个定时器的设置（包括重复1、多次和一直循环）         OK
        情景2：多个定时器的设置（包括重复1、多次和一直循环）         未测试

接口项：
        接口1（构造函数）：crawler_time_alarm();
        使用该构造函数创建的定时器对像需要调用time_alarm_set()设置定时器的时间以及回调函数

        接口2（构造函数）：crawler_time_alarm(long,long,void* (*)(void*),void*,int repeat_time=1);
        使用该构造函数创建的定时器对象不需要设置，如果调用设置函数，则覆盖初始参数

        接口3（获取定时器剩余次数）：crawler_alarm_get();
        可以获取到定时器剩余的定时次数

        接口4（销毁定时器）：crawler_alarm_destroy();
        销毁定时器对象

例子项：
        
        #include "crawler_time_alarm.h"
        #include <stdio.h>
        #include <unistd.h>
        #include <iostream>
        using namespace std;

        void* call_back(void* arg)
        {
            cout << *(char*)arg << endl;
        }

        int main(int argc,char **argv)
        {
            crawler_time_alarm m_time(1,0,&call_back,(void*)"wokao",-1);
            m_time.time_alarm_create();
            sleep(10);
            m_time.time_alarm_destroy();
            return 0;
        }

#################################
类名：crawler_thread_pool
功能：爬虫线程池类
作者：张春阳
创建时间：2014-11-4
#################################
测试项；

⑴ 创建线程池，线程池从队列中获取任务进行处理
        情景1：加入大量任务，线程池处理                  未测试
        情景2：少量不连贯任务                            未测试
        情景3：线程池中线程数量和工作数的有效比例        未测试
        情景4：在队列中还存在任务时杀死线程池            未测试
        情景5：缓冲队列已满的情况下向队列中添加任务      未测试

接口项：
        接口1（构造函数）：crawler_thread_pool();
        从配置文件中读取线程池中线程的个数以及缓冲队列的大小           
       
        接口2（向任务队列中加入任务）：thread_pool_add_job(void* (*)(void*),void*);
        向任务队列中加入任务
        
        接口3（销毁线程池）：thread_pool_destroy();
        工作进行完自动销毁线程池

例子项：
        #include "crawler_thread_pool.h"
        #include <stdio.h>
        #include <iostream>
        #include <unistd.h>
        #include <pthread.h>
        using namespace std;

        void* work(void* msg)
        {
            char *p = (char*) msg;
            printf("回调函数%s\n",p);
            sleep(1);
        }

        int main(int argv,char **argc)
        {
            crawler_thread_pool m_thread;
            m_thread.thread_pool_add_job(work,(void*)"1");
            m_thread.thread_pool_add_job(work,(void*)"2");
            m_thread.thread_pool_add_job(work,(void*)"3");
            m_thread.thread_pool_add_job(work,(void*)"4");
            m_thread.thread_pool_add_job(work,(void*)"5");
            m_thread.thread_pool_add_job(work,(void*)"6");
            m_thread.thread_pool_add_job(work,(void*)"7");
            m_thread.thread_pool_add_job(work,(void*)"8");
            m_thread.thread_pool_add_job(work,(void*)"9");
            m_thread.thread_pool_add_job(work,(void*)"10");
            m_thread.thread_pool_add_job(work,(void*)"11");
            m_thread.thread_pool_add_job(work,(void*)"12");
            m_thread.thread_pool_add_job(work,(void*)"13");
            m_thread.thread_pool_add_job(work,(void*)"14");
            m_thread.thread_pool_add_job(work,(void*)"15");
            m_thread.thread_pool_add_job(work,(void*)"16");
            sleep(5);
            m_thread.thread_pool_destroy();
            return 0;
        }

#################################
类名：crawler_bloom_filter
功能：爬虫线程池类
作者：张春阳
创建时间：2014-11-1
#################################
测试项；

⑴向布隆过滤器中添加项
        情景1；添加相似的字符串                     未测试
        情景2：添加大量的字符串                     未测试

⑵检查布隆过滤器中是否存在项
        情景1：验证小量相似字符串添加进布隆过滤器   未测试
        假正率
        情景2：验证大量字符串插入的假正率           未测试

⑶ 布隆过滤器的持久化                                未实现
⑷ 删除布隆过滤器中的值
⑸ 对布隆过滤器进行剪发操作


接口项；
        接口1（构造函数）：crawler_bloom_filter(const unsigned long long max_ele_num=10000,const double false_positive=0.01);
        初始化布隆过滤器，默认的大小为10000,假正率为0.01

        接口2（检查布隆过滤器中是否存在该值）：int bloom(const string&);
        检查是否在布隆过滤器中， 有一定的错误率，没有的可能会判断有

        接口3（向布隆过滤器中添加值）：int bloom_add(const string&);
        向布隆过滤器中添加值

例子项：
        
        #include <iostream>
        #include "crawler_bloom_filter.h"
        using namespace std;

        int main(int argc,char **argv)
        {
            crawler_bloom_filter m_bloom;
            m_bloom.bloom_add("爱");
            cout<< "______________________________________" << endl;
            if(m_bloom.bloom_exist("爱") == 1)
            {
                cout << "存在了，成功了一小步" << endl;
            }
            else
            {
                cout << "不存在" << endl;
            }
          
            
            m_bloom.bloom_add("www.baidu.com");
            m_bloom.bloom_add("www.google.com");
            m_bloom.bloom_add("www.sina.com");
           

            if(m_bloom.bloom_exist("www.google.com") == 1)
            {
                cout << "www.google.com 存在了" << endl;
            }
            else
            {
                cout << "www.google.com 不存在" << endl;
            }

            if(m_bloom.bloom_exist("www.office.com") == 1)
            {
                cout << "www.office.com 存在了" << endl;
            }
            else
            {
                cout << "www.office.com 不存在" << endl;
            }
                
            return 0;
        }

#################################
类名：crawler_net
功能：爬虫网络类
作者：张春阳
创建时间：
#################################






#################################
通用错误码
#################################
参数错误                    800
文件路径错误                801
系统错误                    888
线程错误                    900
时间错误                    901
