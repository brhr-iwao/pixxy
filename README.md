## pixXY

### Overview
Pixxy is a WIN32 application for image measurement. It is pronounced same as pixy [piksi].

### Installation
No installation is needed for a Windows user. Double click the icon "pixxy.exe" and you can run pixxy. Pixxy.exe is packed in the zip file on the "[Releases](https://github.com/brhr-iwao/pixxy/releases)" page.

If you meet the error massage "This application failed to start because the application configuration is incorrect. 
Reinstalling the application may fix the problem." when you try to run pixxy, 
try [vcredist_x86.exe for VC2005 sp1 (Microsoft Visual C++ 2005 SP1 Redistributable Package (x86) )](https://www.microsoft.com/en-us/download/details.aspx?id=5638).

### Usage
1. Run pixxy and select File(F) --> Open Image(O)... to open a image file. 
2. Move the mouse cursor arrow and put the tip on the point where you want to know the x-y coordinate.
You can see the coordinate on the lower left status bar of the window.
Right click and select "Copy the coordinate Ctrl + C" in the pop-up menu,
the coordinate in format of "pixel integer value tab pixel integer value" is copied on the clipboard.
3. You can paste the coordinate on your favorite application (spreadsheet, editor and so on) and use it !

### Photo-anthropometry
#### About the Excel spreadsheet file "fece_measurement.xls".
1. The Excel file consists of the three sheets, "landmarks", "dimensions" and "indices". It can be opened on Microsoft Excel, Kingsoft Spreadsheet etc.
2. Open a face photograph on pixxy.
3. Copy pixel coordinate values of an anthropological landmark to clipboard and paste it on the appropriate fields on the "landmarks" sheet.
4. If you complete the above 2 and 3 procedures for adequate anthropological landmarks, anthropological indices are automatically calculated on the "indices" sheet.
5. If you complete until the above 4th procedure, some anthropological classification are done.
6. If you select "male" on the sexuality drop-down list on the "indices" sheet, the most similar population among Japanese, Germany, Italy and Lithuania is shown in the viewpoint of anthropometry.
7. The author also referred to the book "Somatometry", in Anthropology 13 vols. and 3 additional vols., Yuu-zan kaku Publisher, Tokyo, 1991,ISBN4639010427 (in Japanese).
I think that most of users can't read this book and I did not put it on reference-lists in the Excel sheet.

#### Examples
1. The former President of the U.S.
    Mr. Obama has the very narrow face in the viewpoint of anthropological total face index.
However it needs to be considered that he is smiling and opening slightly mouse in the photograph, it leads to higher the total face height.
He has mid face in a viewpoint of anthropological physiognomic upper face index which has no effect from mouse opening.
It can be said that nasal width is under influence of the expression. Smiling leads to wider nasal width.

2. The incumbent President of the U.S.
   His face has similarity with Japanese in viewpoints of anthropometry (total face index and relation among en-en:al-al:ch-ch).

These photographs were not very appropriate because Mr. Obama is smiling and Mr. Trump looks half profile and opens slightly his mouse...

### Acknowledgement
The author is very grateful to Mr. [Sean Barrete](https://nothings.org/) for development and release of [the stb library](https://github.com/nothings/stb) which is very programmer-friendly image loading and writing library.
The author is also grateful to  Mr. [Okamoto Takeshi](https://www.petitmonte.com/) for presenting [an example C source of image-previewing WIN32 common dialogbox](https://www.petitmonte.com/c_cplus/capi_sample52.html). (Sorry! Link distinations are Japanese only).
