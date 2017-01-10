#DCJam 2016 Submission: Dream Crawler
=====================================

This repository contains the source code, binaries, raw assets and
converted assets of the Jumble! Devs submission for the 2016 Dreamcast
Homebrew Game Jam (DCJam 2016).

The submission entry in itch.io is on the following page:

https://jumbledevs.itch.io/dreamcrawler

The entry has several screenshots of the demo, as well as the CDI image
to run it on your Dreamcast / emulator. If just want to try the demo
directly download the CDI image and burn it into a CD. However, if you
want to include the game into a compilation, modify it or create your
own using it as the base, keep reading. :)

# Introduction

Our entry (Dreamcrawler for now on) is a simple 3D demo where we tried
to find out how hard is developing games for the Dreamcast. The demo
consists of three parts:

+ *Engine:* The core engine of the demo handles 3D rendering, scene
management and asset loading. It's developed in the C programming language
and uses the KallistiOS 2.0 library with kosgl to render.
It contains the following submodules:

+ Scene: Rudimentary scene graph handling, includes transform definition
and very simple parenting mechanisms.

+ Renderer: Used to render the 3D scene. Handles rendering meshes and
setting the camera depending on the scene information.

+ Resources: Handles the loading of assets from the CD-ROM. Supports the
load of uncompressed textures, static meshes and bitmap fonts. All the files
must be in the engine's internal format, which is specified below.

+ Debug: Simple shortcut to output debug information through the serial
cable if you're running dc-load or if, like poor people like us, are using
nullDC to debug the game.

+ GUI: Handles the rendering of user interface elements. Supports rendering 
colored and translucent rectangles, textures and bitmap fonts. The font 
rendering is very simple and assumes all fonts to be monospaced.

+ *Game:* The actual code of the demo, basically the main.c file. After developing 
all the logic needed to actually run something on the machine, we didn't have 
too much time to code an actual game, so it isn't that much interesting.

+ *Resource converter:* The tool used to convert assets from the editable formats
used on asset creation (PNG, BMP, OBJ, Blend, DAE, etc) to the engine's custom
formats. The asset converter handles:

+ Static meshes: converts OBJ, DAE, STL, 3DS or BLEND files to the engine internal
mesh file. The input mesh is triangulated and then the triangle vertices, indices,
UV coordinates and normal vectors are saved into the output file. Only the first UV
channel is taken into account. The file must have a single mesh, multi-mesh export
is not supported.

+ Textures: convets PNG, BMP, TIFF into the engine internal texture format. The textures
are saved in RGB888 uncompressed format.

+ Fonts: converts a FNT file compiled with the BMFont Tool to the engine internal
font specification format. When loading a font on the engine, the resulting BINFNT
file must be loaded along the font's texture atlas.

# Building

To build the game, you need a working KallistiOS 2.0 environment. Then, go into the
/dreamcast directory and run:

```
make
```

The /dreamcast/bin directory will be created and if the compilation succeeds, a main.elf
file will be created. Use then the asset data available in /assets to run the game.

To build the resource converter, you will need a working Visual Studio environment or 
maybe Mono. The converter was developed using VS 2015 Enterprise and I don't know if 
it runs elsewhere. If it does, tell me and I'll update! :)

# Using

The demo instructions are available on the jam submission site. There isn't much to do
on it so it won't take long to master. ;)

The resource converter is very simple to use, just drop a file on the executable and the 
converted file will be saved. Only run one at a time, multiple file conversion isn't supported.

# Sharing, copying, editing

The demo and all its source code and asset files are licensed under the GNU General Public License V3. 
This means you're free to copy, distribute, sell, promote and modify the elements of this project 
as long as the license is respected.

If you're interested on doing some Dreamcast development and want to work over this project, 
please get in touch with us. We'll be happy to hear what you're going to do and collaborate!

# Engine file format specification

## Texture files

+ Extension: .raw
+ Format: RGB888 uncompressed texture

|  Field  |  Data type  |  Description  |
|---------|-------------|---------------|
|  type   |   byte unsigned     | 0x00 for uncompressed textures, 0x01 for PVR compressed textures |
|  width  | 2 byte unsigned | Texture width in texels |
| height  | 2 byte unsigned | Texture height in texels |
| data | 3 byte unsigned array (width*height length) | The texture color data in RGB888 format |

## Mesh files

+ Extension: .mesh
+ Format: Uncompressed IEEE-754 vertex, normal, UV data plus integer indices

|  Field  |  Data type  |  Description  |
|---------|-------------|---------------|
| count | 2 byte unsigned | The vertex count of the mesh file |
| vertices | IEEE-754 float array (count*3 length) | Vertex data |
| uvCoords | IEEE-754 float array (count*2 length) | UV Channel 0 data |
| normals | IEEE-754 float array (count*3 length) | Normal data |
| indices | 2 byte unsigned array (count length) | Vertex indices |

## Font files

+ Extension: .binfnt
+ Format: glyph ASCII simbol plus UV location on the texture atlas

|  Field  |  Data type  |  Description  |
|---------|-------------|---------------|
| count | byte unsigned | Glyph count of the file |
| glyphs | glyph structure array (count length) | Array containing the glyph information |

### Glyph structure information

|  Field  |  Data type  |  Description  |
|---------|-------------|---------------|
| id | byte unsigned | ASCII value of the glyph |
| x | byte unsigned | Horizontal offset on the texture atlas |
| y | byte unsigned | Vertical offset on the texture atlas |
| h | byte unsigned | Height of the glyph texture data |
| w | byte unsigned | Width of the glyph texture data |

The glyph location data is stored as unsigned bytes, which are expanded to float values
on load time.
