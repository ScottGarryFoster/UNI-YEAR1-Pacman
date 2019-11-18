# Pacman Recreation
This project is my university submission for a Pacman like game using S2D. This is **only** to be used for **educational** purposes and  learning how to use S2D, C++ and other such tools to produce a video game. This was built in Visual Studios 2019 and worked at the time, if you'd like to see a preview see the below YouTube video. Additionally to see my portfolio see the link below.

YouTube video: [(https://www.youtube.com/watch?v=FB0SqXEZaF8](https://www.youtube.com/watch?v=FB0SqXEZaF8)

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