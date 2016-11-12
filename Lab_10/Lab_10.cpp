// Lab_10.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <math.h>

using namespace std;

double wzor_analityczny(double t)
{
	return 1 - exp(-10.0 * (t + atan(t)));
}

double metoda_bezposrednia_eulera(double krok, int N)
{
	double blad = 0.0, t = krok;
	double y = 0.0;	//warunek poczatkowy y(0) = 0
	double wart_dokladna;

	for (int i = 0; i < N; i++)
	{
		wart_dokladna = wzor_analityczny(t);
		y = y + krok *(-((10.0 *t * t + 20.0) / (t * t + 1.0))*(y - 1.0));
		wart_dokladna = fabs(wart_dokladna - y);
		if (wart_dokladna > blad)
			blad = wart_dokladna;
		t += krok;
	}
	return blad;

}

double metoda_posrednia_eulera(double krok, int N)
{
	double blad = 0.0, t = krok;
	double y = 0.0;	//warunek poczatkowy y(0) = 0
	double wart_dokladna, ulamek;

	for (int i = 0; i < N; i++)
	{
		wart_dokladna = wzor_analityczny(t);
		ulamek = (10.0 * (t + krok) * (t + krok) + 20.0) / ((t + krok) * (t + krok) + 1.0);
		y = (y + krok * ulamek) / (1 + krok * ulamek);
		wart_dokladna = fabs(wart_dokladna - y);
		if (wart_dokladna > blad)
			blad = wart_dokladna;
		t += krok;
	}
	return blad;
}

double metoda_trapezow(double krok, int N)
{
	double blad = 0.0, t = krok;
	double y = 0.0;	//warunek poczatkowy y(0) = 0
	double wart_dokladna, ulamek_n, ulamek_n_plus;

	for (int i = 0; i < N; i++)
	{
		wart_dokladna = wzor_analityczny(t);
		ulamek_n = ((10.0 * t * t + 20.0) / (t * t + 1.0));
		ulamek_n_plus = (10.0 * (t + krok) * (t + krok) + 20.0) / ((t + krok) * (t + krok) + 1.0);;
		y = ((-krok / 2.0) * (ulamek_n * (y - 1.0) - ulamek_n_plus) + y) / (1.0 + (krok / 2.0) * ulamek_n_plus);
		wart_dokladna = fabs(wart_dokladna - y);
		if (wart_dokladna > blad)
			blad = wart_dokladna;
		t += krok;
	}
	return blad;

}

double metoda_bezposrednia_eulera_rys(double krok, double tmax)
{
	double y = 0.0;	//warunek poczatkowy y(0) = 0

	for (double i = 0.0; i < tmax; i += krok)
	{
		y = y + krok *(-((10.0 *i * i + 20.0) / (i * i + 1.0))*(y - 1.0));
	}
	return y;
}

double metoda_posrednia_eulera_rys(double krok, double tmax)
{
	double y = 0.0, ulamek;	//warunek poczatkowy y(0) = 0

	for (double i = 0.0; i < tmax; i += krok)
	{
		ulamek = (10.0 * (i + krok) * (i + krok) + 20.0) / ((i + krok) * (i + krok) + 1.0);
		y = (y + krok * ulamek) / (1 + krok * ulamek);
	}
	return y;
}

double metoda_trapezow_rys(double krok, double tmax)
{
	double y = 0.0, ulamek_n, ulamek_n_plus;	//warunek poczatkowy y(0) = 0

	for (double i = 0.0; i < tmax; i += krok)
	{
		ulamek_n = ((10.0 * i * i + 20.0) / (i * i + 1.0));
		ulamek_n_plus = (10.0 * (i + krok) * (i + krok) + 20.0) / ((i + krok) * (i + krok) + 1.0);;
		y = ((-krok / 2.0) * (ulamek_n * (y - 1.0) - ulamek_n_plus) + y) / (1.0 + (krok / 2.0) * ulamek_n_plus);
	}
	return y;
}

void metody_roznicowe()
{
	double krok = 0.1, mbe_s, mbe_ns, mpe, mt, anl;
	const double t_max = 2.0;
	int N = 1000, i = 0;
	fstream bledy, dane_1, dane_2;

	bledy.open("bledy.txt", fstream::out);

	
	cout << "  i |         krok |            mbe |            mpe|            mt |" << endl;
	cout << "-------------------------------------------------------------------" << endl;

	for (krok; krok > 1e-20; krok /= 2)
	{
		mbe_s = log10(metoda_bezposrednia_eulera(krok, N));
		mpe = log10(metoda_posrednia_eulera(krok, N));
		mt = log10(metoda_trapezow(krok, N));
		bledy << log10(krok) << " " << mbe_s << " " << mpe << " " << mt << endl;
		cout.width(4);
		cout << i << "|";
		cout.width(15);
		cout << krok << "|";
		cout.width(15);
		cout << mbe_s << "|";
		cout.width(15);
		cout << mpe << "|";
		cout.width(15);
		cout << mt << "|" << endl;

		i++;
	}
	bledy.close();

	//cout << endl << "Dane do a b c" << endl;
	//cout << "  i |         krok |           anl |           mpe |          mt |           mbe_s |          mbe_ns |" << endl;
	//cout << "-------------------------------------------------------------------------------------------------------" << endl;

	dane_1.open("dane.txt", fstream::out);
	krok = 0.01;
	for (double t = 0; t < 5; t += 0.01)
	{
		anl = wzor_analityczny(t);
		mpe = metoda_posrednia_eulera_rys(krok, t);
		mt = metoda_trapezow_rys(krok, t);
		mbe_s = metoda_bezposrednia_eulera_rys(krok, t);

		dane_1 << t << " " << anl << " " << mpe << " " << mt << " " << mbe_s << endl;
		/*cout.width( 4 );
		cout << t << "|";
		cout.width( 15 );
		cout << anl << "|";
		cout.width( 15 );
		cout << mpe << "|";
		cout.width( 15 );
		cout << mt << "|" ;
		cout.width( 15 );
		cout << mbe_s << "|" ;
		cout.width( 15 );
		cout << mbe_ns << "|" << endl;*/
	}
	dane_1.close();


	dane_2.open("danee.txt", fstream::out);

	for (double t = 0; t < 5; t += 0.15)
	{
		mbe_ns = metoda_bezposrednia_eulera_rys(0.15, t);
		dane_2 << t << " " << mbe_ns << endl;

	}
	dane_2.close();

}

int _tmain(int argc, _TCHAR* argv[])
{
	metody_roznicowe();
	system("Pause");
	return 0;
}


