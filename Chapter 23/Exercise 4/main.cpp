// -----------------------------------------------------------------------------
// https://lptcp.blogspot.com/
// 
/*
	Chapter 23 - Exercise 4

	Find a real email message file (containing real email messages) and modify the 
	email example to extract subject lines from sender names taken as input from the user.

*/
// https://github.com/l-paz91/principles-practice/
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <iterator>
#include <map>
#include <regex>

#include "MailFile.h"
#include "std_lib_facilities.h"

// -----------------------------------------------------------------------------

wstring search(const wstring& pStr)
{
	// set input mode to allow unicode text
	_setmode(_fileno(stdin), _O_WTEXT);

	wcout << pStr;
	wstring subject;
	getline(std::wcin, subject);

	_setmode(_fileno(stdin), _O_TEXT);
	// return input mode to normal

	return subject;
}

bool searchAgain()
{
	cout << "Search again? y/n\n>> ";
	char c;
	cin >> c;
	if (c == 'y')
	{
		cin.clear();
		cin.ignore(120, '\n');
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------

bool extractEmailMessagesFromSubject(multimap<wstring, const Message*>& pSubjects)
{
	wstring subject = search(L"Enter a subject line to search for: ");

	auto isPair = pSubjects.equal_range(subject);
	if (isPair.first == isPair.second)
	{
		cout << "No messages found with that subject." << endl;
	}
	else
	{
		for (auto p = isPair.first; p != isPair.second; ++p)
		{
			Message m = *p->second;
			m.printMsg();
		}
	}

	return searchAgain();
}

// -----------------------------------------------------------------------------

bool extractSubjectLines(multimap<wstring, const Message*> pSenders)
{
	wstring senderStr = search(L"Enter a sender to search for: ");
	auto sender = pSenders.equal_range(senderStr);

	for (auto msg = sender.first; msg != sender.second; ++msg)
	{
		wcout << findSubjectUsingRegex(*msg->second) << endl;
	}

	return searchAgain();
}

// -----------------------------------------------------------------------------

void fct()
{
	MailFile mailFile{ "randomEmailMessages.txt" };

	// first gather messages from each sender together in a multimap
	multimap<wstring, const Message*> sender;

	// gather subjects and messages together
	multimap<wstring, const Message*> subjects;

	for (const auto& msg : mailFile)
	{
		wstring s;
		if (FindFromAddrUsingRegex(msg, s))
		{
			sender.insert(make_pair(s, &msg));
		}

		s = findSubjectUsingRegex(msg);
		if (s != L"")
		{
			subjects.insert(make_pair(s, &msg));
		}
	}

	while (extractSubjectLines(sender)) {}
}

// -----------------------------------------------------------------------------

int main()
{
	// done to keep an eye on memory tracking (it will always report as a leak if done in main)
	fct();

	cout << "\n";
	pressKeyToContinue();
	_CrtDumpMemoryLeaks();
	return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
