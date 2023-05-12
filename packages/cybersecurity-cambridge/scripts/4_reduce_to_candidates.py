import pprint as pp
import json

# filter out those who are in the breach database

def main():
    people = []
    with open("cleaned_list.json") as file:
        people = json.load(file)
    candidates = []
    for person in people:
        password = person.get("password")
        if password:
            candidates.append(person)
    with open("candidates.json", "w") as outfile:
        json.dump(candidates, outfile)
    print(len(candidates))

if __name__ == "__main__":
    main()
