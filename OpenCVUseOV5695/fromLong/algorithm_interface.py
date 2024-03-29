import json
from json import JSONEncoder
import make_json_serializable  # 用于自定义对象的JSON
import algorithm_test

# 关于楼层编号:从地下3到地上18编号 0-20 一楼编号:3
NUMBER_OF_FLOOR_LEVELS = 21     #地面编号：  0， 1， 2， 3， 4， 5， 6， 7， 8， 9，10，11，12，13，14，15，16，17，18，19，20
                                #楼层编号   -3，-2，-1,  1， 2， 3， 4， 5， 6， 7，8， 9， 10，11，12，13，14，15，16，17，18
max_person = 12  # 电梯载客量
elevator_speed = 0.5  # 电梯速度,每秒0.5层



class algorithm_result:
    elevators = []
    people = []

    def to_json(self):
        dict_name_value = {}

        for name, value in vars(self).items():
            dict_name_value[name] = value
        return dict_name_value


if __name__ == "__main__":
    # result = algorithm_test.test2()
    # a = algorithm_result()
    # a.elevators = result[0]
    # a.people = result[1]
    # print(json.dumps(a, indent=4))
    # print(json.dumps(result, indent=4))
    algorithm_test.py_test()

    # class person:  # 人
#     come_time = 0  # 出现的时间
#     from_floor = 0  # 出发楼层
#     to_floor = 0  # 到达楼层
#     current_floor = 0  # 现在的楼层
#     in_which_elevator = 0  # 在哪个电梯中,可取 0 1 2 3 分别指不在电梯中 在第一个电梯中 在第二个电梯中 在第三个电梯中
#     is_out = False  # 是否已经完成电梯乘坐

#     def to_json(self):
#         dict_name_value = {}

#         for name, value in vars(self).items():
#             dict_name_value[name] = value
#         return dict_name_value
# # 电梯
# # e1:只停单层，e2:只停双数层(包括1层)，e3:全停
# # 单:index=0 2 3 5 7 9 ..
# # 双:index=1 4 6 8 ..
# # index=3为F1,3个电梯都可以停


# class elevator:
#     current_floor = 3.0  # 现在的楼层,允许小数,每一秒都要进行精确更新.初始位置1楼
#     move_direction = 0  # 移动方向,可取到0 1 2,分别是 停止 向上 向下

#     def __init__(self, current_floor, move_direction):
#         self.current_floor = current_floor  
#         self.move_direction = move_direction

#     def to_json(self):
#         dict_name_value = {}

#         for name, value in vars(self).items():
#             dict_name_value[name] = value
#         return dict_name_value
# # 核心算法:顺向截停
# # 对输入人群数组,不应假设每个人的出现时间都是1 6 11 ... 而可能是任意整数时间点(不方便的话再进行沟通),不应假设come_time<=t


# def core_algorithm(time, array_people):
#     pass