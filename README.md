# **Solitaire – Project for Gigathon 2025**

## 1. Support

- **Supported platform:** Windows 10
    

## 2. How to Run the Game

To run the game, navigate to the `SolitaireGame/Binaries/Release` folder and launch `SolitaireGame.exe`.

## 3. How to Compile the Game

1. Run the `Scripts/Setup-Windows.bat` script. It will automatically generate the project files using Premake.
    
2. Open `Solitaire.sln` with Visual Studio.
    
3. Choose the build configuration:
    
    - **Debug** – for development and testing,
        
    - **Release** – for the final build.  
        **Note:** The build configuration affects performance and behavior.
        

## 4. Controls

The controls are intuitive:

- Arrow **Up** / **Down** – navigate through positions in the **Selector**. If something is selectable, a red arrow will appear next to it.
    
- **Home** – enter text input mode.
    
- **Esc** – exit text input mode.
    
- **Enter** – confirm typed text.
    

### Card Movement Commands:

1. **Move cards between Tableau columns**  
    Type `1`, press Enter, then type card identifiers:  
    e.g. `H5 S6` – moves Heart 5 onto Spades 6.
    
2. **Move a card from Tableau to Foundation List**  
    Type `2`, confirm, then enter something like `SA` – moves Ace of Spades to the foundation stack.
    
3. **Move a card from Waste Pile to Tableau**  
    Type `3`, confirm, then provide the destination card, e.g. `S10`, if you want to move `H9` from Waste Pile onto it.
    
4. **Move card from Waste Pile to Foundation List**  
    Type `4` – if the card meets requirements, it will be moved automatically.
    
5. **Shuffle Waste Pile**  
    Type `5`, and the used cards will return to the bottom of the Stock Pile while new ones are revealed.
    
6. **Undo last move**  
    Type `4`, `5`, or `6` – all of these commands will undo the previous move.
    

## 5. Technical Overview

During compilation, the engine automatically generates an asset list from the `Content` folder – this process happens once.

The game is built upon a dynamic library called `SolitaireEngine.dll`, which provides tools for writing clean and organized code.  
The engine handles rendering and keyboard input.

> **Note:** Although a rendering loop might seem suboptimal, it was a deliberate choice – it better simulates dynamic game behavior. The performance difference is virtually unnoticeable.

The engine loads assets defined in `AssetList.txt`.

The architecture is based on the concept of **Worlds** and **Actors** – each board element is a renderable actor.

Implemented features include:

- `.wav` file loader
    
- INI-based configuration system
    
- Configuration manager that keeps `.ini` files in memory throughout runtime
    

## 6. Additional Information

- To adjust music volume, edit the `SolitaireGame/Config/DefaultAudio.ini` file and set the `MusicVolume` value between `0.0` and `1.0` (e.g. `0.55`).
    
- The file `PlayerScores.ini` stores data of players who finished the game – the last 10 scores are displayed in the **Main Menu**.
    
- To change difficulty, edit `DefaultGame.ini` and modify the `CardsToRevealPerStockUse` value.

---

## Glossary

- **Tableau** – main board area, initial layout of cards
    
- **Foundation List** – end stacks, where sequences are built from Ace to King by suit
    
- **Stock Pile** – the deck used to draw new cards
    
- **Waste Pile** – area for drawn cards from the stock