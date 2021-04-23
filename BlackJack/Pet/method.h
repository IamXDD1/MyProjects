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
	cout << endl << "          二十一點" << endl << endl;
	displayEquals(30);
}
void displayOptionsOf(string occasion) {
	if (occasion == "Hub") {
		cout << "        (a)開始新遊戲" << endl;
		cout << "        (b)查看錢包" << endl;
		cout << "        (c)自訂指令" << endl;
		cout << "        (d)--施工中--" << endl;
		cout << "        (e)離開遊戲" << endl;
	}
	else if (occasion == "Game") {
		cout << "     (a)再抽一張牌" << endl;
		cout << "     (b)跳過(結束回合)" << endl;
	}
	displayEquals(30);
	cout << "請輸入：";
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
		cout << "很抱歉，你輸光了你的錢！" << endl;
	}
	cout << "謝謝遊玩，下次再來！" << endl;
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
	cout << endl << "->總點數： " << calculatePoints(deck) << endl;
}
void displayCards(vector <Card> opponentDeck, vector <Card> playerDeck, bool isPassed) {
	displayEquals(30);
	if (!isPassed) {
		cout << "對手的卡牌：" << endl;
		cout << "- " << opponentDeck.at(0).suit << " " << transferSymbol(opponentDeck.at(0).number) << endl;
		if (calculatePoints(opponentDeck) == 21) {
			cout << "- " << opponentDeck.at(1).suit << " " << transferSymbol(opponentDeck.at(1).number) << endl;
			displayPoints(opponentDeck);
		}
		else {
			cout << "- (覆蓋牌)" << endl;
		}
		displayEquals(30);
		cout << "你的卡牌：" << endl;
		for (int i = 0; i < playerDeck.size(); i++) {
			cout << "- " << playerDeck.at(i).suit << " " << transferSymbol(playerDeck.at(i).number) << endl;
		}
		displayPoints(playerDeck);
		displayEquals(30);
	}
	else {
		cout << "對手的卡牌：" << endl;
		for (int i = 0; i < opponentDeck.size(); i++) {
			cout << "- " << opponentDeck.at(i).suit << " " << transferSymbol(opponentDeck.at(i).number) << endl;
		}
		displayPoints(opponentDeck);
		displayEquals(30);
		cout << "你的卡牌：" << endl;
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
			cout << "你與對手同時取得JackPot！" << endl;
			cout << "很遺憾，根據規則，你輸了。" << endl;
			displayEquals(30);
			return 1;
		}
		else if (opponentPoints < 21) {
			cout << "你取得JackPot！" << endl;
			cout << "恭喜，你奇蹟般地贏了！" << endl;
			displayEquals(30);
			return 2;
		}
		else if (opponentPoints > 21) {
			cout << "你取得JackPot！" << endl;
			cout << "而對手爆掉了！" << endl;
			cout << "恭喜，你奇蹟般地贏了！" << endl;
			displayEquals(30);
			return 3;
		}
	}
	else if (playerPoints > 21) {
		if (opponentPoints == 21) {
			cout << "你爆掉了！" << endl;
			cout << "而對手取得JackPot！" << endl;
			cout << "很遺憾，你不幸地輸了。" << endl;
			displayEquals(30);
			return 4;
		}
		else if (opponentPoints < 21) {
			cout << "你爆掉了！" << endl;
			cout << "很遺憾，你輸了。" << endl;
			displayEquals(30);
			return 5;
		}
		else if (opponentPoints > 21) {
			cout << "你與對手同時爆掉了！" << endl;
			cout << "很遺憾，根據規則，你輸了。" << endl;
			displayEquals(30);
			return 6;
		}
	}
	else if (playerPoints < 21) {
		if (opponentPoints == 21) {
			cout << "對手取得JackPot！" << endl;
			cout << "很遺憾，你不幸地輸了。" << endl;
			displayEquals(30);
			return 7;
		}
		else if (opponentPoints < 21) {
			if (isPassed) {
				if (opponentPoints >= playerPoints) {
					cout << "你輸了！" << endl;
					displayEquals(30);
					return 10;
				}
				else {
					cout << "你贏了！" << endl;
					displayEquals(30);
					return 11;
				}
			}
			else
				return 8;
		}
		else if (opponentPoints > 21) {
			cout << "對手爆掉了！" << endl;
			cout << "恭喜，你贏了！" << endl;
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