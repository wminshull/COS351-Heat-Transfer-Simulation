This is a folder for pre-run heat map output files.

You can compile these files into images after building the starterImg.c program. 

NOTE: The heatmaps are square so the img executable only takes one dimension as an argument. 

To create images from these files you'll need to
1. Build starterImg.c using `gcc starterImg.c readData.c -o img`
2. Run the `img` executable with arguments that are correct for the heatmap, i.e.  `./img 1001 < heatmapfilename` (For best results use +1 the size of the heatmap as the argument).
