import pprint as pp
import json
import subprocess

# we checked each record against the provided Breach Database
# to see if we got any hits

def load_file(path):
    people = []
    with open(path) as file:
        people = json.load(file)
    return(people)

def write_file(path, data):
    with open(path, 'w') as outfile:
        json.dump(data, outfile)
        
def main():
    people = load_file("records.json")
    base_command = '/opt/breach/BreachCompilation/query.sh '
    for person in people:
        email = person.get("email")
        if email:
            final_command = base_command + email
            process = subprocess.Popen(final_command.split(), stdout=subprocess.PIPE)
            output, error = process.communicate()
            person["password"] = output.decode('utf-8')
    write_file("records_with_passwords.json", people)

if __name__ == "__main__":
    main()
