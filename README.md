# docking-script

## Download dependencies

To convert a .pdb receptor file to .pdbqt, it is necessary to download and compile [ADFRsuite](https://ccsb.scripps.edu/adfr/downloads/).
I downloaded this on Ubuntu, and the compiling was pretty straightforward. Next, you need to download a python package called [Meeko](https://pypi.org/project/meeko/) which
will be used to turn receptor .sdf files into .pdbqt files. If you follow the instructions to download it with Anaconda it will make your life much easier. 
I'm assuming you've already done this, but you can also download AudoDock Vina from conda as well.

## Download and compile C code

I wrote all this code and compiled it on Ubuntu (Linux) using the native gcc compiler. I do not know if it will work exactly on another machine, just because
C is cranky like that. It should work on BoiseStateBudgie (the CS department linux distro), so you can always do that. Once downloaded and compiled,
you will need to make an to the code, just in terms of the path the mk_prepare_ligand.py command. I will make a comment where these changes need to be made.

## Running the code

This program takes in three inputs: 
1. A .pdbqt file of the receptor file, alread prepared with ADFRsuite (here's a [link](https://autodock-vina.readthedocs.io/en/latest/docking_basic.html) showing you how to do that)
2. A .sdf file containing ALL ligands to be tested by the script. An sdf file can be retrieved from the [ZINC database](https://zinc.docking.org/substances/) if
you click on the dropdown by the page numbers or by the compound count.
3. A config file for vina. This is the search space being used, and I will upload a template .txt file. Coordinates can be obtained in Chimera, though I haven't
gotten that far yet.

With all of that, it should (hopefully) run. Let me know obviously if you have issues, or we can figure out a time to work through it together.

## Next steps

Though the program runs and returns results files, I have no good way of tabulating results to return the best ones. I'm currently brainstorming a way to do this involving
some file parsing and eithe a queue or double linked lists to store the like top ten ligands, but any ideas would be welcome. Also, all of this might have
been completely unnecessary because we won't use it, but it was fun in the moment!

[Here's a tutuorial I found for doing all command line docking with vina](https://autodock-vina.readthedocs.io/en/latest/docking_basic.html)
