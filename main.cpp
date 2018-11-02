// autor : Krystian Plackowski
// sterowanie strzalki
// przy ekranie koncowym : esc zamyka okno

#include <stdio.h>
#include <math.h>
#include "winbgi2.h"
#include <stdlib.h>
#include <time.h>
#include <queue>
#define abs(x)( x > 0 ? x : -(x))

using namespace std;

	
queue < pair< int, int>> Q;					// kolejka trzymajaca wspolrzedne kolejnych kawalkow weza


void square( int x, int y, int color)		// narysuj kwadrat o srodku ( x, y), boku 20 i danym kolorze
{
	int i;
	setcolor( color);

	for( i=9; i > 0; i--)
		rectangle( x-i, y-i, x+i, y+i);

	putpixel( x, y, color);
}

void food( int x, int y, int R, int color)	// narysuj jedzenie (romb) o wielkosci R i kolorze
{
	setcolor( color);

	int i, j;

	for( i = -R; i <= R; i++)
		for( j = -R; j <= R; j++)
			if( abs(i) + abs(j) <= R)
				putpixel( x+i, y+j, color);
}

int losuj( int a, int b)					// losuj liczbe w przedziale < a, b>
{
	b = a + rand() % (b-a);
	b = b - b % 20;							// zaokraglona do wielokrotnosci 20
	return b < a ? b+20 : b;				// upewnij sie ze wynik > a
}

void wypisz( queue < pair< int, int>> Q)	// wypisanie calego weza
{
	int x, y;

	while( !Q.empty())
	{
		x = Q.front().first;
		y = Q.front().second;

		Q.pop();

		square( x, y, BLACK);
	}
}

char sprawdz( queue < pair< int, int>> Q, int xj, int yj)	// sprawdzenie czy jedzenie (kolko) lezy na wezu
{
	int x, y;

	while( !Q.empty())
	{
		x = Q.front().first;
		y = Q.front().second;
		if( x == xj && y == yj)
			return 0;						// 0 - nakladanie sie!
		Q.pop();
	}
	return 1;								// 1 - OK
}

char sprawdz2( queue < pair< int, int>> Q)	// zwraca czy segment nr 1 przecina sie z innym
{
	int x, y, xp, yp;

	xp = Q.back().first;					// wspolrzedne 1. segmentu (glowy)
	yp = Q.back().second;

	while( Q.size() > 1)
	{
		x = Q.front().first;
		y = Q.front().second;
		if( x == xp && y == yp)
			return 0;						// 0 - przeciecie!
		Q.pop();
	}
	return 1;								// 1 - pierwszy segment nie przecina sie z zadnym innym
}



void main()
{
	srand( time( NULL));
	int i, x, y, vx, vy, xj, yj, xp, yp;	// ( xj, yj) - wspolrzedne jedzenia, ( xp, yp) - zapasowe
	char c, cp, d;							// cp - zapamietaj poprzedni ruch, d - pomocnicza


	graphics( 490, 540);
	setcolor( WHITE);
	rectangle( 30-6, 30-6, 450+6, 450+6);	// zrob ramke
	for( i=30-3+1; i < 450+3; i++)
		rectangle( 30-3+1, 30-3+1, i, i);

	vx = vy = 0;



	settextstyle( 10, 0, 3);				// wyswietlanie wyniku
	int wynik = 0;
	char t[15]={0}, *u = "SCORE: 0";		// tablica t jest slowem tak jakby ( "SCORE: %d", wynik)
	for( i=0; i < 10; i++)
		t[i] = u[i];

	outtextxy( 30, 480, t);




	square( 240, 240, BLACK);	Q.push( make_pair( 240, 240));	// zadeklaruj poczatkowa pozycje weza
	square( 240, 260, BLACK);	Q.push( make_pair( 240, 260));
	square( 240, 280, BLACK);	Q.push( make_pair( 240, 280));
	square( 240, 300, BLACK);	Q.push( make_pair( 240, 300));
	x = 240;
	y = 300;


	do												// losuj jedzenie
	{
		xj = losuj( 60, 420);
		yj = losuj( 60, 420);
	}
	while( xj == 240 && 240 <= yj && yj <= 300);	// ktore nie moze sie nalozyc z poczatkowa pozycja weza

	food( xj, yj, 10, BLACK);						// rysuj jedzenie




	cp = 0;

	while( animate(15))
	{
		c = 0;										// w danym ruchu :

		while(kbhit())								// pobierz ostatni znak ze strumienia (jesli jakies znaki sa)
		{
			d = getch();
			if( 37 <= d && d <= 40)
				c = d;
		}

		// W      38
		// S	  40
		// D (->) 39
		// A (<-) 37

		if( c == 38 && cp != 40 && cp != 0)
		{
			vx = 0;
			vy = -20;
		}
		if( c == 40 && cp != 38)
		{
			vx = 0;
			vy = 20;
		}
		if( c == 37 && cp != 39)
		{
			vx = -20;
			vy = 0;
		}
		if( c == 39 && cp != 37)
		{
			vx = 20;
			vy = 0;
		}

		if( cp == 0 && c == 38)					// zapobiegnij, aby pierwszy nie odbywal sie w gore
			c = 0;

		if( c != 0)
			cp = c;


		
		if( vx == 0 && vy == 0)
			continue;

		x += vx;
		y += vy;
		Q.push( make_pair( x, y));					// wrzuc na kolejke wspolrzedne kolejnego (wiec pierwszego) elem. weza


		if( x < 30 || x > 450 || y < 30 || y > 450)	// wyjscie weza poza ramke konczy gre
			break;

		if(sprawdz2( Q) == 0)						// jesli = 0 oznacza to wykrycie przeciecia
			break;


		if( x == xj && y == yj)						// zjadanie
		{
			wynik++;

			food( xj, yj, 10, WHITE);				// wytrzyj stare jedzenie

			do
			{
				xj = losuj( 60, 420);				// losuj wspolrzedne nowego kolka
				yj = losuj( 60, 420);
			}
			while( sprawdz( Q, xj, yj) == 0);		// tak dlugo, az nie bedzie przecinac ono weza
		}
		else										// w przypadku niezjadania w danym ruchu, wytrzyj ostatni segment weza
		{
			xp = Q.front().first;
			yp = Q.front().second;
			Q.pop();
			square( xp, yp, WHITE);
		}

		
		setcolor( BLACK);
		rectangle( 30-3, 30-3, 450+3, 450+3);		// wypisz ramke
		food( xj, yj, 10, BLACK);					// wypisz jedzienie
		



		wypisz( Q);									// wypisz calego weza (nadpisujac przy tym wszystkie jego segmenty)



		setcolor( BLACK);							// wytarcie napisu starego wyniku
		outtextxy( 30, 480, t);


		if( wynik < 10)								// stworzenie i wypisanie nowego napisu
		{
			t[7] = wynik % 10 + 48;					// laczenie slowa i liczby (wyniku)
		}
		else
		{
			t[7] = wynik / 10 + 48;
			t[8] = wynik % 10 + 48;
		}
		setcolor( WHITE);
		outtextxy( 30, 480, t);
	}


	clear();										// wymaz wszystko
	setcolor( BLACK);
	outtextxy( 30, 480, t);


	
	setcolor( WHITE);								// napisy koncowe
	settextstyle( 10, 0, 6);
	outtextxy( 100, 250, "GAME OVER");


	settextstyle( 10, 0, 3);
	char *w = "YOU SCORED:   ";
	for( i=0; i < 14; i++)
		t[i] = w[i];

	if( wynik < 10)									// laczenie slow i liczby
	{
		t[12] = wynik % 10 + 48;
		t[13] = 0;
	}
	else
	{
		t[12] = wynik / 10 + 48;
		t[13] = wynik % 10 + 48;
		t[14] = 0;
	}
	outtextxy( 145, 350, t);
	


	while( getch()!= 27);							// esc konczy gre
	wait();
}
