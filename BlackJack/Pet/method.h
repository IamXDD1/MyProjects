#pragma once
#include<ctime>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
//ints
long long int maxBid = 100000;
long long int minBid = 10;
long long int balance = 2000;
//bools
bool isLeaving = false;
bool isBankrupt = false;
bool isEndless = false;
char act;
//consts
char startNewGame = 'a';
char addACard = 'a';
char pass = 'b';
char leaveGame = 'e';
char checkBalance = 'b';
char command = 'c';
//const(end)
struct Card {
	string suit;
	int number;
	Card(string suit, int number) {
		this->suit = suit;
		this->number = number;
	}
};
vector <Card> mainDeck;
vector <Card> resetDeck(vector<Card> deck) {
	for (int i = 1; i <= 13; i++)
		deck.push_back(Card("Spade", i));
	for (int i = 1; i <= 13; i++)
		deck.push_back(Card("Club", i));
	for (int i = 1; i <= 13; i++)
		deck.push_back(Card("Heart", i));
	for (int i = 1; i <= 13; i++)
		deck.push_back(Card("Diamond", i));
	return deck;
}
int generateRandom(int size) {
	return rand() % size;
}
void displayEquals(int n) {
	for (int i = 0; i < n; i++) {
		cout << "=";
		Sleep(10);
	}
	cout << endl;
}
void displayIntro() {
	displayEquals(30);
	cout << endl << "          �G�Q�@�I" << endl << endl;
	displayEquals(30);
}
void displayOptionsOf(string occasion) {
	if (occasion == "Hub") {
		cout << "        (a)�}�l�s�C��" << endl;
		cout << "        (b)�d�ݿ��]" << endl;
		cout << "        (c)�ۭq���O" << endl;
		cout << "        (d)--�I�u��--" << endl;
		cout << "        (e)���}�C��" << endl;
	}
	else if (occasion == "Game") {
		cout << "     (a)�A��@�i�P" << endl;
		cout << "     (b)���L(�����^�X)" << endl;
	}
	displayEquals(30);
	cout << "�п�J�G";
}
bool compare(Card card1, Card card2) {
	return (card1.number > card2.number);
}
vector <Card> sortDeck(vector <Card> deck) {
	sort(deck.begin(), deck.end(), compare);
	return deck;
}
vector <Card> addCard(vector<Card>deck) {
	int random;
	random = generateRandom(mainDeck.size());
	deck.push_back(mainDeck.at(random));
	mainDeck.erase(mainDeck.begin() + random);
	deck = sortDeck(deck);
	return deck;
}
void displayEnding(bool isBankrupt) {
	if (isBankrupt) {
		cout << "�ܩ�p�A�A����F�A�����I" << endl;
	}
	cout << "���¹C���A�U���A�ӡI" << endl;
}
string transferSymbol(int number) {
	string str_Temp = "";
	switch (number) {
		case  1: str_Temp += 'A'; break;
		case 11: str_Temp += 'J'; break;
		case 12: str_Temp += 'Q'; break;
		case 13: str_Temp += 'K'; break;
		case 10: str_Temp += "10"; break;
		default: str_Temp += static_cast<char>(number + '0');
	}
	return str_Temp;
}
int calculatePoints(vector <Card> deck) {
	int points = 0;
	for (int i = 0; i < deck.size(); i++) {
		if (deck.at(i).number == 1) {
			if (points < 11)
				points += 11;
			else
				points++;
		}
		else if (deck.at(i).number > 10) {
			points += 10;
		}
		else {
			points += deck.at(i).number;
		}
	}
	return points;
}
void displayPoints(vector <Card> deck) {
	cout << endl << "->�`�I�ơG " << calculatePoints(deck) << endl;
}
void displayCards(vector <Card> opponentDeck, vector <Card> playerDeck, bool isPassed) {
	displayEquals(30);
	if (!isPassed) {
		cout << "��⪺�d�P�G" << endl;
		cout << "- " << opponentDeck.at(0).suit << " " << transferSymbol(opponentDeck.at(0).number) << endl;
		if (calculatePoints(opponentDeck) == 21) {
			cout << "- " << opponentDeck.at(1).suit << " " << transferSymbol(opponentDeck.at(1).number) << endl;
			displayPoints(opponentDeck);
		}
		else {
			cout << "- (�л\�P)" << endl;
		}
		displayEquals(30);
		cout << "�A���d�P�G" << endl;
		for (int i = 0; i < playerDeck.size(); i++) {
			cout << "- " << playerDeck.at(i).suit << " " << transferSymbol(playerDeck.at(i).number) << endl;
		}
		displayPoints(playerDeck);
		displayEquals(30);
	}
	else {
		cout << "��⪺�d�P�G" << endl;
		for (int i = 0; i < opponentDeck.size(); i++) {
			cout << "- " << opponentDeck.at(i).suit << " " << transferSymbol(opponentDeck.at(i).number) << endl;
		}
		displayPoints(opponentDeck);
		displayEquals(30);
		cout << "�A���d�P�G" << endl;
		for (int i = 0; i < playerDeck.size(); i++) {
			cout << "- " << playerDeck.at(i).suit << " " << transferSymbol(playerDeck.at(i).number) << endl;
		}
		displayPoints(playerDeck);
		displayEquals(30);
	}
}
int judgeCase(int playerPoints, int opponentPoints, bool isPassed) {
	if (playerPoints == 21) {
		if (opponentPoints == 21) {
			cout << "�A�P���P�ɨ��oJackPot�I" << endl;
			cout << "�ܿ�ѡA�ھڳW�h�A�A��F�C" << endl;
			displayEquals(30);
			return 1;
		}
		else if (opponentPoints < 21) {
			cout << "�A���oJackPot�I" << endl;
			cout << "���ߡA�A�_�ݯ�aĹ�F�I" << endl;
			displayEquals(30);
			return 2;
		}
		else if (opponentPoints > 21) {
			cout << "�A���oJackPot�I" << endl;
			cout << "�ӹ���z���F�I" << endl;
			cout << "���ߡA�A�_�ݯ�aĹ�F�I" << endl;
			displayEquals(30);
			return 3;
		}
	}
	else if (playerPoints > 21) {
		if (opponentPoints == 21) {
			cout << "�A�z���F�I" << endl;
			cout << "�ӹ����oJackPot�I" << endl;
			cout << "�ܿ�ѡA�A�����a��F�C" << endl;
			displayEquals(30);
			return 4;
		}
		else if (opponentPoints < 21) {
			cout << "�A�z���F�I" << endl;
			cout << "�ܿ�ѡA�A��F�C" << endl;
			displayEquals(30);
			return 5;
		}
		else if (opponentPoints > 21) {
			cout << "�A�P���P���z���F�I" << endl;
			cout << "�ܿ�ѡA�ھڳW�h�A�A��F�C" << endl;
			displayEquals(30);
			return 6;
		}
	}
	else if (playerPoints < 21) {
		if (opponentPoints == 21) {
			cout << "�����oJackPot�I" << endl;
			cout << "�ܿ�ѡA�A�����a��F�C" << endl;
			displayEquals(30);
			return 7;
		}
		else if (opponentPoints < 21) {
			if (isPassed) {
				if (opponentPoints >= playerPoints) {
					cout << "�A��F�I" << endl;
					displayEquals(30);
					return 10;
				}
				else {
					cout << "�AĹ�F�I" << endl;
					displayEquals(30);
					return 11;
				}
			}
			else
				return 8;
		}
		else if (opponentPoints > 21) {
			cout << "����z���F�I" << endl;
			cout << "���ߡA�AĹ�F�I" << endl;
			displayEquals(30);
			return 9;
		}
	}
}
int rate(int _case) {
	switch (_case) {
	case 1: return -1; break;
	case 2: return 4; break;
	case 3: return 8; break;
	case 4: return -8; break;
	case 5: return -4; break;
	case 6: return -1; break;
	case 7: return -4; break;
	case 9: return 4; break;
	case 10: return -2; break;
	case 11: return 2; break;
	}
}