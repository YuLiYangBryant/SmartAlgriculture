#!/usr/bin/env python
# coding: utf-8

# In[1]:


get_ipython().system('pip install paho-mqtt')
get_ipython().system('pip install PyMySQL')


# In[1]:


#!/usr/bin/env python
#conding=utf-8


import paho.mqtt.client as mqtt
import pymysql
import logging 





#db = pymysql.connect("127.0.0.1","root","123456","thu_lake")
conn = pymysql.connect(
    host = 'bbbrryannttt.me',
    port = 3306,
    user = 'kobe',
    passwd = '01291002',
    db = 'thu_becky'
)
cursor = conn.cursor()










# 當地端程式連線伺服器得到回應時，要做的動作
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
    #print(data_split[11][12:28])
    #print(data_split[12])
    
    if mac =='00000000aa110090':
        sadata = data_split[12][9:19]
        moi = sadata[0:3]
        pir = sadata[3:6]
        light = sadata[6:10]
        
        
        print('moi:' + moi)
        print('pir:' + pir)
        print('light:' + light)
     #   print(data_split[11])
     #   print(data_split[12])
        cursor.execute("INSERT INTO test(moi, light, pir) VALUES (%s, %s, %s);" %(moi,light,pir))
        conn.commit()
    
    

    
         
    


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





# In[ ]:




