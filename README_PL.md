# **Solitaire – Projekt na konkurs Gigathon 2025**

## 1. Wsparcie

- **Obsługiwane platformy:** Windows 10
    

## 2. Jak uruchomić grę

Aby uruchomić grę, przejdź do folderu `SolitaireGame/Binaries/Release` i uruchom plik `SolitaireGame.exe`.

## 3. Jak skompilować grę

1. Uruchom skrypt `Scripts/Setup-Windows.bat`, który automatycznie wygeneruje pliki projektu za pomocą Premake.
    
2. Otwórz plik `Solitaire.sln` w Visual Studio.
    
3. Wybierz tryb kompilacji:
    
    - **Debug** – do testowania i rozwoju gry,
        
    - **Release** – do kompilacji finalnej wersji.  
        **Uwaga:** Tryb kompilacji wpływa na działanie oraz wydajność gry.
        

## 4. Sterowanie

Sterowanie w grze jest intuicyjne:

- Strzałki **góra** / **dół** – poruszanie się między pozycjami w **Selectorze**. Jeśli coś jest selectorem, obok zobaczysz czerwoną strzałkę.
    
- **Home** – wejście w tryb wpisywania tekstu.
    
- **Esc** – wyjście z trybu wpisywania.
    
- **Enter** – zatwierdzenie wpisanego tekstu.
    

### Komendy sterujące ruchem kart:

1. **Przenoszenie kart między kolumnami w Tableau**  
    Wpisz `1`, zatwierdź Enterem, a następnie wpisz oznaczenia kart:  
    np. `H5 S6` – przesuwa kartę Heart 5 na Spades 6.
    
2. **Przenoszenie karty z Tableau do Foundation List**  
    Wpisz `2`, zatwierdź, a następnie np. `SA` – przesuwa Asa Pik do stosu końcowego.
    
3. **Przenoszenie karty z Waste Pile do Tableau**  
    Wpisz `3`, zatwierdź, a potem podaj kartę docelową, np. `S10`, jeśli chcesz dodać do niej `H9` z Waste Pile.
    
4. **Przenoszenie karty z Waste Pile do Foundation List**  
    Wpisz `4` – jeśli karta spełnia warunki, zostanie automatycznie przeniesiona.
    
5. **Tasowanie Waste Pile**  
    Wpisz `5`, a zużyte karty wrócą na spód Stock Pile, a nowe zostaną odkryte.
    
6. **Anulowanie ruchu**  
    Wpisz `4`, `5` lub `6` – wszystkie te komendy cofają ostatni ruch.
    

## 5. Zarys techniczny projektu

W czasie kompilacji silnik automatycznie generuje listę assetów z folderu `Content` – proces ten odbywa się jednorazowo.

Gra oparta jest na dynamicznej bibliotece `SolitaireEngine.dll`, która dostarcza narzędzi do tworzenia czystego i uporządkowanego kodu.  
Silnik odpowiada za renderowanie gry i obsługę wejść z klawiatury.

> **Uwaga:** Choć renderowanie w pętli może wydawać się mało optymalne, zdecydowałem się na takie rozwiązanie celowo – pozwala ono lepiej symulować dynamiczne zachowanie gry. Różnica w zużyciu zasobów jest praktycznie niezauważalna.

Silnik ładuje assety określone w pliku `AssetList.txt`.

Architektura opiera się na koncepcji **światów** i **aktorów** – każdy element planszy to renderowany aktor.

Zaimplementowano m.in.:

- Loader plików `.wav`
    
- System konfiguracji oparty o pliki `.ini`
    
- Menedżer konfiguracji, który utrzymuje pliki `.ini` w pamięci przez cały czas działania gry
    

## 6. Dodatkowe informacje

- Aby zmienić głośność muzyki, edytuj plik `SolitaireGame/Config/DefaultAudio.ini` i ustaw wartość `MusicVolume` w zakresie od `0.0` do `1.0` (np. `0.55`).
    
- Plik `PlayerScores.ini` przechowuje dane o graczach, którzy ukończyli grę – ostatnie 10 wyników jest wyświetlane w **Menu Głównym**.
    
- Aby zmienić poziom trudności, edytuj plik `DefaultGame.ini` i ustaw wartość klucza `CardsToRevealPerStockUse`.
    

Projekt został w całości przygotowany po angielsku, co pomaga mi rozwijać umiejętności językowe i jednocześnie sprawia, że gra jest dostępna dla osób z całego świata.

---

## Słowniczek terminów

- **Tableau** – główna część planszy, układ początkowy kart
    
- **Foundation List** – stosy końcowe, gdzie budujemy sekwencje kolorami od Asa do Króla
    
- **Stock Pile** – talia, z której dobieramy karty
    
- **Waste Pile** – miejsce, do którego trafiają odkryte karty z talii
    

## Oznaczenia kolorów kart (ENG → PL)

- **Hearts** – Kier
    
- **Diamonds** – Karo
    
- **Spades** – Pik
    
- **Clubs** – Trefl