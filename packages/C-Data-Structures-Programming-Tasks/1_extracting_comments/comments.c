/*
 * Sarim Abbas
 * sarim.abbas@yale.edu
 * comments.c
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h> // isspace function

/* states for the state machine:
 * REG_TEXT = for non-comment characters
 * COMMENT_TYPE_DECIDER = for choosing between single and multi comment styles
 * SINGLE_LINE = for C++ style comments
 * SINGLE_LINE_WHITESPACE = for leading whitespace in a single line comment
 * SINGLE_LINE_NON_WHITESPACE = for non-leading whitespace
 * TAG_DECIDER = for checking start of tag
 * TAG = for working with existing tags
 * LEADING_TAG = for checking tags as part of leading whitespace
 * MULTI_LINE = C style comment state
 * MULTI_LINE_WHITESPACE = for leading whitespace in a multi line comment
 * MULTI_LEADING_TAG = for checking tags as part of leading whitespace
 * DOUBLE_QUOTE = for working with double quotes inside multi line comments
 * DOUBLE_ESCAPER = for escaping backslash within quotes
 * REG_QUOTE = for working with double quotes outside of comments
 * REG_QUOTE_ESCAPER = for escaping backslash within quotes
 */

typedef enum
{
	REG_TEXT, COMMENT_TYPE_DECIDER,
	SINGLE_LINE, SINGLE_LINE_WHITESPACE, SINGLE_LINE_NON_WHITESPACE,
	TAG_DECIDER, TAG, LEADING_TAG,
	MULTI_LINE, MULTI_LINE_WHITESPACE,
	MULTI_LEADING_TAG,
	DOUBLE_QUOTE, DOUBLE_ESCAPER, REG_QUOTE, REG_QUOTE_ESCAPER
} state;

int main(int argc, char* argv[])
{
	// store current and next character
	int read_char;
	int next_char;

	// store the current state
	state current = REG_TEXT;

	// flag to check when a single line comment has started
	bool single_line_started = false;

	// flag to check when a multi line comment has started
	bool multi_line_started = false;

	while((read_char = getchar()) != EOF)
	{
		// find out the next char then put it back
		next_char = getchar();
		ungetc(next_char, stdin);

		switch (current)
		{
			case REG_TEXT:
				if(read_char ==  '/')
				{
					current = COMMENT_TYPE_DECIDER;
				}
				else if(read_char == '\"')
				{
					current = REG_QUOTE;
				}
				break;

			case REG_QUOTE:
				if(read_char == '\"')
				{
					current = REG_TEXT;
				}
				else if(read_char == '\\')
				{
					current = REG_QUOTE_ESCAPER;
				}
				else
				{
					current = REG_QUOTE;
				}
				break;

			case REG_QUOTE_ESCAPER:
				if(read_char)
				{
					current = REG_QUOTE;
				}

			case COMMENT_TYPE_DECIDER:
				if(read_char == '/')
				{
					current = SINGLE_LINE;
					single_line_started = true;
				}
				else if(read_char == '*')
				{
					current = MULTI_LINE;
					multi_line_started = true;
				}
				else
				{
					current = REG_TEXT;
				}
				break;

			case SINGLE_LINE:
				// leading whitespace detector
				// if the single line comment just started, and you get a whitespace char
				// you make the transtition to the single line whitespace dead state
				if(single_line_started == true)
				{
					// first turn off the started indicator
					single_line_started = false;

					// then make the relevant transitions
					if(isspace(read_char) || read_char == '*')
					{
						current = SINGLE_LINE_WHITESPACE;
					}
					else if(read_char == '@')
					{
						current = LEADING_TAG;
					}
					else if(read_char == '\n')
					{
						current = REG_TEXT;
						putchar('\n');
					}
					else
					{
						current = SINGLE_LINE_NON_WHITESPACE;
						putchar(read_char);
					}
				}
				else // if single_line_started == false
				{
					if(read_char == '\n')
					{
						current = REG_TEXT;
						putchar('\n');
					}
					else if(isspace(read_char))
					{
						current = TAG_DECIDER;
						putchar(read_char);
					}
					else // neither newline nor isspace
					{
						current = SINGLE_LINE;
						putchar(read_char);
					}
				}
				break;

			case SINGLE_LINE_WHITESPACE:
				if(read_char == '@')
				{
					current = LEADING_TAG;
				}
				else if(read_char == '\n')
				{
					current = REG_TEXT;
				}
				else if(isspace(read_char) || read_char == '*')
				{
					current = SINGLE_LINE_WHITESPACE;
				}
				else
				{
					current = SINGLE_LINE;
					putchar(read_char);
				}
				break;

			case SINGLE_LINE_NON_WHITESPACE:
				if(read_char == '\n')
				{
					current = REG_TEXT;
					putchar('\n');
				}
				else
				{
					current = SINGLE_LINE;
					putchar(read_char);
				}
				break;

			case TAG_DECIDER:
				if(read_char == '@')
				{
					current = TAG;
				}
				else if(read_char == '\n')
				{
					current = REG_TEXT;
					putchar('\n');
				}
				else
				{
					current = SINGLE_LINE;
					putchar(read_char);
				}
				break;

			case TAG:
				if(read_char == '\n')
				{
					current = REG_TEXT;
					putchar('\n');
				}
				else if(isspace(read_char))
				{
					current = SINGLE_LINE;
					putchar(read_char);
				}
				else
				{
					current = TAG;
				}
				break;

			case LEADING_TAG:
				if(read_char == '\n')
				{
					current = REG_TEXT;
				}
				else if(isspace(read_char) || read_char == '*')
				{
					current = SINGLE_LINE_WHITESPACE;
				}
				else if(read_char != '\n')
				{
					current = LEADING_TAG;
				}
				break;

			case MULTI_LINE:
				if(multi_line_started == true)
				{
					// first, turn off the started indicator
					multi_line_started = false;

					// if you know the forward slash is next
					if(next_char == '/')
					{
						// and you see an asterisk currently
						if(read_char == '*')
						{
							// just skip a step to the REG_TEXT state
							current = REG_TEXT;
							// and advance the pointer
							read_char = getchar();
							// make sure you don't output a new line
						}
						// but if you don't see an asterisk, but whitespace or tag
						else if(isspace(read_char) || read_char == '@')
						{
							// and you can go to the whitespace state
							current = MULTI_LINE_WHITESPACE;
						}
						else if(read_char == '\n')
						{
							current = MULTI_LINE;
							multi_line_started = true;
							putchar('\n');
						}
						else if(read_char == '\"')
						{
							current = DOUBLE_QUOTE;
							putchar(read_char);
						}
						// for everything else like letters or numbers
						else
						{
							current = MULTI_LINE;
							putchar(read_char);
						}
					}
					else // if forward slash isn't NEXT
					{
						// and you see a space or asterisk or tag, that means its whitespace
						if(isspace(read_char) || read_char == '*')
						{
							current = MULTI_LINE_WHITESPACE;
						}
						else if(read_char == '\n')
						{
							current = MULTI_LINE;
							multi_line_started = true;
							putchar('\n');
						}
						else if(read_char == '@')
						{
							current = MULTI_LEADING_TAG;
						}
						else if(read_char == '\"')
						{
							current = DOUBLE_QUOTE;
							putchar(read_char);
						}
						else
						{
							current = MULTI_LINE;
							putchar(read_char);
						}
					}
				}
				else // if multi_line_started = false
				{
					if(next_char == '/')
					{
						if(read_char == '*')
						{
							// just skip a step to the REG_TEXT state
							current = REG_TEXT;
							// and advance the pointer
							read_char = getchar();
							// and output a new line
							putchar('\n');
						}
						else if(read_char == '\n')
						{
							current = MULTI_LINE;
							multi_line_started = true;
							putchar('\n');
						}
						else if(read_char == '\"')
						{
							current = DOUBLE_QUOTE;
							putchar(read_char);
						}
						else
						{
							current = MULTI_LINE;
							putchar(read_char);
						}
					}
					else // if a forward slash is not the next char
					{
						if(read_char == '\n')
						{
							current = MULTI_LINE;
							multi_line_started = true;
							putchar('\n');
						}
						else if(isspace(read_char) && next_char == '@')
						{
							current = MULTI_LEADING_TAG;
							putchar(read_char);
							read_char = getchar();
						}
						else if(read_char == '\"')
						{
							current = DOUBLE_QUOTE;
							putchar(read_char);
						}
						else
						{
							current = MULTI_LINE;
							putchar(read_char);
						}
					}
				}
				break;

			case MULTI_LINE_WHITESPACE:
				if(next_char == '/')
				{
					if(read_char == '*')
					{
						current = REG_TEXT;
						read_char = getchar();
					}
					else if(read_char == '\n')
					{
						current = MULTI_LINE;
						multi_line_started = true;
					}
					else if(isspace(read_char))
					{
						current = MULTI_LINE_WHITESPACE;
					}
					else if(read_char == '@')
					{
						current = MULTI_LEADING_TAG;
					}
					else if(read_char == '\"')
					{
						current = DOUBLE_QUOTE;
						putchar(read_char);
					}
					else
					{
						current = MULTI_LINE;
						putchar(read_char);
					}
				}
				else // if next char is not forward slash
				{
					if(isspace(read_char) || read_char == '*')
					{
						current = MULTI_LINE_WHITESPACE;
					}
					else if(read_char == '\n')
					{
						current = MULTI_LINE;
						multi_line_started = true;
					}
					else if(read_char == '@')
					{
						current = MULTI_LEADING_TAG;
					}
					else if(read_char == '\"')
					{
						current = DOUBLE_QUOTE;
						putchar(read_char);
					}
					else // regular stuff
					{
						current = MULTI_LINE;
						putchar(read_char);
					}
				}
				break;

			case MULTI_LEADING_TAG:
				if(next_char == '/')
				{
					if(read_char == '*')
					{
						current = REG_TEXT;
						read_char = getchar();
					}
					else if(read_char == '\n')
					{
						current = MULTI_LINE;
						multi_line_started = true;
					}
					else
					{
						current = MULTI_LEADING_TAG;
					}
				}
				else // if next char is not a forward slash
				{
					// if a space, check for the start of another tag
					if(isspace(read_char))
					{
						// if a tag again, loop to the same state
						if(next_char == '@')
						{
							current = MULTI_LEADING_TAG;
							putchar(read_char);
							read_char = getchar();
						}
						// if not a tag, then don't output and go to multi-line state
						else
						{
							current = MULTI_LINE;
						}
					}
					else // if not a space but anything else, consume it
					{
						current = MULTI_LEADING_TAG;
					}
				}
				break;

			case DOUBLE_QUOTE:
				// if you get a backslash escape, go straight to the DOUBLE_ESCAPER
				if(read_char == '\\')
				{
					current = DOUBLE_ESCAPER;
					// but don't output the backslash
				}
				else if(read_char == '\"')
				{
					// the quote is finished
					current = MULTI_LINE;
					putchar(read_char);
				}
				else
				{
					// assume it is inside and part of the quote
					current = DOUBLE_QUOTE;
					putchar(read_char);
				}
				break;

			// escapes pretty much everything
			case DOUBLE_ESCAPER:
				if(read_char)
				{
					current = DOUBLE_QUOTE;
					putchar(read_char);
				}
				break;
		}
	}
	// exit with success
	return 0;
}
