#include<iostream>
#include"method.h"
#include<Windows.h>
#include<vector>
#include<string>
using namespace std;
int main() {
	srand(time(NULL));
	vector<Card>playerDeck;
	vector<Card>opponentDeck;
	displayIntro();
	while (!isLeaving){
		displayOptionsOf("Hub");
		cin >> act;
		if (act == startNewGame) {
			long long int bid = minBid - 1;
			int _case;
			cout << "您現有金錢： " << balance << endl;
			cout << "請輸入你要下注的金額(最小下注 " << minBid << " 、最大下注 " << maxBid << ")： ";
			while (!(bid >= minBid && bid <= maxBid)) {
				cin >> bid;
				if (!(bid >= minBid && bid <= maxBid)) {
					displayEquals(30);
					cout << "您輸入的金額不再範圍內。" << endl;
					cout << "您現有金錢： " << balance << endl;
					cout << "請再次輸入(最小下注 " << minBid << " 、最大下注 " << maxBid << ")： ";
				}
				else if (bid > balance && !isEndless) {
					displayEquals(30);
					cout << "您的金錢不足！" << endl;
					cout << "請再次輸入： ";
					bid = maxBid + 1;
				}
			}
			playerDeck.clear();
			opponentDeck.clear();
			mainDeck = resetDeck(mainDeck);
			for (int i = 0; i < 2; i++) {
				playerDeck = addCard(playerDeck);
				opponentDeck = addCard(opponentDeck);
			}
			bool isPassed = false;
			while (true) {
				if (isPassed) {
					//-----------------AI-------------------
					while (calculatePoints(opponentDeck) < 16) {
						opponentDeck = addCard(opponentDeck);
						cout << "對手抽了一張牌。" << endl;
					}
					//---------------------------------------
				}
				displayCards(opponentDeck, playerDeck, isPassed);
				_case = judgeCase(calculatePoints(playerDeck), calculatePoints(opponentDeck), isPassed);
				//-------------判斷機制-----------------
				if (_case != 8) {
					if (rate(_case) > 0)
						cout << "金錢 +" << rate(_case) * bid << endl;
					else
						cout << "金錢 " << rate(_case) * bid << endl;
					displayEquals(30);
					balance += rate(_case) * bid;
					break;
				}
				//-------------------------------------
				if (!isPassed) {
					displayOptionsOf("Game");
					cin >> act;
					if (act == addACard) {
						cout << "你抽了一張牌。" << endl;
						playerDeck = addCard(playerDeck);
					}
					else if (act == pass)
						isPassed = true;
				}
			}
		}
		else if (act == leaveGame) {
			displayEnding(isBankrupt);
			break;
		}
		else if (act == checkBalance) {
			displayEquals(30);
			cout << "金錢： " << balance << endl;
			displayEquals(30);
		}
		else if (act == command) {
			string _command;
			cout << "Command: ";
			cin >> _command;
			if (_command == "/getMoney") {
				balance = 9999999;
				displayEquals(30);
			}
			else if (_command == "/enableEndlessGame") {
				isEndless = true;
				displayEquals(30);
			}
			else if (_command == "/disableEndlessGame") {
				isEndless = false;
				displayEquals(30);
			}
			else if (_command == "/changeMinBid") {
				int tmp;
				cin >> tmp;
				minBid = tmp;
				displayEquals(30);
			}
			else if (_command == "/changeMaxBid") {
				int tmp;
				cin >> tmp;
				maxBid = tmp;
				displayEquals(30);
			}
		}
		if (balance < 10 && !isEndless) {
			isBankrupt = true;
			displayEnding(isBankrupt);
			break;
		}
	}
	return 0;
}