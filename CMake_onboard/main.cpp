#include <iostream>
using namespace std;

class Elevator
{
private:
    int floor_now;//当前所在楼层
    int floor_headfor;//目标楼层
    int floor_max;
    int floor_min;//服务范围
    // bool people_num_now_mutex;//线程锁//感觉人数和质量是分别检测的，不需要互相锁？   
    unsigned int people_num_now;//当前人数
    unsigned int people_num_max;//人数限制
    double weight_now;//当前重量
    double weight_max;//重量限制
    bool is_full;
    int state;//1为运行，-1为闲置，0为到站//电梯状态
    int headfor;//1为上行，-1为下行，0为停靠//电梯当前运行方向
    Sequence elevator_working_sequence;//工作列表
    bool sequence_mutex;//线程锁//同时只允许一个线程更改队列。
    bool* buttom_status;
public:
    //获取
    void getPeopleNumCV();//视觉获取人数
    void getWeight();//传感器获取质量
    void getButtonStatus();//获取按钮状态

    //与调度通信
    void getInf();//收到信息
    void handleRequire();//处理发来的请求
    void sendInf();//发送信息

    //其它方法
    bool inServiceRange();//判断是否在服务范围内
    bool overWeightDetect();//判断是否超重
    bool overPeopleNumDetect();//判断是否超人数
    void addSequenceNode();//插入工作序列节点
    void deleteSequenceNode();//删除工作序列节点
    void setTargetFloor();//设置目标楼层
    void lathe();//车
    void arrive();//到达

};
struct Sequence
{
    SequenceNode* sequence_head;
    SequenceNode* p;
    
};
struct SequenceNode
{
    int to_floor;
    SequenceNode* next_node;
};
int main()
{
    cout<<"HelloWorld"<<endl;
    return 0;
}