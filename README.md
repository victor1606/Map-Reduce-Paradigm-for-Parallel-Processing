# Map-Reduce-Paradigm-for-Parallel-Processing
Used the Map-Reduce paradigm in order to process multiple files containing random numbers and storing only the perfect powers(map) and aggregating the final results(reduce)

Calugaritoiu Ion-Victor

main.cpp:
    - extrag numarul de mapperi, de reduceri si numele fisierului de 
text din argumentele din linia de comanda; deschid pe rand toate fisierele 
si le calculez marimea folosind functiile "seekg()" si "tellg()"; adaug 
perechile de tip <nume_fisier, marime_fisier> intr-un vector pe care-l sortez 
dupa marimea fisierelor;
    - sunt initializate mutex-ul, bariera si vectorul de structuri de tip
thread_info; vectorul va avea atatea elemente cate thread-uri sunt lansate,
pentru a asigura ca atunci cand structurile sunt trimise ca parametru functiei 
"parallel()" din "pthread_create()", fiecare thread va avea acces la un 
"thread_id" stocat in zone diferite de memorie; campurile structurilor sunt 
populate cu id-ul corespunzator si cu referinte la structurile de date 
utilizate;
    - am ales sa stochez listele partiale intr-un vector de structuri de tip 
"mapper_list", care la randul lor contin cate un vector de vectori de "int";
    - listele agregate sunt stocate in intr-un vector de vectori 
("reducer_matrix");
    - sunt lansate toate thread-urile (nr de mapperi + nr de reduceri) si
join-uite dupa rulare; sunt distruse mutex-ul si bariera;

parallel.cpp:
    - fiecare thread apeleaza functia "parallel()", din care sunt apelate fie
"mapper()", fie "reducer()", in functie de id-ul thread-ului;
    - mapper(): 
        - cat timp vectorul de fisiere nu este gol, se extrage cate un
    filename de la final, astfel obtinand fisierul cu filesize-ul cel mai mare;
    pop-ul se face intr-un mutex, astfel evitand situatia in care 2 sau mai 
    multe thread-uri incearca sa faca pop la acelasi fisier;
        - fisierul este deschis si parcurs, aplicandu-se functia 
    "perfect_power_fct()" pe fiecare element citit; rezultatul functiei este
    un vector de exponenti (exponentul puterii perfecte), elementele fiind 
    adaugate in listele partiale corespunzatoare;
        - la finalul functiei de mapper este pusa o bariera care asigura ca
    reducerii incep sa prelucreze listele partiale doar in momentul in care
    toti mapper-ii si-au terminat complet procesarea;
    - reducer():
        - aceeasi bariera de la finalul functiei "mapper()" este pusa la 
    inceputul functiei "reducer()";
        - toate listele partiale sunt preluate din "mapper_matrix" si combinate
    in cate un vector din "reducer_matrix" folosind functia "insert()" din stl;
        - elimin toate duplicatele din fiecare vector (functia "unique()") si
    scriu in fisierul de output corespunzator numarul de elemente ramas in
    fiecare vector;

helpers.cpp: perfect_power_fct()
    - functia foloseste cautare binara pentru a cauta eficient bazele care
ridicate la exponent rezulta numarul "nr" primit ca parametru; functia obtine
toti exponentii si ii stocheaza intr-un vector pe care-l returneaza;    
