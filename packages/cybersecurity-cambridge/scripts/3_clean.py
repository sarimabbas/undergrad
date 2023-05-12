import requests
import json
import pprint as pp

# clean the results from previous

def load_file(path):
    people = []
    with open(path) as file:
        people = json.load(file)
    return(people)

def write_file(path, data):
    with open(path, 'w') as outfile:
        json.dump(data, outfile)

def main():
    people = load_file("new_list.json")

    # clean passwords
    for person in people:
        password_entry = person["password"]
        possible_passwords = password_entry.split('\n')
        cleaned_passwords = []
        for entry in possible_passwords:
            if ':' in entry:
                cleaned_passwords.append(entry.strip().split(':')[1])
        person["password"] = cleaned_passwords

    # extract usernames
    for person in people:
        username = person["email"].split('@')[0]
        person["username"] = username

    write_file("cleaned_list.json", people)


if __name__ == "__main__":
    main()
