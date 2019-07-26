from flask import Flask,request,render_template,redirect,session
import uuid
from geventwebsocket.handler import WebSocketHandler
from gevent.pywsgi import WSGIServer
import json


app = Flask(__name__)



WEBSOCKET_DICT = {}

@app.route('/message/<user>')
def message(user):
    print(user)
    print(request.values)
    # 1. 判断是否为Websocket请求,http不包含wsgi.websocket
    ws = request.environ.get('wsgi.websocket')
    print(ws)
    if not ws:
        return 'use websocket'
    # 此处连接成功

    WEBSOCKET_DICT[user] = ws
    while True:
        # 2. 等待用户发送消息，并接受
        message = ws.receive()
        print(message)
        try:
            message1=json.loads(message)
            print(message)


            if "Disconnect" in message1:
                del WEBSOCKET_DICT[user]
            if "target" in message1:
                # if message1['target'] in WEBSOCKET_DICT:
                for user_s in WEBSOCKET_DICT.values():
                    data = {"Down": user}
                    # user_s.send(json.dumps(data))
                    user_s.send(message)

            elif "Online" in message1:
                for user_s in WEBSOCKET_DICT.values():
                    data={"Up":user}
                    user_s.send(json.dumps(data))
            elif "Offline" in message1:
                for user_s in WEBSOCKET_DICT.values():
                    data={"Down":user}
                    user_s.send(json.dumps(data))
            else:
                data = {"Msg": "Offline!"}
                WEBSOCKET_DICT[user].send(json.dumps(data))
        except:pass







if __name__ == '__main__':
    # 如果是http请求走app使用原有的wsgi处理，如果是websocket请求走WebSocketHandler处理
    http_server = WSGIServer(('', 10002), app, handler_class=WebSocketHandler)
    http_server.serve_forever()