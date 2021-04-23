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
			cout << "�z�{�������G " << balance << endl;
			cout << "�п�J�A�n�U�`�����B(�̤p�U�` " << minBid << " �B�̤j�U�` " << maxBid << ")�G ";
			while (!(bid >= minBid && bid <= maxBid)) {
				cin >> bid;
				if (!(bid >= minBid && bid <= maxBid)) {
					displayEquals(30);
					cout << "�z��J�����B���A�d�򤺡C" << endl;
					cout << "�z�{�������G " << balance << endl;
					cout << "�ЦA����J(�̤p�U�` " << minBid << " �B�̤j�U�` " << maxBid << ")�G ";
				}
				else if (bid > balance && !isEndless) {
					displayEquals(30);
					cout << "�z�����������I" << endl;
					cout << "�ЦA����J�G ";
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
						cout << "����F�@�i�P�C" << endl;
					}
					//---------------------------------------
				}
				displayCards(opponentDeck, playerDeck, isPassed);
				_case = judgeCase(calculatePoints(playerDeck), calculatePoints(opponentDeck), isPassed);
				//-------------�P�_����-----------------
				if (_case != 8) {
					if (rate(_case) > 0)
						cout << "���� +" << rate(_case) * bid << endl;
					else
						cout << "���� " << rate(_case) * bid << endl;
					displayEquals(30);
					balance += rate(_case) * bid;
					break;
				}
				//-------------------------------------
				if (!isPassed) {
					displayOptionsOf("Game");
					cin >> act;
					if (act == addACard) {
						cout << "�A��F�@�i�P�C" << endl;
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
			cout << "�����G " << balance << endl;
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