#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Team
{
	string name;
	string result; // для олимпиадной сетки
	int point = 0; // для табличной лиге
};
struct Match
{
	Team first;
	Team second;
	Team winner;
	Team loser;
};

int len_teams = 8; // Количество команды, которые участвуют в турнире
vector<Team> all_teams; // Все команды, участвующие в турнире
vector<vector<Match>> grid_teams; // сетка команд, которые играю между собой (пара: команда 1, команда 2).
// Для каждого раунда (1/16, 1/8 и т.д. - своя очередь матчей)

// Проверка на то, что строка - это число
bool IsNumber(const string& s) {
	if (s.empty()) return false; // если строка пустая, то s - это не число
	for (char c : s) {
		if (!isdigit(c)) return false; // если встреченный символ не является числом, то s - это не число
	}
	return true;
}
// Сбросить результаты играющих команд (сброс настроек по умолчанию)
void ResetTeams() {
	system("cls"); // очищаем выводы в консоль

	len_teams = 8;
	for (int i = 0; i < grid_teams.size(); ++i) {
		grid_teams[i].clear(); // очистка матчей во всех раундах олимпиадной сетки
	}
	grid_teams.clear(); // очищаем все раунды олимпиадной сетки

	// очистка всех созданных команд
	for (auto& team : all_teams) {
		team.point = 0;
		team.result = "";
	}
	all_teams.clear();
}
// Перевод строки из двоичной системы в десятичную систему
int BinaryStringToDecimal(const string& binary) {
	int result = 0;
	for (char c : binary) {
		result = result * 2 + (c - '0'); // поразрядно переводим из двоичной в десятичную (один из способов перевода)
	}
	return result;
}
// Проверка на то, является ли число степенью двойки
bool IsPowerOfTwo(int x) {
	if (x <= 0) return false; // если число меньше или равно 0, то оно не является степенью двойки

	// Постепенно делим число. Если по мере деления появляется остаток, то число не степень двойки.
	// Если число после делений превратилось в 1, то число является степенью двойки
	while (x % 2 == 0) {
		if ((x /= 2) == 1) return true;
	}
	return false;
}
// Рандомайзер случайных чисел
int Random(int start, int end)
{
	return rand() % (end - start + 1) + start; // обычный рандомайзер
}
// Перетасовать массив команд для того, чтобы турнирная сетка создавалась случайно
void ShuffleTeams(vector<Team>& teams)
{
	int len = teams.size();
	for (int i = 0; i < Random(1'000, 10'000); ++i) {
		swap(teams[Random(0, len - 1)], teams[Random(0, len - 1)]); // меняем местами два элемента в массиве
	}
}
// Создание имён комад (у каждой команды имя - это число от 1, до len включительно
vector<Team> GenerateTeams(int len)
{
	vector<Team> create_teams;
	for (int i = 0; i < len; ++i) {
		// Создаём команду
		Team team;
		team.name = to_string(i + 1);
		// Добавляем команду
		create_teams.push_back(team);
	}
	return create_teams;
}
// Симуляция матчей
void MatchSimulationGrid(vector<Team>& input, vector<Team>& all_results, bool is_winner_branch = true)
{
	// Если количество команд равна 1, то мы дошли до края и дальше не получится симулировать, т.к. команда одна, ей не с кем играть
	if (input.size() == 1) {
		all_results.push_back(input[0]); // записываем все сыгранные матчи команды в результат.
		// При чём эта функция реализована так, что в результате будут сразу отсортированные команды по своим местам (1, 2, 3 и т.д. места)
		return;
	}

	vector<Team> winners; // здесь будут команды победители
	vector<Team> losers; // здесь будут команды проигравшие
	vector<Match> matches_round; // сохраняем результаты матчей, чтобы 

	// Играем все матчи
	for (int i = 0; i < input.size(); i += 2) {
		Team& t1 = input[i]; // команда 1
		Team& t2 = input[i + 1]; // команда 2, которая идёт следом после первой
		Match match; // будем хранить информацию о матче и в будущем выводить
		match.first = t1;
		match.second = t2;

		bool t1_wins = Random(0, 1) == 0; // определяем победила ли первая команда

		if (t1_wins) {
			t1.result += '0'; // 0 - победа
			t2.result += '1'; // 1 - поражение
			winners.push_back(t1);
			losers.push_back(t2);
			match.winner = t1;
			match.loser = t2;
		}
		else {
			t1.result += '1';
			t2.result += '0';
			winners.push_back(t2);
			losers.push_back(t1);
			match.winner = t2;
			match.loser = t1;
		}
		matches_round.push_back(match); // сохраняем информацию о матче
	}

	if (is_winner_branch) grid_teams.push_back(matches_round); // при выводе информации будем отображать только победителей, чтобы воспринимать в привычной форме
	MatchSimulationGrid(winners, all_results, is_winner_branch);
	MatchSimulationGrid(losers, all_results, false);
}
// Выводы в консоль
void PrintSimulationGrid() {
	cout << "===================" << endl;
	cout << "Итоги турнирной сетки в привычной словесной форме:" << endl;
	// Выводим все раунды и их матчи
	for (int i = 0; i < grid_teams.size(); ++i) {
		if (i != 0) cout << endl;
		cout << "Раунд " << i + 1 << ":\n";

		vector<Match> round = grid_teams[i];
		int match_num = 1;

		for (const auto& match : round) {
			cout << "  Матч " << match_num << ": "
				<< match.first.name << " против " << match.second.name
				<< " → Победитель: " << match.winner.name << "\n";
			++match_num;
		}
	}
	cout << "===================" << endl;
}
// Вывод результатов (по тому, кто какое место занял) игр в сетке
void PrintResultsGrid(vector<Team> all_results)
{
	cout << "===================" << endl;
	cout << "Результаты всех игр: " << endl;
	for (int i = 0; i < all_results.size(); ++i) {
		// переводим получившийся результат в играх из двоичной в десятичную и узнаём какое место заняла команда
		cout << BinaryStringToDecimal(all_results[i].result) + 1 << " место: " << all_results[i].name << endl;
	}
	cout << "===================" << endl;
}
// Создание турнирной таблицы
void MatchSimulationTable(vector<Team>& teams)
{
	int win_points = 3, draw_points = 1, defeat_points = 0;
	// Каждый играет с каждым. Это достигается с помощью вложенного цикла
	for (int i = 0; i < teams.size() - 1; ++i) {
		for (int j = i + 1; j < teams.size(); ++j) {
			int res_match = Random(0, 2); // 0 - победа, 1 - ничья, 2 - поражение
			switch (res_match) {
			case 0:
				teams[i].point += win_points;
				teams[j].point += defeat_points;
				break;
			case 1:
				teams[i].point += draw_points;
				teams[j].point += draw_points;
				break;
			case 2:
				teams[i].point += defeat_points;
				teams[j].point += win_points;
				break;
			}
		}
	}
}
// Сортировка по очкам турнирного чемпионата (пузырьком)
void SortByPoints(vector<Team>& teams) {
	for (int i = 0; i < teams.size(); ++i) {
		for (int j = 0; j < teams.size() - i - 1; ++j) {
			if (teams[j].point < teams[j + 1].point) {
				Team temp = teams[j];
				teams[j] = teams[j + 1];
				teams[j + 1] = temp;
			}
		}
	}
}
// Вывод в консоль результатов таблицы
void PrintSimulationTable(vector<Team> teams)
{
	SortByPoints(teams); // сортируем, чтобы удобно было вывести информацию
	cout << "\nТурнирная таблица:\n";
	cout << "Место     | Команда           | Очки\n";
	cout << "------------------------------------\n";

	for (int i = 0; i < teams.size(); ++i) {
		cout << " " << i + 1;

		for (int j = 0; j < 9 - to_string(i + 1).length(); ++j)
			cout << " "; // отступ, чтобы вывести таблицу ровной

		cout << "| ";

		// Команда: выравниваем до 11 символов
		cout << teams[i].name;
		for (int j = 0; j < (18 - teams[i].name.length()); ++j)
			cout << " "; // отступ, чтобы вывести таблицу ровной

		cout << "| " << teams[i].point << "\n";
	}

	cout << "------------------------------------\n";
}
// Вывод имён всех команд в консоль
void PrintTeams(const vector<Team>& teams)
{
	cout << "===================" << endl;
	cout << "Все команды, учавствующие в турнире:" << endl;
	for (int i = 0; i < teams.size(); ++i) {
		cout << teams[i].name;
		if (i != teams.size() - 1)
			cout << "; ";
	}
	cout << "\n===================" << endl;
}
// Запрос на ввод количества команд
void EditLenTeams()
{
	system("cls");
	do {
		cout << "Введите количество команд (количество команд должно являться степенью двойки): "; cin >> len_teams;
		cout << endl;
		// если не является степенью двойки, то сообщаем об ошибке
		if (!IsPowerOfTwo(len_teams)) {
			system("cls");
			cout << "Ошибка: введённое число не является степенью двойки. Попробуйте снова...\n" << endl;
		}
	} while (!IsPowerOfTwo(len_teams));
	system("cls");
}
// Считывание с файла
bool LoadTeamsFromFile(const string& filename, vector<Team>& teams) {
	// Открываем файл
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Ошибка: не удалось открыть файл: " << filename << endl;
		return false;
	}

	// Извлекаем строки по одному и запоминаем
	string line;
	while (getline(file, line)) {
		if (!line.empty()) {
			Team t;
			t.name = line;
			teams.push_back(t);
		}
	}

	file.close(); // закрываем файл

	// Если не является степенью двойки, то пишем об ошибке
	if (!IsPowerOfTwo(teams.size())) {
		cout << "Ошибка: количество команд должно быть степенью двойки (2, 4, 8, 16, ...)" << endl;
		return false;
	}
	len_teams = teams.size(); // меняем количество команд

	return true;
}

int main()
{
	system("chcp 1251>nul"); // Для работы русского языка
	srand(time(0)); // Связываем семя со временем
	
	bool isRunning = true;
	bool teams_selected = false;
	while (isRunning) {
		string choice;
		int num;
		do {
			cout << "Заданное число команд: " << len_teams << endl;
			cout << "Выберите действие (введите число):\n1) Построить и провести симуляцию олимпиадной турнирной сетки" << endl;
			cout << "2) Построить и провести симуляцию турнирной таблицы" << endl;
			cout << "3) Считать команды из файла" << endl;
			cout << "4) Поменять количество команд" << endl;
			cout << "5) Очистить выбранные команды и вернуть настройки по умолчанию" << endl;
			cout << "6) Выйти из програмы" << endl;
			cout << "Выбор: "; cin >> choice;

			if (IsNumber(choice)) {
				num = stoi(choice); // преобразование строки в целое число
				if (num < 0 or num > 6) {
					system("cls");
					cout << "Операции под таким номером нет. Повторите попытку...\n" << endl;
				}
			}
			else {
				system("cls");
				cout << "Введено было не число. Повторите попытку...\n" << endl;
				num = -1; // любое число вне диапазона
			}
		} while (!IsNumber(choice) or (num < 0 or num > 6));

		switch (num) {
		case 1: {
			vector<Team> teams;
			if (!teams_selected) {
				teams = GenerateTeams(len_teams); // гененрируем команды (даём им числовое имя)
				all_teams = teams; // сохраняем оригинал
			}
			else {
				teams = all_teams;
			}
			PrintTeams(teams);
			ShuffleTeams(teams); // перетасуем команды, чтобы составить турнирную сетку случайно

			vector<Team> results;
			MatchSimulationGrid(teams, results);

			PrintResultsGrid(results);
			PrintSimulationGrid();

			cout << "Введите что-нибдь для возврата в главное меню: "; cin >> choice;
			ResetTeams();
			break;
		}
		case 2: {
			vector<Team> teams;
			if (!teams_selected) {
				teams = GenerateTeams(len_teams); // гененрируем команды (даём им числовое имя)
				all_teams = teams; // сохраняем оригинал
			}
			else {
				teams = all_teams;
			}
			PrintTeams(teams);
			ShuffleTeams(teams); // перетасуем команды, чтобы составить турнирную таблицу случайно

			MatchSimulationTable(teams);
			PrintSimulationTable(teams);

			cout << "Введите что-нибдь для возврата в главное меню: "; cin >> choice;
			ResetTeams();
			break;
		}
		case 3: {
			vector<Team> teams;
			if (LoadTeamsFromFile("teams.txt", teams)) {
				all_teams = teams; // сохраняем оригинал
				teams_selected = true;
				cout << "Команды выбраны успешно!" << endl;
			}
			else {
				teams_selected = false;
				cout << "Команды не удалось выбрать!" << endl;
				break;
			}

			cout << "\nВведите что-нибдь для возврата в главное меню: "; cin >> choice;
			system("cls");
			break;
		}
		case 4: { // изменение количества команд
			if (teams_selected) {
				cout << "Нельзя изменить количество команд, т.к. команды были считаны из файла. Сначала очистите считанные команды" << endl;
				cout << "\nВведите что-нибдь для возврата в главное меню: "; cin >> choice;
				system("cls");
				break;
			}
			EditLenTeams();
			break;
		}
		case 5: { // очистить всё и вернуть настройки по умолчанию
			system("cls");
			ResetTeams();
			teams_selected = false;

			break;
		}
		case 6: { // выйти из программы
			isRunning = false;
			break;
		}
		default:
			system("cls");
			cout << "Неизвестная ошибка\n" << endl;
		}
	}

	return 0;
}