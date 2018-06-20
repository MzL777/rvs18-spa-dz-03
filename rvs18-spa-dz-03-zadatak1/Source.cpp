#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
	pair<pair<int, int>, pair<int, int>> p;
	cout << "A redak: ";
	cin >> p.first.first;
	cout << "A stupac: ";
	cin >> p.first.second;

	cout << "B redak: ";
	cin >> p.second.first;
	cout << "B stupac: ";
	cin >> p.second.second;

	pair<int, int> t = p.first;
	do {
		system("cls");
		// Calculate
		if (t.first > p.second.first)--t.first;
		else if (t.first < p.second.first)++t.first;
		else {
			if (t.second > p.second.second)--t.second;
			else if (t.second <= p.second.second)++t.second;
		}
		// Draw
		for (int i = 1; i <= 20; ++i) {
			for (int j = 1; j <= 40; ++j) {
				if (i == t.first && j == t.second) cout << "x";
				else if (i == p.first.first && j == p.first.second) cout << "A";
				else if (i == p.second.first && j == p.second.second) cout << "B";
				else cout << "-";
			}
			cout << endl;
		}
		Sleep(long(100));
	} while (t != p.second);
	return 0;
}