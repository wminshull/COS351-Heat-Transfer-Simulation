# COS351-Heat-Transfer-Simulation
University project using Jacobian iterations and Von Neumann neighborhood to simulate heat transfer

You can run this program serially or using OpenMP, but be warned the serial version will take a VERY long time to complete with the default settings (no arguments). 

To build and run this using the OpenMP implementation:
1. `gcc -fopenmp heatomp.c -o heatomp`
2. `./heatomp arg1 arg2` 
* arg1 and arg2 are height and width dimensions of the grid. If you specify 1 or none of these the program will run at its default grid size of 1,000 x 1,000. You MUST specify both in order to get a custom grid size. 

To build and run the Serial version of this program:
1. `gcc heatserial.c -o heatserial`
2. `./heatserial arg1 arg2` 
* Again, you MUST specify both dimensions of the grid (`arg1` and `arg2`) if you want the program to run with a custom grid size. 
By default this program will print a message every 100 iterations, telling you what iteration it's on, and it will produce a heatmap every 10,000 iterations. It will end after the default 100,000 generations.

The heatmaps generated from these programs are will be generated as `heatput'k'` with `'k'` being the number of heatmaps the program has generated so far. The program will generate `heatput0` as soon as the grid is done being initialized, so the number at the end of each heatmap file corresponds to the number of tens of thousands of iterations the program has completed i.e. `heatput1` is the resulting heatmap after the program has made it through 10,000 iterations. 

To create images out of these files:
1. `gcc starterImg.c readData.c -o img`
2. `./img arg1 < heatput0`
* Until I can fully debug this you'll need to set `arg1` to the size of the heatmap grid + 1. For example, using default settings of the programs, the resulting heatmaps can be converted to images using `./img 1001 < heatput0` where `heatput0` is the name of the heatmap you want to generate an image of. 
