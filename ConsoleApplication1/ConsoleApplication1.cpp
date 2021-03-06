// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
/* El siguiente programa representa la implementación de los requisitos planteados en la práctica 1
 de la asignatura Matematica Aplicada. El algoritmo inicialmente escogido para ser implementado fue el
 algoritmo QuickHull. Posteriormente se decidio implementar tambien los algoritmos GiftWrapping e 
 Incremental con el fin de poder establecer una comparación entre ellos.
 
 Para la implementación, se ha utilizado como referencia, además del material de clase, el libro
 Computational Geometry in C, de Joshep O´Rourke. Al consultar el libro, se puede ver que los tres 
 algoritmos elegidos tienen complejidad O(n^2), al menos en los casos más desfavorables (aunque también se 
 afirma, que el algoritmo Incremental se puede modificar para obtener una complejidad O(n * lg(n)) para los 
 casos no favorables). Este comportamiento ha quedado reflejado durante el desarrollo del código, puesto 
 que al utilizar cantidades de puntos grandes, el algoritmo GiftWrapping llega a ejecutarse en menos tiempo 
 que los otros dos. Este es un ejemplo de esas ejecuciones (Texto obtenido de la consola):

 QuickHull - GiftWrapping - Incremental
 42.059  26.552  48.347

 Dibujando ultimo caso ejecutado. El numero de puntos a dibujar es: 4000000

 
 La ejecución de este programa se puede modificar utilizando las variables numberAttempts y numeroPuntos. 
 Estas dos variables se encuentran declaradas aproximadamente en la linea 168 de este fichero. Una vez que 
 la ejecución finalice, se escribirá en pantalla la media de tiempo de ejecución que ha necesitado cada 
 algoritmo para obtener las envolventes convexas.

 Las nubes de puntos utilizadas se obtienen aleatoriamente invocando a la función test2Dataset. Además de 
 esta función, también se ha declarado la función test1Dataset, que devuelve una nube de puntos constante. 
 La llamada a la función test1Dataset está comentada en el código junto a la llamada a test2Dataset. De esta 
 manera es facil cambiar una por otra.

 Al finalizar la ejecución del programa, se pintará en una ventana (utilizando la librería Allegro) el último
 Convex Hull calculado durante el procesado (incluyendo también la nube de puntos completa). Esto permite 
 comprobar, de manera visual, que todos los algoritmos obtienen la misma envolvente convexa. De no ser así, se 
 podrá ver en pantalla la discrepancia.
 
 Finalmente, el programa esperará a que se pulse una tecla y la tecla enter para finalizar
 */

#include "stdafx.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_color.h>

#include <vector>
#include <ctime>
#include <algorithm>
#include <iomanip>

#include "Point.h"
#include "ConvexHull.h"
#include "ConsoleApplication1.h"

using namespace std;


vector<Point> test1Dataset() {

	vector<Point> convexHull;
	vector<Point> pointsVector;

	pointsVector.push_back(Point(10, 140));
	pointsVector.push_back(Point(120, 120));
	pointsVector.push_back(Point(170, 150));
	pointsVector.push_back(Point(140, 190));
	pointsVector.push_back(Point(110, 140));
	pointsVector.push_back(Point(160, 130));
	pointsVector.push_back(Point(120, 180));
	pointsVector.push_back(Point(180, 160));
	pointsVector.push_back(Point(180, 170));
	pointsVector.push_back(Point(180, 180));
	pointsVector.push_back(Point(170, 170));
	pointsVector.push_back(Point(130, 140));
	pointsVector.push_back(Point(140, 170));
	pointsVector.push_back(Point(10, 140)); // Repetido a proposito un punto que esta incluido en la envolvente convexa
	pointsVector.push_back(Point(150, 160));

	pointsVector.push_back(Point(10, 150));


	return pointsVector;
}

vector<Point> test2Dataset(int numeroPuntos) {

	vector<Point> pointsVector;

	//Inicializar generador de numeros aleatorios
	srand((unsigned)time(NULL));

	int valorX;
	int valorY;
	int const VALOR_MAXIMO_X = 1000;
	int const VALOR_MAXIMO_Y = 700;

	for (int i = 0; i < numeroPuntos; i++) {
		valorX = (rand() % VALOR_MAXIMO_X) + 20;
		valorY = (rand() % VALOR_MAXIMO_Y) + 20;

		Point p(valorX, valorY);

		pointsVector.push_back(p);
	}

	return pointsVector;
}

int main()
{
	vector<Point> convexHullVector_QH;
	vector<Point> convexHullVector_GW;
	vector<Point> convexHullVector_IN;
	vector<Point> pointsVector;


	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_COLOR white = ALLEGRO_COLOR();
	ALLEGRO_COLOR yellow = ALLEGRO_COLOR();
	ALLEGRO_COLOR red = ALLEGRO_COLOR();
	ALLEGRO_COLOR green = ALLEGRO_COLOR();
	ALLEGRO_COLOR blue = ALLEGRO_COLOR();
	ALLEGRO_BITMAP *backBuffer = NULL;

	// Fijar los colores utilizados para pintar las envolventes convexas
	white.r = 255;
	white.g = 255;
	white.b = 255;

	yellow.r = 63;
	yellow.g = 63;
	yellow.b = 0;

	red.r = 50;
	red.g = 0;
	red.b = 0;

	green.r = 0;
	green.g = 50;
	green.b = 0;

	blue.r = 0;
	blue.g = 0;
	blue.b = 50;

	al_init_primitives_addon();

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(1040, 730);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	backBuffer = al_get_backbuffer(display);

	al_clear_to_color(al_map_rgb(0, 0, 0));

	time_t start;
	time_t end;

	cout << setprecision(7);

	const int numberAttempts = 40;
	const int numeroPuntos = 400;
	double tablaTiempos[numberAttempts][3] = { 0 };

	for (int i = 0; i < numberAttempts; i++) {
		//pointsVector = test1Dataset();

		pointsVector = test2Dataset(numeroPuntos);

		start = clock();
		convexHullVector_QH = ConvexHull::quickHull(pointsVector);
		end = clock();
		tablaTiempos[i][0] += (float(end - start) / CLOCKS_PER_SEC);

		//cout << "QH Time: " << float(end - start) / CLOCKS_PER_SEC << endl;

		start = clock();
		convexHullVector_GW = ConvexHull::giftWrapping(pointsVector);
		end = clock();
		tablaTiempos[i][1] += (float(end - start) / CLOCKS_PER_SEC);

		//cout << "GW Time: " << float(end - start) / CLOCKS_PER_SEC << endl;

		std::sort(pointsVector.begin(), pointsVector.end());
		start = clock();
		convexHullVector_IN = ConvexHull::incremental(pointsVector);
		//convexHullVector_IN = ConvexHull::graham(pointsVector);
		end = clock();
		tablaTiempos[i][2] += (float(end - start) / CLOCKS_PER_SEC);

		//cout << "IN Time: " << float(end - start) / CLOCKS_PER_SEC << endl;
	}


	double tiemposTotales[3] = { 0 };

	cout << "QuickHull - GiftWrapping - Incremental" << endl;
	for (int i = 0; i < numberAttempts; i++) {
		for (int j = 0; j < 3; j++) {
			tiemposTotales[j] += tablaTiempos[i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		cout << tiemposTotales[i] / static_cast<double>(numberAttempts) << "\t";
	}
	cout << endl << endl;

	cout << "Dibujando ultimo caso ejecutado. El numero de puntos a dibujar es: " << numeroPuntos << endl << endl;
	//Dibujar todos los puntos que se están tratando
	for (int i = 0; i < pointsVector.size(); i++)
		al_put_pixel(pointsVector.at(i).getX(), pointsVector.at(i).getY(), yellow);

	//Dibujar el convex hull obtenido con el algoritmo QuickHull
	for (auto it = convexHullVector_QH.begin(); it < convexHullVector_QH.end() - 1; ++it) {
		al_draw_line((*it).getX(), (*it).getY(), (*(it + 1)).getX(), (*(it + 1)).getY(), red, 2.0);
	}

	//Dibujar la línea que cierra el convex hull
	al_draw_line((*(convexHullVector_QH.end() - 1)).getX(), (*(convexHullVector_QH.end() - 1)).getY(),
		(*convexHullVector_QH.begin()).getX(), (*convexHullVector_QH.begin()).getY(), red, 2.0);

		
	//Dibujar el convex hull con el algoritmo GiftWrapping
	for (auto it = convexHullVector_GW.begin(); it < convexHullVector_GW.end() - 1; ++it) {
		al_draw_line((*it).getX(), (*it).getY(), (*(it + 1)).getX(), (*(it + 1)).getY(), green, 2.0);
	}

	//Dibujar la línea que cierra el convex hull
	al_draw_line((*(convexHullVector_GW.end() - 1)).getX(), (*(convexHullVector_GW.end() - 1)).getY(),
		(*convexHullVector_GW.begin()).getX(), (*convexHullVector_GW.begin()).getY(), green, 2.0);


	//Dibujar el convex hull obtenido con el algoritmo Incremental
	for (auto it = convexHullVector_IN.begin(); it < convexHullVector_IN.end() - 1; ++it) {
		al_draw_line((*it).getX(), (*it).getY(), (*(it + 1)).getX(), (*(it + 1)).getY(), blue, 2.0);
	}

	//Dibujar la línea que cierra el convex hull
	al_draw_line((*(convexHullVector_IN.end() - 1)).getX(), (*(convexHullVector_IN.end() - 1)).getY(),
		(*convexHullVector_IN.begin()).getX(), (*convexHullVector_IN.begin()).getY(), blue, 2.0);
	


	al_flip_display();

	cout << "Enter para finalizar" << endl;
	char s;
	cin >> s;

	al_destroy_display(display);

    return 0;
}


