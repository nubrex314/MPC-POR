# Termostat
Autor Martin Sukdolak

Dokumentace
1.1	Zapojeni:
ATMega328P	Periferie
PB0	LCD-RS
PB1	LCD-E
PB2	
PB3	Speaker
PB4	Generátor
PB5	LED 
PC0	LCD-Tlačítka
PC1	Teploměr
PC2	Rele-chalzeni
PC3	Relé-topeni
PC4	TWI-SDA	
PC5	TWI-SCL
PD0	Debug-TX
PD1	Debug-RX
PD2	
PD3	
PD4	LCD-D4
PD5	LCD-D5
PD6	LCD-D6
PD7	LCD-D7

2.1	Popis programu
a.	Main
Program pracuje s 32 B polem (data_EEPROM) které je ukládané do paměti EEPROM. Na jednotlivých indexech jsou uloženy hodnoty dle tabulky:
Index	funkce	Rozsah hodnot
0	RTC-sekundy	0-60
1	RTC-minuty	0-60
2	RTC-hodiny	0-60
3	RTC-dny	0-31
4	RTC-měsíce	0-12
5	RTC-roky	0-255
6	Hystereze-cele číslo	0-255
7	Hystereze-desetině číslo	0-255
8	Kalibrace-cele číslo	0-255
9	Kalibrace-desetině číslo	0-255
10	Teplota topeni-cele číslo	0-255
11	Teplota topeni-desetině číslo	0-255
12	Teplota chlazeni-cele číslo	0-255
13	Teplota chlazeni-desetině číslo	0-255
14	Kalibrace-znaménko	0,1
Po inicializaci všech součástek se vyčte hodnota z EEPROM a uloží se do data_EEPROM tyto data se následně zapíši do RTC a spustí se znělka super Mario. Tato znělka slouží pro nevidící aby věděli, že booting je u konce.
Hodnoty jsou v hlavni smyčce programu ukládaný minimálně každých 5 sekund podle generátoru z RTC který je nastaveny na 1 Hz. Každých 5 s se vyčte hodnota z RTC a uloží se do data_EEPROM a následně se vypíše na displej. Na zaklade znaménka kalibrace (index 14) se přičte či odečte hodnota kalibrace od hodnoty změřené pomoci ADC na termistoru. Tato hodnota se vypíše na displej a zkontroluje se na zaklade ni, zda je potřebné přepnout relátka (funkce relátek dále v dokumentaci). Nakonec se znovu data v data_EEPROM uloží do EEPROM.
Program dále reaguje na další 3 události
1.	Zmačknuti tlačítka displeje (menu LOCAL, menu Teplota)
2.	Přijati správy „r“ z UART (menu REMOTE)
3.	Zmačknuti tlačítka pod displejem (blind mode)
Po prvních dva se vola daná funkce menu a následně se zapisuje do RTC a EEPROM. Pro třetí se zavolá funkce pro přehráni hodnoty teploty v Morseově abecedě.
 

b.	Menu
Jsou implementovaný 3 funkce menu:
1.	Menu – ovládaní pomoci tlačítek pod displejem
2.	Menu topeni – ovládaní pomoci tlačítek pod displejem
3.	Menu rem – ovládaní pomoci UART  
Do prvního menu se dostane uživatel zmačknutím tlačítka SELECT. Pomoci RIGT/LEFT je možné se pohybovat mezi jednotlivými stavy a měnit tak konkrétní hodnoty jednotek v data_EEPROM. V jednotlivých stavech je možné zmačknout tlačítko pod displejem a přehrát tak jednotky zobrazené na displeji. Toto menu pracuje pouze s celými čísly a slouží tak pro zběžné nastaveni hodnot a přehráni hodnot pomoci repráku. Z tohoto menu se uživatel dostane bud mačkáním LEFT až za hodnotu minut nebo mačkáním RIGHT až za hodnotu Teploty chlazeni.
Druhé menu je přístupné v hlavni obrazovce pomoci zmačknuti LEFT/RIGHT, UP/DOWN. Zmačknutím se začne měnit hodnota topeni a chlazeni v celých číslech. Každých 5 s se obrazovka vrátí do hlavni obrazovky. Není možné se dostat do záporných hodnot a zároveň není možné zvýšit hodnot chlazeni tak aby přesahovala hodnotu topeni minus hystereze a naopak. V takových případě se zobrazí varovná hláška.
Třetí menu je přístupné přijatým znaku „r“ pomoci UART. Následně si uživatel dle dialogu vybere, kterou hodnotu chce upravovat. Pro hodnoty z RTC se zadává postupně hodnota desítek a potom jedniček. Před každým zápisem se vypíše aktuální hodnota. Pro hodnoty teploty je potřebné zapsat hodnotu desítek jedniček a po vypsán aktuální hodnoty desetin zapsat postupně desetiny a následně stotiny. Pro kalibraci je potřebné zapsat eště hodnotu znaménka + či -. V případě že uživatel zadal spatně první číslo nebo chce ponechat aktuální hodnotu muže zmačknout klávesy ENTER. Z tohoto se uživatel dostane zmačknutím klávesy ESC nebo zmačknutím tlačítka pod displejem.

 
c.	Relé
Termostat funguje na zaklade 3 stavové logiky s hysterezi:
 
Při inicializaci se vypnou obě relátka. Program pracuje s tím že ví jestli topí nebo chladí a aktuální hodnotu teploty. Na zaklade těchto informaci upravuje výstup relátek dle stavového diagramu:
 
3.1	Inovace
a.	reproduktor
V tomto projektu je použitý reproduktor za účelem signalizaci pro nevidící. Reproduktor funguje na zaklade rychlého zapínaní a vypínaní pinb3 podle cyklu procesoru. Toto přepínaní trvá daný počet cyklu a vytváří tak ton na reproduktoru. Reproduktor je použit při jakémkoliv přečteni tlačítka mimo NONE, úvodní znělce a morseovce. 
Zmačknuti tlačítka je signalizováno vždy o trochu jíním tonem pro rozeznaní o které tlačítko se jedna. 
Morseova abeceda je převedena z pole kde jsou uloženy hodnoty jednotlivých číslic pomoci jedniček a nul, kdy jednička znázorňuje čárku a nula tečku. Pro hodnoty čárky a tečky jsou přehrány dane tony. Po ukončeni čísla je zahrán ton ukončeni. 
Znělka je vytvořena pomoci pole hodnot představujících tony a mezery pro hodnotu 0. Díky tomu je možné tvořit různě dlouhé tony a mezery s konstantní délkou 50 cyklu.
Problém tohoto řešeni nastává v případě že dojde přerušeni a aktuálně se přehrává tón. V tomto případě jsou lehce slyšet záseky v dane melodii. 
b.	Krabička
Krabička byla nadizajnovaná za zlepšením vizualizace, kryti a zabránění mačkaní tlačítka RESET.


