import re
import sys

def main():
	def one():
		two()

	def two():
		print("TWO")

	two()
	regex = r"(?:\# *)(?P<type>\w+)(?: *)(?P<name>[^\W\d]+\w*)?(?: *)(?P<value>.*\b)"
	test_str = ("#       define 9sarim abbas \n\n"
				"# define 9this breaks down \n"
				" \n"
				"#else\n"
				"#endif \n\n"
				"#define \n\n"
				"#define 324 knwfergkjrejkg krjngkrjegh\n\n"
				"#else\n\n"
				"#define AB 12\n"
				"#define A a\n"
				"#define B b\n\n"
				"# define name\n\n"
				"#define name\n\n\n"
				"#define\n\n")
	matches = re.finditer(regex, test_str, re.MULTILINE)
	for match in matches:
		print(match)





if __name__ == "__main__":
    main()
