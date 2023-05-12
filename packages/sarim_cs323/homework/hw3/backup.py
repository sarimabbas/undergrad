# general case
define_dict = {}
prev = None
for i in range(0, len_matches):
		# check if directive is syntactically correct
		if not checkDirectiveSyntax(match):
			continue
		# if it is a define, process it
		if matches[i].group("type") == "define":
			# process into define_dict
			name_value[match.group("name")] = (match.group("value").rstrip()
                                      if match.group("value") else "")
		# if it is an ifdef, check whether is is defined
		if matches[i].group("type") == "ifdef":
			if matches[i].group("name") in define_dict:
				# get content and append
				if_content = contents[matches[i].end():matches[i + 1].start()]
				processed_contents.append(if_content)
				# TODO: expand tokens
			else:
				continue
		# if it is an endif,
		if matches[i].group


def ifDefPattern():
	# iteration 1: does not work with else
	regex = r"(?P<ifdef>\# *ifdef)(?: *)(?P<name>[^\W\d]+\w*)(?:\s)(?P<content>.*)(?P<endif>\# *endif)"
	# iteration 2: works with else
	regex = r"(?P<ifdef>\# *ifdef)(?: *)(?P<name>[^\W\d]+\w*)(?:\s)(?P<content_if>.*)(?P<else>\# *else)(?:\s)(?P<content_else>.*)(?P<endif>\# *endif)"
	# iteration 3: lazy matching version
	regex = r"(?P<ifdef>\# *ifdef)(?: *)(?P<name>[^\W\d]+\w*)(?:\s)(?P<content_if>.*?)(?P<else>\# *else)(?:\s)(?P<content_else>.*?)(?P<endif>\# *endif)"
	pattern = re.compile(regex, re.MULTILINE | re.DOTALL)
	return pattern


# do any error checking for invalid directives
name_value = {}
for match in matches:
		# check for missing names for directives that need them
		if match.group("type") in ["define", "undef", "ifdef"]:
			# if there aren't names, print error
			if not match.group("name"):
				printDirectiveError(match)
			# otherwise if there are names...
			else:
				# store all #define into dictionary
				if match.group("type") == "define":
					name_value[match.group("name")] = (match.group("value").rstrip()
                                            if match.group("value") else "")
		# process #include separately (the funky regex see's "FILE" of type <value>)
		elif match.group("type") == "include":
			if not match.group("value"):
				printDirectiveError(match)
		# these cannot have any names or values
		elif match.group("type") in ["endif", "else"]:
			if match.group("name") or match.group("value"):
				printDirectiveError(match)
		# and no other directives can be accepted
		else:
			printDirectiveError(match)
