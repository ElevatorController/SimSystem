#include <iostream>
using namespace std;

class Elevator
{
private:
    int floor_now;//��ǰ����¥��
    int floor_headfor;//Ŀ��¥��
    int floor_max;
    int floor_min;//����Χ
    // bool people_num_now_mutex;//�߳���//�о������������Ƿֱ���ģ�����Ҫ��������   
    unsigned int people_num_now;//��ǰ����
    unsigned int people_num_max;//��������
    double weight_now;//��ǰ����
    double weight_max;//��������
    bool is_full;
    int state;//1Ϊ���У�-1Ϊ���ã�0Ϊ��վ//����״̬
    int headfor;//1Ϊ���У�-1Ϊ���У�0Ϊͣ��//���ݵ�ǰ���з���
    Sequence elevator_working_sequence;//�����б�
    bool sequence_mutex;//�߳���//ͬʱֻ����һ���̸߳��Ķ��С�
    bool* buttom_status;
public:
    //��ȡ
    void getPeopleNumCV();//�Ӿ���ȡ����
    void getWeight();//��������ȡ����
    void getButtonStatus();//��ȡ��ť״̬

    //�����ͨ��
    void getInf();//�յ���Ϣ
    void handleRequire();//������������
    void sendInf();//������Ϣ

    //��������
    bool inServiceRange();//�ж��Ƿ��ڷ���Χ��
    bool overWeightDetect();//�ж��Ƿ���
    bool overPeopleNumDetect();//�ж��Ƿ�����
    void addSequenceNode();//���빤�����нڵ�
    void deleteSequenceNode();//ɾ���������нڵ�
    void setTargetFloor();//����Ŀ��¥��
    void lathe();//��
    void arrive();//����

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