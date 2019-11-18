# Pacman Recreation
This project is my university submission for a Pacman like game using S2D. This is **only** to be used for **educational** purposes and  learning how to use S2D, C++ and other such tools to produce a video game. This was built in Visual Studios 2019 and worked at the time, if you'd like to see a preview see the below YouTube video. Additionally to see my portfolio see the link below.

YouTube video: [https://www.youtube.com/watch?v=FB0SqXEZaF8](https://www.youtube.com/watch?v=FB0SqXEZaF8)

My portfolio: [https://ScottGarryFoster.com](https://ScottGarryFoster.com)

# Reflection on the Project
## Introduction
As an introduction to C++ as a whole I was given a framework called S2D which I believe is based on OpenGL however we were not to touch the more complicated things during this project and instead concentrate on creating a game form first principles.

The end results was a fairly faithful recreation of Pacman with ghost AI close to the original and path-finding.

I created an image manual for the submission which can be seen in the video as I took pages from this and incorporated it into the showcase. The map itself is rather interesting as I used a tile-map and then had a few switch statements select the correct tile to show based on the surrounding areas, meaning that the maps in the game are not simply images of the maps but instead files informing the game of where Pacman may go.

Completed in just over 3 months this project taught me C++ through games programming and although this is not my best work it does demonstrate my learning when paired with later projects.

## Overall Reflection
Considering this way my first project I feel the results are rather nice. I had a completed Pacman game with my own graphics and music without C++ knowledge. The levels load from file which is no small feat. and the overall quality of the product is impressive. I did happen to start again with this project once through out it's lifetime as I felt I knew far more about encapsulation and methods of organizing the classes. I look back on this project now with all I know and there are far less classes than I use today, everything is just kept in big sets of code, I'll be it I did encapsulate a little. There are no major bugs that I know of in this project which is rather nice and I owe that to me recreating the movement system three times in order to get it feeling right. The scoreboard system is also a nice feature and I like how I added name entry.

## Sections I'm proud of
There's a lot of good in here for a first project, the path-finding, the way the tile-map works and so on. This is a project I'll hold a place in my heart for.

### A Star Path-finding
Something I wanted in here from the start was A star path-finding. This is a technique of path-finding aiming to find the shortest route between two points. This system in my game works and even works through the screen wraps somewhat. The way this works is a few tiles beyond the screen warp the tiles will start considering the warp as an option and see if this number of less than not using the screen warp. In short, if it's shorter to go through the screen warp it'll start telling the AI of this a few steps away from it.

Where I feel this falls short is simply in my knowledge of the language and system. It works however if I were to instead make a node based system this wouldn't need so many breakouts. Additionally it would use less memory and run far better than it currently does.

### Ghost AI
So the ghosts follow the proper AI. I'll be it the original game did not have A star path-finding however the ghosts will act similar to the original game. The red ghost will aim straight for you at all times, the pink ghost will attempt to aim in front of you, the blue ghost will attempt to get between you and the red ghost and the orange will get close till it runs away. These are all in here along with scatters and the release times. It's really a tribute to how much I was attempting to simulate the actual game.

I will say the AI class in this is very messy, it needs to be split off into smaller pieces and encapsulated. Additionally programming the blue ghost was difficult and I'm not even sure it 100% works.

### Fluid Tunnels
The tunnels were not given to us as part of the project, they basically just had the sprite move to the other side of the screen when it left the screen. I on the other hand decided early on to had two sprites for the player one drawn in normal play and the other only drawn to have this smooth transition. The way Pacman moves of the screen he'll start to appear on the other-side during the transition literally pixel by pixel. This feels much smoother, further more the fluid tunnels may occur in any direction and works for all the ghosts.

The changes to this system would be the overall movement system. There are a lot of checks and balances which could be removed by thinking of the system as an encapsulated movement system to be applied to all characters. Similar to how did this in the future Super Mario Bros. project I'd attach a movement to a character meaning it could be bespoke.

### Loading Levels from file / tile-map creation

The way the map is drawn is not simply an image per level but each tile of non-movable area is it's own tile. The game then loads in the level map and decides which tile to display on all the non-movable area. For example if a movable tile is to the South East of you and everything else is non-movable then you must be the top left corner tile. You extract this to the entire map and maybe do a second pass for some more difficult tiles and you have a completely fluid system.

If I were to do this again I think I'd attempt to clean up the switch statements and maybe implement a tile interface/polymorph class so that each tile could be customized by passing a different character.

### Scoring System and High-score board
The scoring system might not sound like a lot but on top of collecting the bits you have to multiply the score for every ghost caught on a particular vulnerable run. This means if you can capture say three ghosts from one special bit you'll gain more points than capturing three ghosts separately from three special bits. Additionally the high-score board allows you to enter in your name and features the backspace key. These had to figured out manually within the system and shows off how the text system works. Further more the scores order from greatest to least.

There's not a lot of improving to be done on this system except a general clean up. I feel the system for recognizing the score position the current score should be entered into could be much neater.

## Parts I'm less proud of

## Overall Improvements

