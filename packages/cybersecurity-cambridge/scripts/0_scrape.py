import requests
import json
from bs4 import BeautifulSoup
import pprint as pp

# number removed
PAGES = 0 

def person_parse(person):
    person_cleaned = {}
    person_cleaned["name"] = person.find("div", "name").find("strong").string
    person_cleaned["email"] = person.find("div", "email").find("a").string
    person_cleaned["division"] = person.find("div", "division").find("a").string
    person_cleaned["role"] = person.find("div", "role").string
    return(person_cleaned)

def write_file(path, data):
    with open(path, 'w') as outfile:
        json.dump(data, outfile)
    
def main():
    base_url = "" # URL removed
    people_cleaned = []
    for i in range(1, PAGES + 1):
        page_url = base_url + str(i)
        response = requests.request("GET", page_url)
        soup = BeautifulSoup(response.content, "html.parser")
        people = soup.find_all("div", "") # target class remoced
        for person in people:
            person_cleaned = person_parse(person)
            if person_cleaned not in people_cleaned:
                people_cleaned.append(person_cleaned)
    write_file("list.json", people_cleaned)

if __name__ == "__main__":
    main()
