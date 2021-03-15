# ComputerNetworks
## Tema 1
Proiectati si implementati urmatorul protocol de comunicare intre procese:

- [x] comunicarea se face prin executia de comenzi citite de la tastatura in procesul parinte si executate in procesele copil
- [x] comenzile sunt siruri de caractere delimitate de new line
- [x] raspunsurile sunt siruri de octeti prefixate de lungimea raspunsului
- [x] rezultatul obtinut in urma executiei oricarei comenzi va fi afisat de procesul parinte (partial: pentru o singura comanda)
- [x] protocolul minimal cuprinde comenzile:
    - "login : username" - a carei existenta este validata prin utilizarea unui fisier de configurare
    - "myfind file" - o comanda care permite gasirea unui fisier si afisarea de informatii asociate acelui fisier; informatiile vor fi de tipul: data crearii, data modificarii, dimensiunea fisierului, drepturile de access asupra fisierului etc.
    - "mystat file" - o comanda ce permite vizualizarea atributelor unui fisier
    - "quit"
- [x] in implementarea comenzilor "myfind" si "mystat" nu se va utiliza nicio functie din familia "exec()" (sau alta similara) in vederea executiei unor comenzi de sistem ce ofera functionalitatile respective
- [x] comunicarea intre procese se va face folosind cel putin o data fiecare din urmatoarele mecanisme ce permit comunicarea: pipe-uri interne, pipe-uri externe si socketpair (partial: am folosit un pipe)

## Tema 2
Redactarea raportului tehnic se va face in format LNCS (https://www.springer.com/gp/computer-science/lncs/conference-proceedings-guidelines). Structura generala pe sectiuni a raportului va fi urmatoarea:

-[x] Introducere
-[x] Tehnologiile utilizate (e.g. TCP, UDP... - caracteristicile care au fost utilizate in cadrul proiectului). Justificati de ce ati ales aceste tehnologii.
-[x] Arhitectura aplicatiei (conceptele implicate, diagrama aplicatiei detaliata)
-[x] Detalii de implementare (cod relevant particular proiectului, care va fi documentat; scenarii de utilizare)
-[x] Concluzii (e.g. cum ar putea fi imbunatatita solutia propusa?)
-[x] Bibliografie

## Proiect
Sa se implementeze un client si un server web de baza care vor comunica folosind protocolul TCP. Serverul va contine cateva pagini web (fisiere .html simple) si va pune la dispozitia clientilor urmatoarele comenzi: login (autentificarea la server), list (furnizeaza lista cu fisierele continute), get (copie o pagina web de pe server), add (uploadeaza pe server o pagina web), delete (sterge o pagina web de pe server). Clientul va interactiona cu serverul folosind comenzile oferite.
