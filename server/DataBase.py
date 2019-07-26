from flask import Flask, request,jsonify
from flask_pymongo import PyMongo
from flask_socketio import SocketIO
from werkzeug.datastructures import ImmutableMultiDict
import json
app = Flask(__name__)
app.config["MONGO_URI"] = "mongodb://localhost:27017/works"
mongo = PyMongo(app)
socketio = SocketIO(app)
user_socket_dict={}
mongo = PyMongo(app)
@app.route("/user/",methods=['post'])
def user_profile():
    print(request.form['Username'])
    username=request.form['Username']
    password=request.form['Password']
    nm=mongo.db.works.find_one({"name": request.form["Name"]})
    print(nm)
    if(nm):
        a = dict({'Msg': "Name Exist"})
        return jsonify(a)

    name=mongo.db.works.find_one({"username":username})

    if(name):
        user = mongo.db.works.find_one({"username":username,"pass":password})
        if(user):
            a=dict({'Msg':"Already"})
            return jsonify(a)
        else:
            a = dict({'Msg': "Exist"})
            return jsonify(a)
    else:
        a = dict({'Msg': "Success"})
        return jsonify(a)
@app.route("/signup/",methods=['post'])
def signup():
    print(request.form)
    username = request.form['Username']
    password = request.form['Password']
    name=request.form['Name']
    pic=request.form['Pic']
    info=mongo.db.works.find_one({"username":username,"pass":password})
    if(info):
        a = dict({'Msg': "Exist"})
        return jsonify(a)
    else:
        mongo.db.works.insert({"username":username,"pass":password,"name":name,"pic":pic})
        a=dict({'Msg':"Success"})
        return jsonify(a)
@app.route("/signin/",methods=['post'])
def signin():
    username = request.form['Username']
    password = request.form['Password']
    info=mongo.db.works.find_one({"username":username,"pass":password})
    if(info):
        infom=info
        print(info)

        a = dict({'Msg': "Success",'Name':infom['name'],"Pic":infom['pic']})
        if("mem" in infom):
            a['Mem']=infom['mem']
        print(a)

        return jsonify(a)
    else:
        a = dict({'Msg': "Wrong"})
        return jsonify(a)

@app.route("/mem/",methods=['post'])
def member():
    print(request.values)
    print(request.form)
    name=request.form.get('Name',None)
    mem=request.form.get('Mem',None)
    print(name)
    print(mem)
    mongo.db.works.update_one({"name":name},{"$set": { "mem": mem }})
    info = mongo.db.works.find_one({"name": name})
    print(info)
    a=dict({'Msg':"Success"})
    return jsonify(a)
@app.route("/get/",methods=['post'])
def get():
    name = request.form.get('Name', None)
    info = mongo.db.works.find_one({"name": name})
    a = dict({'Msg': "Success", 'Name': info['name'], "Pic": info['pic']})

if __name__ == '__main__':
    app.run(host='0.0.0.0')
