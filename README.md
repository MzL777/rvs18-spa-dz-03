# rvs18-spa-dz-02
## Domaća zadaća 02 za SPA u 2018
### ROK PREDAJE: 25.6.2018. 08:59
SPA domaća zadaća 03
Cilj ove domaće zadaće je osnažiti studentovo znanje rada s algoritmima traženja.
### Zadatak 1 (max 1 bod iz ishoda 4)
U prilogu je aplikacija kakvu trebate napraviti (Zadatak1.exe), a sastoji se od sljedećeg: prilikom
pokretanja aplikacije korisnik unosi redak i stupac početne točke A i redak i stupac krajnje točke B (broj
retka ide od 1 do 20, a broj stupca od 1 do 40). Nakon toga, aplikacija treba osmisliti put od točke A do
točke B i svakih 100 milisekundi iscrtati gdje se trenutno nalazimo. Točka A i točka B se mogu nalaziti na
bilo kojem dopuštenom mjestu (isprobajte kako radi aplikacija u prilogu s A = 1, 1 i B = 20, 40, te kako
radi s A = 20, 20 i B = 10, 10).
### Zadatak 2 (max 1 bod iz ishoda 4)
Prethodna aplikacija je odlična u slučaju kad nema neprelaznih prepreka. Međutim, ako se na igralištu
pojave zidovi, morat ćemo mijenjati način dolaska od A do B tako da ne prolazimo kroz zidove, već da ih
zaobiđemo. Druga aplikacija (Zadatak2.exe) demonstrira takav način rada (isprobajte kako radi aplikacija
u prilogu s A = 1, 1 i B = 1, 40 – uredno ćemo zaobići zid i stići do cilja. Navedeni princip vrijedi koliko god
zidova postavimo na igralištu).
Vaš zadatak je isprogramirati aplikaciju tako da se igrač na putu od A do B zaobilazi zidove i sretno stiže
do odredišta. Postoji puno načina za to napraviti, a najjednostavniji je primjenom Dijkstrinog algoritma
pronalaska najkraćeg puta (https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm). Odlična
implementacija koju smijete koristiti (smijete i druge, naravno, a možete napisati i svoju, ali ova je
najkraći put do rješenja): http://rosettacode.org/wiki/Dijkstra%27s_algorithm.
### Bonus
Sva rješenja zadatka 2 napravljena u SFML-u ulaze u natjecanje za bonuse u iznosima od 1, 2 i 3 dolara.
Bonusi će biti podijeljeni prema vizualnom WAAAAUUUU efektu izazvanom kod asistenata, nastavnika te
kolega studenata.
### Predaja rješenja
Kad ste gotovi, rješenje stavite na svoj GitHub i pošaljite svom asistentu e-mail s linkom.