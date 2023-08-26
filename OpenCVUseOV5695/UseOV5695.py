import cv2
import time
import os
import torch
from PIL import Image

def main():
    # 使用GStreamer管道从MIPI摄像头捕获视频，添加视频帧率
    # gst_pipeline = 'v4l2src device=/dev/video0 ! video/x-raw,format=NV12,width=640,height=480,framerate=30/1 ! appsink'
    # 从GStreamer管道创建OpenCV VideoCapture对象
    # cap = cv2.VideoCapture(gst_pipeline, cv2.CAP_GSTREAMER)
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        # 如果无法打开摄像头，则输出提示信息
        print("无法打开摄像头")
        return
    time0 = time.time() # 获取开始时间

        
    childpid=os.fork() #创建子进程
    if childpid==0:#子进程
        print("寻找可执行文件")
        os.system('/home/topeet/Elevator/SSHClient')#执行可执行文件
    else:#父进程
        model = torch.hub.load('ultralytics/yolov5', 'custom', path='yolov5s.pt')
        people_num_remain=0
        # begin_time:int=1
        while True:
            # 从摄像头捕获帧
            ret, frame = cap.read()
            # print("")
            # print('main2 return {}'.format(rec))
            # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2BGR)
            # 如果捕获到帧，则显示它
            if ret:
                save_path="save_picture.png"
                cv2.imwrite(save_path,frame)
                #人数检测模块
                image_path = 'save_picture.png'
                image = Image.open(image_path)
                results = model(image)
                detections = results.pandas().xyxy[0]
                a = 0
                for _, detection in detections.iterrows():
                    label = detection['name']
                    if label == 'person':
                        a = a+1

                print("人数为："+str(a))
                if a>people_num_remain:#如果有出现人数增加
                    os.system(('/home/topeet/Elevator/writejson '+str(a-people_num_remain)))#执行可执行文件
                people_num_remain=a
                time1 = time.time() # 获取结束时间
                fps = 1 / (time1 - time0) # 计算实时帧率
                time0 = time1 # 更新开始时间
                cv2.putText(frame, "FPS: {:.2f}".format(fps), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2) # 在图像上显示帧率
                cv2.imshow("MIPI Camera", frame)
            # 按下'q'键退出循环
            if cv2.waitKey(1) & 0xFF == ord("q"):
                break
            # print("准备睡觉")
            # time.sleep(1)
            # print("睡觉结束")
            # begin_time=begin_time+1
            print("一次循环结束")

    # 释放资源并关闭窗口
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()