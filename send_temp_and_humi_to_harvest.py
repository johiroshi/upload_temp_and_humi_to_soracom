#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import serial
import time
import requests
import json

# 第一引数を interval に設定
interval=5 if len(sys.argv)==1 else int(sys.argv[1])

def main():
    con=serial.Serial('/dev/ttyACM0', 9600)
    print('connected.')
    while 1:
        start_time = time.time()
        print("- 温湿度を測定します")
        str=con.readline() # byte code
        if (str):
            data = str.strip().decode('utf-8')
            token = data.split(",")
            temprature = token[0]
            humidity = token[1]
            print(token[0])
            print(token[1])
            headers = {'Content-Type': 'application/json'}
            payload = {'temprature': temprature, 'humidity': humidity }
            print("- データを送信します")
            try:
                r = requests.post('http://harvest.soracom.io', data=json.dumps(payload),headers=headers, timeout=5)
                print(r)
            except requests.exceptions.ConnectTimeout:
                print('ERROR: 接続がタイムアウトしました。"connect_air.sh" は実行していますか？')
                sys.exit(1)
                if r.status_code == 400:
                    print('ERROR: データ送信に失敗しました。Harvest が有効になっていない可能性があります。')
                    sys.exit(1)
        # 指定した秒数に１回実行するためのウェイトを入れる
        wait = start_time + interval - start_time
        if wait > 0:
            time.sleep(wait)

if __name__ == '__main__':
    main()
