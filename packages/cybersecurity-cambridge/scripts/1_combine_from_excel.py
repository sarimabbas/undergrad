import json
import csv
import pprint as pp

# some emails were collected into an excel sheet 
# and needed to be merged in with the results from 0_scrape.py
# the final output list was about 4000

def main():
    people_cleaned = []
    csv_data = None
    infile = open("emails.csv") 
    csv_data = csv.reader(infile)
    for row in csv_data:
        person_cleaned = {}
        person_cleaned["name"] = row[0].strip()
        person_cleaned["division"] = row[1].strip()
        person_cleaned["email"] = row[2].strip()
        person_cleaned["role"] = ""
        people_cleaned.append(person_cleaned)
    infile.close()
    infile_json = open("list.json")
    existing_emails = json.load(infile_json)

    print("In csv: {}".format(len(people_cleaned[1:])))
    print("In existing: {}".format(len(existing_emails)))

    infile_json.close()
    existing_emails.extend(people_cleaned[1:])

    print("In total: {}".format(len(existing_emails)))

    outfile_json = open("new_list.json", "w")
    json.dump(existing_emails, outfile_json)
    print("Finished")
    

if __name__ == "__main__":
    main()


