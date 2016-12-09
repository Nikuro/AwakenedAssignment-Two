// TextAdventure.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <ctime>

using namespace std;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

/* when the player makes game decisions with the arrow keys,
   the choices need to be able to loop around. like...
   if you have choice 2, and try to go higher,
   it'll loop back to 0.
   i would accomplish this using the % operator,
   but it doesn't play nice with negatives,
   like if you went from 0 -> -1, it won't loop back to 2.
   this is the solution :P
*/
int mod(int a, int b)
{
	if (b < 0)
		return mod(a, -b);
	int ret = a % b;
	if (ret < 0)
		ret += b;
	return ret;
}


// typeOut prototype
void typeOut(const string& message, long msPerCharacter, bool shouldContinue);
// askQuestion prototype
void askQuestion(int questionId, int selectedChoice, bool doAnimate, string choices[3]);
// handleQuestionResponse prototype
void handleQuestionResponse(int questionId, int selectedChoice);


// slow typing logic
void typeOut(const string& message, long msPerCharacter, bool shouldContinue)
{
	for (int count = 0; count <= message.length()-1; count++)
	{
		//if input is detected before message is complete, including while sleeping, then
		// finish outputting the message instantly. it's a little hacky.
		while (_kbhit())
		{
			msPerCharacter = 0;
			_getch();
		}
		cout << message.at(count) << flush;
		if (count == message.length()-1)
		{
			if (shouldContinue == false)
			{
				cout << "<press a key to advance>\n";
				int ch = _getch();
				// the following code prevents an arrow key press from triggering getch twice
				if (ch == 0 || ch == 224) // if the first value is esc
				{
					switch (_getch()) // the real value
					{
					
					// in case i ever need to, i have the individual cases here
					// it hasn't proven to be useful yet though
					case 75:
						break;
					case 77:
						//pass for now
						break;
					case 72:
						//pass for now
						break;
					case 80:
						//pass for now
						break;
					}
				}
				system("cls");
				break;
			}
			else
			{
				//this might become useful later
			}

		}
		sleep_for(milliseconds(msPerCharacter));
	}
}

// askQuestion logic
// types out user's options and lets them choose
void askQuestion(int questionId, int selectedChoice, bool doAnimate, string choices[3])
{
	system("color 0f");
	// this will reset the text cursor to the top left
	// i do this instead of system("cls")
	// because it negates the redraw of the screen
	// with system("cls") the screen would first go blank, then redraw
	// which lead to some gross flickering
	// this is my solution
	COORD coordinate;
	coordinate.X = 0;
	coordinate.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);

	string newChoices[3];
	// using a for loop leaves room for adding more than three choices, if i ever want to
	// im still not satisfied with how i do this, but for now it works
	for (int x = 0; x < 3; x++)
	{
		if (x == selectedChoice)
		{
			newChoices[x] = " ->  " + choices[x] + "\n";
		}
		else
		{
			newChoices[x] = " -   " + choices[x] + "\n";
		}
	}

	// we will animate if the question is being asked for the first time.
	if (doAnimate)
	{
		typeOut(newChoices[0], 12, true);
		typeOut(newChoices[1], 12, true);
		typeOut(newChoices[2], 12, true);
	}
	else
	{
		cout << newChoices[0];
		cout << newChoices[1];
		cout << newChoices[2];
	}
	// this code is something i found online.
	// it works like this.
	// since one arrow key sends two different values...
	// we can just check each one individually!
	// an up arrow would look like these three
	// (0 || 224), 72

	bool needChoice = true;
	while (needChoice)
	{
		int ch = _getch();
		if (ch == 0 || ch == 224) // if the first value is esc
		{
			switch (_getch()) // the real value
			{
			case 72:
				// arrow up
				needChoice = false;
				// use modulo to wrap choice around if it goes past the third choice
				selectedChoice = mod(selectedChoice - 1, 3);
				// call self (i think this is called recursion?) to re-print the choices, with the updated selection
				askQuestion(questionId, selectedChoice, false, choices);
				break;
			case 80:
				// arrow down
				needChoice = false;
				selectedChoice = mod(selectedChoice + 1, 3);
				askQuestion(questionId, selectedChoice, false, choices);
				break;
			}
		}
		else if (ch == 13)
		{
			//selectedChoice is finalized here
			needChoice = false;
			system("cls");
			handleQuestionResponse(questionId, selectedChoice);

		}
	}
}

// handleQuestionResponse logic
void handleQuestionResponse(int questionId, int selectedChoice)
{
	// At the moment, the response for each selectedChoice per questionId is hard coded.
	// I actually thought about storing questions, answers, etc in a json format.
	// It could eventually lead to a Choose-Your-Own-adventure creator :P
	// Sorry, that's mostly irrelevant. Anyway, yeah, this is hard coded for now.
	// It's a mess. Whoops.

	// What do you call out?
	if (questionId == 0)
	{
		// Purple appears!
		system("color 0f");
		typeOut("[A crashing sound resonates through the entire room.]\n", 55, true);
		typeOut("[The lights turn back on.]\n", 55, true);
		typeOut("[A person is sprawled out on the floor.]\n", 55, true);
		typeOut("[Groaning, he says...]\n", 55, false);
		system("color 0d");
		if (selectedChoice == 0)
		{
			//Let me leave!
			typeOut("Kid, that's not something I can do.\n", 45, true);
			typeOut("I'd love to help you, but...\n", 45, true);
			typeOut("I'm a little tied up right now.\n", 45, false);
			system("color 0f");
			typeOut("[The stranger reveals his hands, which are bound together.]\n", 45, false);
			system("color 0d");
			typeOut("Care to help out a fellow prisoner?\n", 55, false);
			system("color 0f");
			string choices[3] = { "Alright, I'll help.", "Do you know what's going on?", "Why should I trust you?" };
			askQuestion(1, 0, true, choices);
		}
		else if (selectedChoice == 1)
		{
			//Is anyone there?
			typeOut("Well... I'm here.\n", 50, true);
			typeOut("And I'm in a bit of a situation.\n", 55, true);
			typeOut("I'll tell you what I know, but first...\n", 55, false);
			system("color 0f");
			typeOut("[The stranger reveals his hands, which are bound together.]\n", 45, false);
			system("color 0d");
			typeOut("I could use a little assistance.\n", 45, true);
			typeOut("Mind giving me some help?\n", 45, false);
			system("color 0f");
			string choices[3] = { "Alright, I'll help.", "What is this place?", "Why should I trust you?" };
			askQuestion(2, 0, true, choices);
		}
		else if (selectedChoice == 2)
		{
			//That's not scary to me.
			typeOut("Oh, please.\n", 45, true);
			typeOut("Save the Shakespeare for later.\n\n", 45, true);
			typeOut("...That really wasn't scary for you?\n", 55, true);
			typeOut("But... I found it scary...\n", 45, false);
			system("color 0f");
			typeOut("[You catch the newcomer staring at your hands.]\n", 45, true);
			typeOut("[Immediately, the face of concern leaves the stranger...]\n", 45, true);
			typeOut("[...It's replaced by a glare of suspicion.]\n", 45, false);
			system("color 0d");
			typeOut("They didn't tie you up?\n", 45, false);
			system("color 0f");
			string choices[3] = { "No, why?", "What are you talking about?", "Who is \"they?\"" };
			askQuestion(3, 0, true, choices);
		}
	}
	// Care to help a fellow prisoner?
	else if (questionId == 1)
	{
		if (selectedChoice == 0) // Alright, I'll help.
		{
			system("color 0d");
			typeOut("Thank goodness.\n\n", 50, true);
			typeOut("..........\n", 50, true);
			typeOut("You know, you should be more careful.\n", 45, true);
			typeOut("You're lucky that I'm not with them.\n", 45, true);
			typeOut("Or at least, that I'm not with them anymore.\n", 45, false);
			typeOut("You're currently standing in Silenced Sound laboratories.\n", 50, true);
			typeOut("Specifically, their research and development facility.\n", 50, true);
			typeOut("Building # A1.\n", 50, true);
			typeOut("I don't know what you did to these guys, but...\n", 50, true);
			typeOut("If you've landed in # A1 without any idea of what's going on...\n", 50, true);
			typeOut("You must've made 'em mad. What did you do?\n", 50, true);
			typeOut("Wait-wait-wait, before you answer that...\n", 50, true);
			typeOut("You should probably know who I am.\n", 50, true);
			typeOut("I'm the founder and former CEO of Silenced Sound.\n", 50, true);
			typeOut("And I quit my job to save people like you.\n", 50, false);
			// ^^ noble enough? jackass.
			typeOut("I even went ahead and deci-\n", 40, true);
			typeOut("Wait... you're not tied up.\n", 50, true);
			typeOut("They didn't tie you up?\n", 60, false);
			string choices[3] = { "No, why?", "What are you talking about?", "Who is \"they?\"" };
			askQuestion(4, 0, true, choices);
		}
		else if (selectedChoice == 1) // Do you know what's going on?
		{
			system("color 0d");
			typeOut("I know some of what's going on...\n", 50, true);
			typeOut("But I don't know enough.\n", 50, true);
			typeOut("I worked here, a long time ago.\n", 50, false);
			typeOut("You're currently standing in Silenced Sound laboratories.\n", 50, true);
			typeOut("Specifically, their research and development facility.\n", 50, true);
			typeOut("Building # A1.\n", 50, true);
			typeOut("I don't know what you did to these guys, but...\n", 50, true);
			typeOut("If you've landed # in A1 without any idea of what's going on...\n", 50, true);
			typeOut("You must've made 'em mad. What did you do?\n", 50, true);
			typeOut("Wait-wait-wait, before you answer that...\n", 50, true);
			typeOut("You should probably know who I am.\n", 50, true);
			typeOut("I'm the founder and former CEO of Silenced Sound.\n", 50, true);
			typeOut("And I quit my job to save people like you.\n", 50, false);
			// ^^ noble enough? jackass.
			typeOut("I even went ahead and deci-\n", 40, true);
			typeOut("Wait... you weren't tied up.\n", 50, true);
			typeOut("They didn't tie you up?\n", 60, false);
			string choices[3] = { "No, why?", "What are you talking about?", "Who is \"they?\"" };
			askQuestion(4, 0, true, choices);
		}
		else if (selectedChoice == 2) // Why should I trust you?
		{
			system("color 0d");
			typeOut("...why am I here... why did I do this...\r", 35, true);
			// overwrite the vague "why am i here" dialogue
			typeOut("                                        \r", 60, true);
			typeOut("Anyway, I'll tell you exactly why you should trust me.\n", 50, false);
			system("color 0f");
			typeOut("[Even with bound hands, the person still manages to stand up straight.]\n", 45, true);
			typeOut("[With a hard stare, the stranger warns you...]\n", 45, false);
			system("color 0d");
			typeOut("You should trust me because you don't have another choice.\n", 50, true);
			typeOut("This place is called Silenced Sound.\n", 50, true);
			typeOut("You aren't in the greatest situation right now.\n\n", 50, true);
			typeOut("I'm here to get you out.\n", 50, true);
			typeOut("I have a disguise for you. I hid it away before...\n", 50, true);
			typeOut("Before I let myself get captured.\n\n", 50, true);
			typeOut("Oh, you should know that I'm already in a disguise.\n", 50, true);
			typeOut("I don't really look like this at all.\n", 50, true);
			typeOut("If I arrived here with my true appearance...\n", 50, true);
			typeOut("I probably wouldn't have been captured at all.\n", 50, true);
			typeOut("Rather, I'd have been welcomed back.\n", 50, true);
			typeOut("With open arms, most likely.\n\n", 50, true);
			typeOut("...Which is how it should be!\n", 50, true);
			typeOut("I did build this place, after all!\n", 50, false);
			typeOut("I'm the founder and former CEO of Silenced Sound.\n", 50, true);
			typeOut("And I quit my job to save people like you.\n", 50, false);
			// ^^ noble enough? jackass.
			typeOut("I even went ahead and deci-\n", 40, true);
			typeOut("Wait... you weren't tied up.\n", 50, true);
			typeOut("They didn't tie you up?\n", 60, false);
			string choices[3] = { "No, why?", "What are you talking about?", "Who is \"they?\"" };
			askQuestion(4, 0, true, choices);
		}
	}
	// Mind giving me some help?
	else if (questionId == 2)
	{
		if (selectedChoice == 0) // Alright, I'll help.
		{
			system("color 0d");
			typeOut("Thank goodness. My hands are free.\n\n", 50, true);
			typeOut("..........\n", 50, true);
			typeOut("You know, you should be more careful.\n", 45, true);
			typeOut("You're lucky that I'm not with them.\n", 45, true);
			typeOut("Or at least, that I'm not with them anymore.\n", 45, false);
			typeOut("I used to work with these these morons.\n", 45, true);
			typeOut("Right now, you're in Silenced Sound laboratories.\n", 50, true);
			typeOut("Specifically, their research and development facility.\n", 50, true);
			typeOut("Building # A1.\n", 50, true);
			typeOut("I don't know what you did to these guys, but...\n", 50, true);
			typeOut("If you've landed in # A1 without any idea of what's going on...\n", 50, true);
			typeOut("You must've made 'em mad. What did you do?\n", 50, true);
			typeOut("Wait-wait-wait, before you answer that...\n", 50, true);
			typeOut("You should probably know what I did here.\n", 50, true);
			typeOut("I'm the founder and former CEO of this Silenced Sound place.\n", 50, true);
			typeOut("And I quit my job to save people like you.\n", 50, false);
			// ^^ noble enough? jackass.
			typeOut("I even went ahead and deci-\n", 40, true);
			typeOut("Wait... you weren't tied up.\n", 50, true);
			typeOut("They didn't tie you up?\n", 60, false);
			string choices[3] = { "No, why?", "What are you talking about?", "Who is \"they?\"" };
			askQuestion(4, 0, true, choices);
		}
		else if (selectedChoice == 1) //What is this place?
		{
			system("color 0d");
			typeOut("Well, you're currently standing in Silenced Sound laboratories.\n", 50, true);
			typeOut("Specifically, their research and development facility.\n", 50, true);
			typeOut("Building # A1.\n", 50, true);
			typeOut("I don't know what you did to these guys, but...\n", 50, true);
			typeOut("If you've landed in # A1 without any idea of what's going on...\n", 50, true);
			typeOut("You must've made 'em mad. What did you do?\n", 50, true);
			typeOut("Wait-wait-wait, before you answer that...\n", 50, true);
			typeOut("You should probably know who I am.\n", 50, false);
			typeOut("I'm the founder and former CEO of Silenced Sound.\n", 50, true);
			typeOut("And I quit my job to save people like you.\n", 50, false);
			// ^^ noble enough? jackass.
			typeOut("I even went ahead and deci-\n", 40, true);
			typeOut("Wait... you weren't tied up.\n", 50, true);
			typeOut("They didn't tie you up?\n", 60, false);
			string choices[3] = { "No, why?", "What are you talking about?", "Who is \"they?\"" };
			askQuestion(4, 0, true, choices);
		}
		else if (selectedChoice == 2) // Why should I trust you?
		{
			system("color 0d");
			typeOut("...why am I here... why did I do this...\r", 35, true);
			// overwrite the vague "why am i here" dialogue
			typeOut("                                        \r", 60, true);
			typeOut("Anyway, I'll tell you exactly why you should trust me.\n", 50, false);
			system("color 0f");
			typeOut("[Even with bound hands, the person still manages to stand up straight.]\n", 45, true);
			typeOut("[With a hard stare, the stranger warns you...]\n", 45, false);
			system("color 0d");
			typeOut("You should trust me because you don't have another choice.\n", 50, true);
			typeOut("This place is called Silenced Sound.\n", 50, true);
			typeOut("You aren't in the greatest situation right now.\n\n", 50, true);
			typeOut("I'm here to get you out.\n", 50, true);
			typeOut("I have a disguise for you. I hid it away before...\n", 50, true);
			typeOut("Before I let myself get captured.\n\n", 50, true);
			typeOut("Oh, you should know that I'm already in a disguise.\n", 50, true);
			typeOut("I don't really look like this at all.\n", 50, true);
			typeOut("If I arrived here with my true appearance...\n", 50, true);
			typeOut("I probably wouldn't have been captured at all.\n", 50, true);
			typeOut("Rather, I'd have been welcomed back.\n", 50, true);
			typeOut("With open arms, most likely.\n\n", 50, true);
			typeOut("...Which is how it should be!\n", 50, true);
			typeOut("I did build this place, after all!\n", 50, false);
			typeOut("I'm the founder and former CEO of Silenced Sound.\n", 50, true);
			typeOut("And I quit my job to save people like you.\n", 50, false);
			// ^^ noble enough? jackass.
			typeOut("I even went ahead and deci-\n", 40, true);
			typeOut("Wait... you weren't tied up.\n", 50, true);
			typeOut("They didn't tie you up?\n", 60, false);
			string choices[3] = { "No, why?", "What are you talking about?", "Who is \"they?\"" };
			askQuestion(4, 0, true, choices);
		}
	}
	// They didn't tie you up?
	else if (questionId == 3)
	{
		if (selectedChoice == 0) //	No, why?
		{
			system("color 0d");
			typeOut("Simply bizarre. You're the first one they haven't tied up...\n", 50, true);
			typeOut("...and yet, you're in Building # A1.\n", 50, true);
			typeOut("What exactly did you do?\n", 50, false);
			typeOut("If it was severe enough to land you in # A1...\n", 50, false);
			system("color 0f");
			typeOut("[The stranger reveals his hands, which are bound together.]\n", 45, false);
			system("color 0d");
			typeOut("Why didn't they tie you up?\n", 50, true);
			typeOut("...Oh no. Maybe... They're on to me? Already?\n", 50, true);
			typeOut("That can't be right. That's impossible.\n", 50, true);
			typeOut("That's...\n", 65, false);
			system("color 0c");
			typeOut("[CONNECTION TERMINATED]\n", 35, false);
		}
		else if (selectedChoice == 1) //What are you talking about?
		{
			system("color 0d");
			typeOut("Simply bizarre. You're the first one they haven't tied up...\n", 50, true);
			typeOut("...and yet, you're in Building # A1.\n", 50, true);
			typeOut("What exactly did you do?\n", 50, false);
			typeOut("If it was severe enough to land you in # A1...\n", 50, false);
			system("color 0f");
			typeOut("[The stranger reveals his hands, which are bound together.]\n", 45, false);
			system("color 0d");
			typeOut("Why didn't they tie you up?\n", 50, true);
			typeOut("...Oh no. They're on to me? Already?\n", 50, true);
			typeOut("That can't be right. That's impossible.\n", 50, true);
			typeOut("That's...\n", 65, false);
			system("color 0c");
			typeOut("[CONNECTION TERMINATED]\n", 35, false);
		}
		else if (selectedChoice == 2) //Who is "they?"
		{
			system("color 0d");
			typeOut("\"They\" are the folks that made this place.\n", 50, true);
			typeOut("And \"this place\" is Silenced Sound laboratories.\n", 50, true);
			typeOut("You're in their research and development facility.\n", 50, true);
			typeOut("Building # A1.\n", 50, true);
			typeOut("Why are you in Building # A1?\n", 50, false);
			typeOut("What exactly did you do?\n", 50, true);
			typeOut("If it was severe enough to land you in # A1...\n", 50, false);
			system("color 0f");
			typeOut("[The stranger reveals his hands, which are bound together.]\n", 45, false);
			system("color 0d");
			typeOut("Why didn't they tie you up?\n", 50, true);
			typeOut("...Oh no. They're on to me? Already?\n", 50, true);
			typeOut("That can't be right. That's impossible.\n", 50, true);
			typeOut("That's...\n", 65, false);
			system("color 0c");
			typeOut("[CONNECTION TERMINATED]\n", 35, false);
		}
	}
	// They didn't tie you up? [hands already revealed]
	else if (questionId == 4)
	{
		if (selectedChoice == 0) //	No, why?
		{
			system("color 0d");
			typeOut("Simply bizarre. You're the first one they haven't tied up...\n", 50, true);
			typeOut("...and yet, you're in Building # A1.\n", 50, true);
			typeOut("What exactly did you do?\n", 50, false);
			typeOut("If it was severe enough to land you in # A1...\n", 50, false);
			typeOut("Why didn't they tie you up?\n", 50, false);
			typeOut("...Oh no. Maybe... They're on to me? Already?\n", 50, true);
			typeOut("That can't be right. That's impossible.\n", 50, true);
			typeOut("That's...\n", 65, false);
			system("color 0c");
			typeOut("[CONNECTION TERMINATED]\n", 35, false);
		}
		else if (selectedChoice == 1) //What are you talking about?
		{
			system("color 0d");
			typeOut("Simply bizarre. You're the first one they haven't tied up...\n", 50, true);
			typeOut("...and yet, you're in Building # A1.\n", 50, true);
			typeOut("What exactly did you do?\n", 50, false);
			typeOut("If it was severe enough to land you in # A1...\n", 50, false);
			typeOut("Why didn't they tie you up?\n", 50, false);
			typeOut("...Oh no. They're on to me? Already?\n", 50, true);
			typeOut("That can't be right. That's impossible.\n", 50, true);
			typeOut("That's...\n", 65, false);
			system("color 0c");
			typeOut("[CONNECTION TERMINATED]\n", 35, false);
		}
		else if (selectedChoice == 2) //Who is "they?"
		{
			system("color 0d");
			typeOut("\"They\" are the folks that made this place.\n", 50, true);
			typeOut("My old co-workers.\n", 50, true);
			typeOut("Hey, what exactly did you do?\n", 50, true);
			typeOut("If it was severe enough to land you in # A1...\n", 50, false);
			typeOut("Why didn't they tie you up?\n", 50, false);
			typeOut("...Oh no. They're on to me? Already?\n", 50, true);
			typeOut("That can't be right. That's impossible.\n", 50, true);
			typeOut("That's...\n", 65, false);
			system("color 0c");
			typeOut("[CONNECTION TERMINATED]\n", 35, false);
		}
	}
}

int main()
{
	// NOTE: There are some double line endings in here for formatting purposes.
	// Tons of dialogue here. Apologies to whoever is reviewing the code, ha
	typeOut("<TUTORIAL>\n\n", 60, true);
	typeOut("<You can skip the typing animation>\n", 60, true);
	typeOut("<by pressing any key>\n\n", 60, false);
	system("cls");
	system("color 0c");
	cout << "<TUTORIAL>\n\n";
	typeOut("<[RED] text is evil. Watch out for it.>\n", 60, false);
	system("cls");
	system("color 0b");
	cout << "<TUTORIAL>\n\n";
	typeOut("<[BLUE] text is good. You can trust it.>\n", 60, false);
	system("cls");
	system("color 0f");
	cout << "<TUTORIAL>\n\n";
	typeOut("<[WHITE] text is observation. Always think out loud.>\n", 60, false);
	system("cls");
	system("color 0d");
	cout << "<TUTORIAL>\n\n";
	typeOut("<[PURPLE] text is unknown. Keep an open mind.>\n", 60, false);
	system("cls");
	system("color 07");
	typeOut("[Two truths and a lie]\n\n", 70, true);
	typeOut("<press a key to begin your journey>\n", 60, true);
	_getch();
	system("cls");
	system("color 0c");
	typeOut("[You hear a distant voice.]\n", 80, true);
	typeOut("Is the prisoner finally ready to talk?\n\n", 70, true);
	typeOut("[Another voice replies...]\n", 70, true);
	typeOut("No... but it seems like the punk is finally waking up.\n\n", 80, true);
	typeOut("[The first voice quickly asks...]\n", 50, true);
	typeOut("How do you know?\n\n", 90, true);
	typeOut("[The second voice responds with confidence.]\n", 80, true);
	typeOut("Because the jailbird's listening in on us right now.\n\n", 80, false);
	system("color 0f");
	typeOut("[The voices stop. You open your eyes.]\n\n", 50, true);
	typeOut("[You try to take in your surroundings, but...]\n", 50, true);
	typeOut("[You quickly realize that there is nothing to see. It's pitch black.]\n\n", 50, false);
	system("color 0c");
	typeOut("[Suddenly, you're being grabbed.]\n", 50, true);
	typeOut("[You're pushed forwards by an unseen force.]\n\n", 50, true);
	typeOut("[Unable to fight back in the dark...]\n", 50, true);
	typeOut("[You don't waste any energy trying to escape.]\n\n", 50, false);
	system("color 0f");
	typeOut("[You're shoved into a very small room. It's empty.]\n\n", 50, true);
	typeOut("[The place may have a little bit of dim lighting, but...]\n", 50, true);
	typeOut("[For some reason, you still miss the darkness of the room you just left.]\n\n", 50, false);
	typeOut("[You wait for something to happen. Anything...]\n", 50, true);
	typeOut("[...]\n", 60, true);
	typeOut("[...]\n", 60, true);
	typeOut("[...]\n", 60, false);
	typeOut("[The lights flicker a few times.]\n", 50, true);
	typeOut("[As if someone had read your thoughts, the lights go out.]\n\n", 50, true);
	typeOut("[You are submerged in the shadowy blackness once again.]\n", 50, true);
	typeOut("[After a few seconds, you call out to the darkness...]\n\n\n", 50, false);
	string questionOne[3] = { "Let me leave.", "Is anyone there?", "That's not scary to me." };
	askQuestion(0, 0, true, questionOne);
	// after asking a question, there should be a way to get a value back here.
	// since askQuestion() is recursive, i don't think i can get a return value when someone answers it, though.
	// if i do int response = askQuestion(), the logic might wind up looking like this:
	// response = askQuestion() = askQuestion() = askQuestion... etc
	// and i don't like how that would turn out. might not work at all :P
	// for now, and for the sake of time, my solution is: within the askQuestion function, call another function, with the questionId as a parameter
	
	// anyway, if we've gotten here, every available question has been exhausted. end the journey now.
	cout << "<press a key to end your journey>\n";
	_getch();
	return 0;
}