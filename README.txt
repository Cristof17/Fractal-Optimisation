Cristofor Rotsching 333CA

Pentru implementarea temei am optimizat urmatoarele:

Fractal.c
	Functia Prepare_Fractal: implementarea initiala apela functia Process_Fractal care
	intoarce dimensiunea structurii	primite ca parametru prin pozitionarea a 2 pointeri.
	Apelurile functiei le-am inlocuit direct cu sizeof(struct Fractal) pentru optimizarea schimbarii de context.
	Deoarece folosirea functiei Prepare_Fractal este redundanta, functiile folosite de in corpul acesteia 
	sunt de asemenea redundate.
		
	Array-ul colors este redundant asemenea variabilei used.

	In interiorul Functiei Wrap() se itera pana la o valoare stabilita pentru
	a decrementa o valoare. Optimizarea consta in scadearea valorii de iteratie
	direct din variabila ret.

	Asignarea valorii de 0 fiecarui element din vectorul colors din functia
	acquire_line este de asemenea redundanta.

	In interiorul functiei iterate() sunt prea multe alocari pe stiva si dealocari 
	prin apelarea diferitelor functii pentru calcularea valorii length. 
	Pentru aceasta am implementat operatiile matematice pentru inmultirea a doua
	numere complexe si am inlocuit apelul functiei cabs() cu formula de calculare 
	(x^2 + y^2)^(1/2). Daca scoatem logaritm din aceasta functie rezulta 1/2 * log
	= 0.5 * log

	Operatiile ce implica numere complexe, inmultirea si adunarea le-am scris conform
	forulelor matematice doarece prin analiza cu perf am vazut ca a doua cea mai apelata
	functie 30% din timpul de executie este petrecut in functia de inmutire a doua numere
	complexe.
	
Image.c

	In interiorul sursei Image.c in functia image_create() se aloca memorie pentru
	variabila header si se itera pe fiecare element al sau pentru a initializa
	array-ul cu 0, apoi se facea free(). Sunt operatii redundante.

	Cele 2 for-uri din interiorul functiei image_close() le-am redusdoar un singur for(), iar
	in locul for-ului care scrie caracter cu caracter in fisierul de output, am inlocuit cu 
	pe toata lungimea array-ul de culori astfel incat sa nu mai fie necesara calcularea
	iteratorului i.
