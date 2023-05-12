# Cyber Security @ Cambridge

Scripts used for OSINT and phish attempt.

All identifying information, URLs, and credentials have been stripped.

## phish_app

Download files

```
git clone https://github.com/sarimabbas/cybersecurity-cambridge
```

Virtual environment setup

```
mkdir ~/.virtualenv
cd ~/.virtualenvs
python3 -m venv cybersecurity
source ~/.virtualenvs/cybersecurity/bin/activate
```

Dependencies installation

```
cd /path/to/cybersecurity-cambridge/phish_app
pip3 install -r requirements.txt
```

Running Flask development server

```
export FLASK_APP=app.py
flask run
```

The app can be deployed to a host like PythonAnywhere:  https://help.pythonanywhere.com/pages/Flask/

## scripts

Working (but not fully tested) scripts, ordered into a workflow and cleaned of vulnerable information.

Execute using: 

```
python3 script_name.py 
```
