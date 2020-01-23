#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;

struct etykietaCzasowa
{
	int czas_wlozenia = -1;
	int czas_zdjecia = -1;
};


void przegladajWglab(etykietaCzasowa* etykiety, int n, vector<int>* sasiedzi, int obecny, vector<int>* przerobione, int* krok, vector<int>* stos);
void startPrzegladajWglab(etykietaCzasowa* etykiety, int n, vector<int>* sasiedzi, vector<int>* stos);
void przegladajWglabCzasy(int* czasyPrzetwarzania, int n, vector<int>* sasiedzi, int obecny, vector<int>* przerobione, vector<vector<int>>* las, vector<int>* stos);
void startPrzegladajWglabCzasy(int* czasyPrzetwarzania, int n, vector<int>* sasiedzi, vector<vector<int>>* las, vector<pair<int, int>> czasyWierzcholki, vector<int>* stos);


int main()
{
	fstream plikIn, plikOut;
	plikIn.open("In0501.txt");
	plikOut.open("Out0501.txt");
	int n = 0;
	string t;
	plikIn >> n;
	getline(plikIn, t);		// pomijanie entera

	vector<int>* wejscie = new vector<int>[n];


	for (int i = 0; i < n; ++i)
	{
		stringstream stringStream;
		string linia;
		getline(plikIn, linia);

		stringStream << linia;
		while (!stringStream.eof())
		{
			int sasiad = i;
			stringStream >> sasiad;
			wejscie[i].push_back(sasiad);
		}
	}
	etykietaCzasowa* etykiety = new etykietaCzasowa[n];
	vector<int>* stos = new vector<int>;
	// 1) pocz¹tek
	startPrzegladajWglab(etykiety, n, wejscie, stos);
	int* czasPrzetwarzania = new int[n];
	for (int i = 0; i < n; ++i)
	{
		czasPrzetwarzania[i] = etykiety[i].czas_zdjecia - etykiety[i].czas_wlozenia;
	}
	// 1) koniec
	// 2) pocz¹tek
	// 2) transformacja na postaæ macierzow¹
	int** macierz = new int*[n];
	for (int i = 0; i < n; ++i)
		macierz[i] = new int[n];

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (find(wejscie[i].begin(), wejscie[i].end(), j) == wejscie[i].end())
				macierz[i][j] = 0;
			else
				macierz[i][j] = 1;
		}
	}
	// 2) stworzenie listy sasiedztwa z transpozycji
	vector<int>* sasiedztwoTranspozycja = new vector<int>[n];
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (macierz[i][j] == 1)
				sasiedztwoTranspozycja[j].push_back(i);
		}
	}
	vector<int> test = sasiedztwoTranspozycja[2];
	// 2) koniec
	vector<int>* stosCzas = new vector<int>;
	// 3) poczatek
	// 3) posortowanie wieszcho³ków zgodnie z czasami przetwarzania
	vector<pair<int, int>> czasyWierzcholki;
	for (int i = 0; i < n; ++i)
	{
		czasyWierzcholki.push_back(make_pair(czasPrzetwarzania[i], i));
	}
	sort(czasyWierzcholki.begin(), czasyWierzcholki.end());
	reverse(czasyWierzcholki.begin(), czasyWierzcholki.end());
	// 3) wywo³anie przechodzenia w glab
	vector<vector<int>>* las = new vector<vector<int>>;
	startPrzegladajWglabCzasy(czasPrzetwarzania, n, sasiedztwoTranspozycja, las, czasyWierzcholki, stosCzas);
	// 3) koniec
	// 4) pocz¹tek
	// 4) wypisywanie danych
	for (int i = 0; i < n; ++i)
		plikOut << "[" << i << "](" << etykiety[i].czas_wlozenia << "/" << etykiety[i].czas_zdjecia << "), ";

	plikOut << endl;
	for (int i = 0; i < n; ++i)
		plikOut << (*stos)[i] << " ";
	plikOut << endl;
	for (int i = 0; i < n; ++i)
		plikOut << (*stosCzas)[i] << " ";
	plikOut << endl;
	plikOut << las->size() << endl;
	for (int i = 0; i < las->size(); ++i)
	{
		plikOut << "{";
		for (int j = 0; j < (*las)[i].size(); ++j)
		{
			plikOut << (*las)[i][j] << " ";
		}
		plikOut << "} ";
	}
	plikOut << endl;
	plikIn.close();
	plikOut.close();
	return 0;
}

void przegladajWglab(etykietaCzasowa* etykiety, int n, vector<int>* sasiedzi, int obecny, vector<int>* przerobione, int* krok, vector<int>* stos)
{
	etykiety[obecny].czas_wlozenia = (*krok)++;
	przerobione->push_back(obecny);
	stos->push_back(obecny);
	for (int i = 0; i < sasiedzi[obecny].size(); ++i)
	{
		 if (find(przerobione->begin(), przerobione->end(), (sasiedzi[obecny])[i]) == przerobione->end())
		 {
			 przegladajWglab(etykiety, n, sasiedzi, (sasiedzi[obecny])[i], przerobione, krok, stos);
		 }
	}
	 etykiety[obecny].czas_zdjecia = (*krok)++;
}
void startPrzegladajWglab(etykietaCzasowa* etykiety, int n, vector<int>* sasiedzi, vector<int>* stos)
{
	vector<int>* przerobione = new vector<int>();
	int* wKrok = new int;
	*wKrok = 1;
	int i = 0;
	while (przerobione->size()< n)
	{
		if (find(przerobione->begin(), przerobione->end(), i) == przerobione->end())
		{
			przegladajWglab(etykiety, n, sasiedzi, i, przerobione, wKrok, stos);
			++i;
		}
		else
			++i;
	}
}
void przegladajWglabCzasy(int* czasyPrzetwarzania, int n, vector<int>* sasiedzi, int obecny, vector<int>* przerobione, vector<vector<int>>* las, vector<int>* stos)
{
	vector<pair<int, int>> czasSasiedzi;
	for (int i = 0; i < sasiedzi[obecny].size(); ++i)
		czasSasiedzi.push_back(make_pair(czasyPrzetwarzania[i], sasiedzi[obecny][i]));

	sort(czasSasiedzi.begin(), czasSasiedzi.end());
	reverse(czasSasiedzi.begin(), czasSasiedzi.end());

	przerobione->push_back(obecny);
	stos->push_back(obecny);
	for (int i = 0; i < sasiedzi[obecny].size(); ++i)
	{
		if (find(przerobione->begin(), przerobione->end(), czasSasiedzi[i].second) == przerobione->end())
		{
			las->back().push_back(czasSasiedzi[i].second);
			przegladajWglabCzasy(czasyPrzetwarzania, n, sasiedzi, czasSasiedzi[i].second, przerobione, las, stos);
		}
	}
	 
}
void startPrzegladajWglabCzasy(int* czasyPrzetwarzania, int n, vector<int>* sasiedzi, vector<vector<int>>* las, vector<pair<int, int>> czasyWierzcholki, vector<int>* stos)
{
	vector<int>* przerobione = new vector<int>();

	int i = 0;
	while (przerobione->size() < n)
	{
		if (find(przerobione->begin(), przerobione->end(), czasyWierzcholki[i].second) == przerobione->end())
		{
			las->push_back(*(new vector<int>()));
			las->back().push_back(czasyWierzcholki[i].second);
			przegladajWglabCzasy(czasyPrzetwarzania, n, sasiedzi, czasyWierzcholki[i].second, przerobione, las, stos);
			++i;
		}
		else
			++i;
	}
}