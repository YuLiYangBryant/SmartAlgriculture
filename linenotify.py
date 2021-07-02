#!/usr/bin/env python
# coding: utf-8

# In[2]:


get_ipython().system('pip install requests')


# In[ ]:


#!/usr/bin/env python
#conding=utf-8


import paho.mqtt.client as mqtt
import pymysql
import logging 
import requests
token = 'c3XbAW87YujrhR6KKNGQSKWReX3sTKtvMkxzpLJNf8N'
moidrypir1warning = '注意！！！土壤太乾，有人經過感測區！！！'
moidrypir0warning = '注意！！！土壤太乾！！！'
moiwatpir1warning = '注意！！！土壤太濕，有人經過感測區！！！'
moiwatpir0warning = '注意！！！土壤太濕！！！'
moinonpir1warning = '注意！！！有人經過感測區！！！'


def linenotifyMessage(token, msg):
    headers = {
    "Authorization": "Bearer " + token, 
    "Content-Type" : "application/x-www-form-urlencoded"
    }

    payload = {'message': msg}
    r = requests.post("https://notify-api.line.me/api/notify", headers = headers, params = payload)
    return r.status_code

  # 修改為你要傳送的訊息內容
#message = 'Notify from LINE, HELLO WORLD'
  # 修改為你的權杖內容





def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # 將訂閱主題寫在on_connet中
    # 如果我們失去連線或重新連線時 
    # 地端程式將會重新訂閱
    client.subscribe("GIOT-GW/UL/1C497B455949")

# 當接收到從伺服器發送的訊息時要進行的動作
def on_message(client, userdata, msg):
    # 轉換編碼utf-8才看得懂中文
    #print(msg.topic+" "+ msg.payload.decode('utf-8'))
    data = msg.payload.decode('utf-8')
    str_data = str(data)
    data_split = str_data.split(',',-1)
    mac = data_split[11][12:28]

    
    if mac =='00000000aa110090':
        sadata = data_split[12][9:19]
        MOI = sadata[0:3]
        PIR = sadata[3:6]
        print(MOI)
        print(PIR)
        
        moi = int(MOI)
        pir = int(PIR)
        
        
        
        if moi <= 50 and pir == 1:
            linenotifyMessage(token, moidrypir1warning)
        elif moi <= 50 and pir == 0:
            linenotifyMessage(token, moidrypir0warning)
        elif moi >= 85 and pir == 1:
            linenotifyMessage(token, moiwatpir1warning)
        elif moi >= 85 and pir == 0:
            linenotifyMessage(token, moiwatpir0warning)
        elif moi < 85 and moi > 50 and pir == 1:
            linenotifyMessage(token, moinonpir1warning)
        else:
            None
            
        



 
    
    

    
         
    


# 連線設定
# 初始化地端程式
client = mqtt.Client()

# 設定連線的動作
client.on_connect = on_connect

# 設定接收訊息的動作
client.on_message = on_message

# 設定登入帳號密碼
client.username_pw_set("admin","admin")

# 設定連線資訊(IP, Port, 連線時間)
client.connect("104.199.215.165", 1883, 60)

# 開始連線，執行設定的動作和處理重新連線問題
# 也可以手動使用其他loop函式來進行連接
client.loop_forever()


# In[ ]:

#test for git pull


