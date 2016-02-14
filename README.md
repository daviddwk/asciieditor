# asciieditor
This program was made speficially for making uniform resource files for the program freefish https://github.com/daviddwk/freefish
You may otherwise find this useful for any similar programs

# workings
In asciieditor you made frames of ascii art. These frames consist of charcter and color codes. To the user the color codes are not seen, but the charcters are the color that will be applied to them if the resource is used in freefish. This makes ediitng files much easier as well as how ascii editor will keep your files uniform while a typical text editor will not. In the end asciieditor will outpput a file with text, colorcodes, and frames seperated by an empty line.

# using
On startup asciieditor will ask you for the number of frames, height, and width of your new resource. The height and the width are measured in charcter and the frames are sets of charcters of the given height and width.

The cursor in asciieditor is signified by an asterisk if it is on a space or it is signified by bolding a charcter if it is on a charcter. When a charcter is typed it is put into the space the cursor is on. If you type a space it will replace the charcter with a space. Arrow keys are used to navigate

The escape key can be used for a few functions. Pressing escape followed by any of the following keys will produce the stated results

f Allows you to enter the color you wish the color with. Pressing enter will apply this color to the charcter you currently have your cursor over.

p Allows cause any charcter that your cusor moves over to change to the color currently selected via the f command.
arrow keys THe left and right arrow keys allow you to shift through frames. The frame number is displayed in the upper left corner at all times.

i j k l Allows you to shimmy all of the charcter in the frame. Any charcter that are pushed off the edge are lost. i is up. j is left. k is down. l is right.

r Allows you to change the number of frames, height, or width.

c Allows you to copy to another.

w Allows you to write or save your file to.
