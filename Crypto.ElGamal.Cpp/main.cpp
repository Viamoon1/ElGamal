#include "stdafx.h"
#include "ElGamal.h"
const char ESC = 27;
const char UP = 72;
const char DOWN = 80;
const char ENTER = 13;


using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos;
void setConsoleFontSize(int fontSize) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
	cfi.dwFontSize.Y = fontSize;
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}
void disableConsoleResize() {
	HWND consoleWindow = GetConsoleWindow(); // Отримання вікна консолі
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE); // Отримання поточного стилю вікна
	style &= ~WS_SIZEBOX; // Вимкнення можливості зміни розміру
	SetWindowLong(consoleWindow, GWL_STYLE, style); // Застосування змін

}

void showKeys(const pair<std::pair<Int, Group>, Int>& keys) {
	const auto& publicKey = keys.first;
	const auto& privateKey = keys.second;
	cout << "Public Key:\n";
	cout << "p: " << publicKey.second.first << "\n";
	cout << "g: " << publicKey.second.second << "\n";
	cout << "y: " << publicKey.first << "\n";

	cout << "Private Key:\n";
	cout << "x: " << privateKey << "\n";
}
vector<pair<Int, Int>> readPairsFromFile(const string& filename) {
	vector<pair<Int, Int>> pairs; 

	ifstream file(filename); 

	// Перевірка, чи файл відкрився успішно
	if (!file.is_open()) {
		throw runtime_error("Could not open file for reading");
	}
	string line;
	while (getline(file, line)) { // Читання рядків з файлу
		istringstream iss(line); // Рядок вводу
		Int first, second; // Пара чисел

		// Читання пари з рядка
		if (iss >> first >> second) {
			pairs.push_back(make_pair(first, second));
		}
		else {
			cout << ("Error read ");
		}
	}
	file.close(); 

	return pairs;
}

void saveKeysToFile(const pair<std::pair<Int, Group>, Int>& keys, const std::string& filename) {
	std::ofstream file(filename);
	// Перевірка, чи файл відкрився успішно
	if (!file.is_open()) {
		cout << "Could not open file for writing";
	}
	else {
		// Запис ключів у файл
		const auto& publicKey = keys.first;
		const auto& privateKey = keys.second;

		file << "Public Key:\n";
		file << "p: " << publicKey.second.first << "\n";
		file << "g: " << publicKey.second.second << "\n";
		file << "y: " << publicKey.first << "\n";

		file << "Private Key:\n";
		file << "x: " << privateKey << "\n";
		cout << "Keys are saved";
		// Закриття файлу
		file.close();
	}
}


pair<std::pair<Int, Group>, Int> importKeysFromFile(const std::string& filename) {
	ifstream file(filename);

	// Перевірка, чи файл відкрився успішно
	if (!file.is_open()) {
		cout << "Could not open file for reading" ;
	}
	else {
		// Зчитування публічного ключа
		Int y, p, g;
		string line;
		getline(file, line); // Пропустити перший рядок "Public Key:"
		file >> line >> p;
		file >> line >> g;
		file >> line >> y;

		Group publicKey = std::make_pair(p, g);

		// Пропустити рядок "Private Key:"
		getline(file, line);
		getline(file, line);

		// Зчитування приватного ключа
		Int x;
		file >> line >> x; // "x: value"

		// Закриття файлу
		file.close();
		cout << "Keys are imported";
		return make_pair(make_pair(y, publicKey), x);
	}
	file.close();
	return make_pair(make_pair (0, make_pair(0,0)), 0);
}
string wstringToString(const std::wstring& wstr) {
	std::string str(wstr.begin(), wstr.end());
	return str;
}

wstring openFileDialog() {
	OPENFILENAME ofn;       // Структура для діалогу відкриття файлу
	WCHAR szFile[260];      // Буфер для імені файлу

	// Ініціалізація структури
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
	ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Виклик діалогового вікна
	if (GetOpenFileName(&ofn) == TRUE) {
		return wstring(ofn.lpstrFile);
	}
	else {
		cout<< ("No file selected or an error occurred.");
	}
}

string readFile(const std::string& filename) {
	// Відкриття файлу для читання
	std::ifstream file(filename);
	std::stringstream buffer;
	// Перевірка, чи файл відкрився успішно
	if (!file.is_open()) {
		cout << ("Could not open file");
	}
	else {
		// Створення stringstream для читання вмісту файлу
		
		buffer << file.rdbuf();

		// Закриття файлу
		
		// Повернення вмісту файлу у вигляді рядка
	}
	file.close();
	return buffer.str();
}

void writeFile(const std::string& filename, vector<pair<Int, Int>>& crypt_text) {
	std::ofstream file(filename);
	// Перевірка, чи файл відкрився успішно
	if (!file.is_open()) {
		cout << ("Could not open file for writing");
	}
	else {
		// Запис тексту у файл
		for (auto pair : crypt_text) {
			file << pair.first << " " << pair.second << endl;
		}

		cout << "Text crypted in file " << filename << endl;
		// Закриття файлу
	}
	file.close();
}

void writeStringToFile(const string& filename, const string& content) {
	ofstream file(filename); // Відкриття файлу для запису

	// Перевірка, чи файл відкрився успішно
	if (!file.is_open()) {
		cout<<("Could not open file for writing");
	}
	else {
		// Запис рядка в файл
		file << content;
		cout << "Text writed in file " << filename << endl;
	}
	file.close(); // Закриття файлу
}
void ConsoleCursorVisible(bool show, short size)

{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hStdOut, &structCursorInfo);
	structCursorInfo.bVisible = show;
	structCursorInfo.dwSize = size;
	SetConsoleCursorInfo(hStdOut, &structCursorInfo);

}

void GoToXY(short x, short y)

{
	SetConsoleCursorPosition(hStdOut, { x, y });

}
void test() {
	std::setlocale(LC_ALL, "ukr");

	auto keys = ElGamal::keyGen(128);

	auto public_key = keys.first;
	auto private_key = keys.second;


	std::string message = "Як же я заїбався, а ще дохуя";


	std::vector<Int> text;

	for (auto ch : message) {
		text.push_back(static_cast<Int>(ch));
		std::cout << ch;
	}
	std::cout << std::endl;

	std::vector<std::pair<Int, Int>> crypt_text;
	for (auto msg : text) {
		crypt_text.push_back(ElGamal::encrypt(public_key, msg));
	}

	std::string result = "";
	for (auto pair : crypt_text) {
		auto c = ElGamal::decrypt(private_key, public_key, pair);
		result += static_cast<char>(c);
	}
	std::cout << "Decrypted m = " << result << std::endl;
}

pair<pair<Int, Group>, Int> menu_create_keys() {
	string Menu[] = { "Генерацiя","Ввести значення P","Зберегти ключi","Iмпортувати ключi","Показати ключі","Вийти" };
	int active_menu = 0;

	pair<pair<Int, Group>,Int> keys;
	bool exit = false;
	char ch;
	string number;

	while (!exit)
	{
		int x = 50, y = 22;

		GoToXY(x, y);
		
		for (int i = 0; i < size(Menu); i++) {
			if (i == active_menu) SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			else SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			GoToXY(x, y++);
			cout << Menu[i] << endl;
		}

		ch = _getch();
		if (ch == -32) ch = _getch();
		switch (ch)
		{
		case ESC:
			exit = true;
			break;
		case UP:
			if (active_menu > 0) --active_menu;
			else active_menu = 5;
			break;
		case DOWN:
			if (active_menu < size(Menu) - 1) ++active_menu;
			else active_menu = 0;
			break;
		case ENTER:
			system("CLS");
			switch (active_menu)
			{
			case 0:
				GoToXY(0, 3);
				cout << "Введiть розмiр числа p у бiтах(4, 8, 16, 32, 64, 128): ";
				getline(cin, number);
				keys = ElGamal::keyGen(stoi(number));
				break;
			case 1:
				GoToXY(0, 3);
				cout << "Введiть число: ";
				getline(cin, number);
				keys = ElGamal::keyGenOfP(stoull(number));
				break;
			case 2:
				GoToXY(0, 3);
				cout << "Введiть назву файлу: ";
				getline(cin, number);
				saveKeysToFile(keys, number);
				
				break;
			case 3:
				GoToXY(0, 3);
				cout << "Введiть назву файлу: ";
				getline(cin, number);
				keys = importKeysFromFile(number);
				break;
			case 4:
				GoToXY(0, 3);
				showKeys(keys);
				break;
			case 5:
				exit = true;
				break;
			default:
				cout << ch;
				break;
			}
		default:
			cout << ch << endl;
		}
	}
	return keys;
}

vector<pair<Int, Int>> menu_encrypt_text(pair<Int, Group> public_key) {
	string Menu[] = { "Зашифрувати","Зашифрувати з файлу", "Вийти" };
	int active_menu = 0;

	pair<pair<Int, Group>, Int> keys;
	bool exit = false;
	char ch;
	string text;
	vector<pair<Int, Int>> crypt_text;
	while (!exit)
	{
		int x = 50, y = 25;

		GoToXY(x, y);

		for (int i = 0; i < size(Menu); i++) {
			if (i == active_menu) SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			else SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			GoToXY(x, y++);
			cout << Menu[i] << endl;
		}

		ch = _getch();
		if (ch == -32) ch = _getch();
		switch (ch)
		{
		case ESC:
			exit = true;
			break;
		case UP:
			if (active_menu > 0) --active_menu;
			else active_menu = 2;
			break;
		case DOWN:
			if (active_menu < size(Menu) - 1) ++active_menu;
			else active_menu = 0;
			break;
		case ENTER:
			system("CLS");
			switch (active_menu)
			{
			case 0:
				GoToXY(0, 3);
				cout << "Введiть текст для зашифрування: ";
				getline(cin, text);

				//шифрування
				for (auto ch: text) {
					crypt_text.push_back(ElGamal::encrypt(public_key, static_cast<Int>(ch)));
				}
				//вивід шифротексту
				for (auto pair : crypt_text) {
					cout << "(" << pair.first << "," << pair.second << ") ";
				}

				break;
			case 1:
				GoToXY(0, 3);

				text = readFile(wstringToString(openFileDialog()));
				//шифрування
				for (auto ch : text) {
					crypt_text.push_back(ElGamal::encrypt(public_key, static_cast<Int>(ch)));
				}
				//вивід шифротексту
				writeFile("encrypt_text.txt", crypt_text);
			
				break;
			case 2:
				exit = true;
				break;
			default:
				cout << ch;
				break;
			}
		default:
			cout << ch << endl;
		}
	}
	return crypt_text;
}

vector<pair<Int, Int>> menu_decrypt_text(pair<pair<Int, Group>, Int> keys) {
	std::setlocale(LC_ALL, "ukr");
	string Menu[] = { "Розшифрувати(одну пару)","Розшифрувати з файлу", "Вийти" };
	int active_menu = 0;

	bool exit = false;
	char ch;
	char symbol;
	string text;
	vector<pair<Int, Int>> crypt_text;
	pair<Int, Int> pair;
	while (!exit)
	{
		int x = 50, y = 25;

		GoToXY(x, y);

		for (int i = 0; i < size(Menu); i++) {
			if (i == active_menu) SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			else SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			GoToXY(x, y++);
			cout << Menu[i] << endl;
		}

		ch = _getch();
		if (ch == -32) ch = _getch();
		switch (ch)
		{
		case ESC:
			exit = true;
			break;
		case UP:
			if (active_menu > 0) --active_menu;
			else active_menu = 2;
			break;
		case DOWN:
			if (active_menu < size(Menu) - 1) ++active_menu;
			else active_menu = 0;
			break;
		case ENTER:
			text =  "";
			system("CLS");
			switch (active_menu)
			{
			case 0:
				GoToXY(0, 3);

				cout << "Введiть a: ";
				getline(cin, text);
				pair.first = stoull(text);

				cout << "Введiть b: ";
				getline(cin, text);
				pair.second = stoull(text);

				//розшифрування
				cout << "Розшифрований сивол: " << static_cast<char>(ElGamal::decrypt(keys.second, keys.first, pair)) << endl;
				break;
			case 1:
				GoToXY(0, 3);
				//шифрування
				crypt_text = readPairsFromFile(wstringToString(openFileDialog()));
				
				for (auto pair : crypt_text) {
					symbol = static_cast<char>(ElGamal::decrypt(keys.second, keys.first, pair));
					cout << symbol;
					text += symbol;
				}
				writeStringToFile("decrypt_text.txt", text);
				system("CLS");
				break;
			case 2:
				exit = true;
				break;
			default:
				cout << ch;
				break;
			}
		default:
			cout << ch << endl;
		}
	}
	return crypt_text;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::setlocale(LC_ALL, "ukr");
	SetConsoleTitle(L"EL Gamal");
	disableConsoleResize();

	string Menu[] = {"Створення ключiв","Шифровка", "Розшифровка", "Вийти"};
	int active_menu = 0;
	ConsoleCursorVisible(false, 100);
	
	pair<pair<Int, Group>, Int> keys;
	char ch;
	while (true)
	{
		int x = 50, y = 15;
		GoToXY(x, y);

		for (int i = 0; i < size(Menu); i++) {
			if (i == active_menu) SetConsoleTextAttribute(hStdOut, FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			else SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			GoToXY(x, y++);
			cout << Menu[i] << endl;
		}
		ch = _getch();
		if (ch == -32) ch = _getch(); 
		
		switch (ch)
		{
		case ESC:
			exit(0);
		case UP:
			if (active_menu > 0) --active_menu;
			else active_menu = 3;
			break;
		case DOWN:
			if (active_menu < size(Menu)-1) ++active_menu;
			else active_menu = 0;
			break;
		case ENTER:
			switch (active_menu) 
			{
			case 0:
				system("CLS");
				GoToXY(x, 1);
				cout << "Вибраний режим " << Menu[active_menu];
				keys  = menu_create_keys();
				break;
			case 1:
				system("CLS");
				GoToXY(x, 1);
				cout << "Вибраний режим " << Menu[active_menu];
				menu_encrypt_text(keys.first);
				break;
			case 2:
				system("CLS");
				GoToXY(x, 1);
				cout << "Вибраний режим " << Menu[active_menu];
				menu_decrypt_text(keys);
				break;
			case 3:
				exit(0);
			default:
				break;
			}
		default:
			break;
		}

	}



	


}

