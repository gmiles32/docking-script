/*
* Author: Gabe Miles
* Date: 3/23/2022
* Description: Takes in a receptor file (protein), and a directory
* containing ligand files, formats them correctly, and runs them through
* Vina, a ligand-protein interaction modeling software.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_BUFFER_LENGTH 4096

static int usage(void){
    fprintf(stdout, "Usage: ./testligand -r <receptor path> -l <ligand path> -s <search space> -o <output filename>\n");
    fprintf(stdout, "\t-r <receptor path>   Path to receptor file, in .pdbqt format - already been processed\n");
    fprintf(stdout, "\t-l <ligand path>     Path to .sdf file containing all the ligands to be tested, unsplit\n");
    fprintf(stdout, "\t-s <search space>    Path to .txt file containing search space parameters. Use Chimera to find this.\n");
    fprintf(stdout, "\t-h                   Outputs this message\n");
    return 0;
}

char* itoa(int val, int base){
	
	static char buf[32];
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}

int main (int argc, char * argv[]){
    char receptorPath[MAX_BUFFER_LENGTH];
    char ligandsPath[MAX_BUFFER_LENGTH];
    // char ligandsPath[] = "for-sale.sdf";
    char configPath[MAX_BUFFER_LENGTH];
    char outputPath[MAX_BUFFER_LENGTH];

    FILE * allLigands;
    char line[MAX_BUFFER_LENGTH];
    char ligandFileName[MAX_BUFFER_LENGTH];

    DIR * ligandDir;
    struct dirent * dir;

    // Gets command line arguments from user
    char opt;
    while ((opt = getopt(argc, argv, "r:l:s:o:h")) != -1){
        switch (opt){
            case 'r':
                strncpy(receptorPath, optarg, MAX_BUFFER_LENGTH);
                break;
            case 'l':
                strncpy(ligandsPath, optarg, MAX_BUFFER_LENGTH);
                break;
            case 's':
                strncpy(configPath, optarg, MAX_BUFFER_LENGTH);
                break;
            case 'h':
                usage();
                exit(0);
            default:
                usage();
                exit(1);
        }
    }

    // The following code is unnecessary, as in its more trouble than its worth since no 
    // automation is really gained

    // //Converts .pdb receptor file to .pdbqt, outputs to the same directory
    // if (strstr(receptorPath, ".pdb") != NULL){
    //     //Gets the path up until filename
    //     char * newLigand = strrchr(receptorPath, '/');
    //     size_t pathLength = strlen(receptorPath) - strlen(newLigand);
    //     char receptorDirPath[pathLength];
    //     strncpy(receptorDirPath, receptorPath, pathLength);
    //     //No null character on new string, needs to be added
    //     receptorDirPath[pathLength] = '\0';
    //     printf("The target directory path is: %s\n", receptorDirPath);

    //     //Exhanges .pdb for .pdbqt in filename for output file
        // size_t outputFileLength = strlen(newLigand) + 2;
        // char pdbqtReceptor[outputFileLength];
        // strncpy(pdbqtReceptor, newLigand, strlen(newLigand) - 4);
        // strcat(pdbqtReceptor, ".pdbqt");
    //     printf("The .pdbqt output file is: %s\n", pdbqtReceptor);

    //     //Adds the path to the new output filename
    //     char pdbqtReceptorPath[pathLength + outputFileLength];
    //     strcpy(pdbqtReceptorPath, receptorDirPath);
    //     strcat(pdbqtReceptorPath, pdbqtReceptor);
    //     printf("The output file path is: %s\n", pdbqtReceptorPath);

    //     //Make command
    //     char command[MAX_BUFFER_LENGTH];
    //     strcat(command, "./~/Programs/ADFRsuite/bin/prepare_receptor -r ");
    //     strcat(command, receptorPath);
    //     strcat(command, " -o ");
    //     strcat(command, pdbqtReceptorPath);

    //     //Execute
    //     system(command); 

    // } else {
    //     fprintf(stderr, "Receptor file not of format '*.pdb'\n");
    //     exit(1);
    // }

    printf("Making directories to store files...");

    //Makes storage directories for made files
    system("mkdir test-ligands-sdf");
    system("mkdir test-ligands-pdbqt");
    system("mkdir results");

    printf("Complete\n");
    printf("Formatting ligand files...");

    //Seperates ligand files from big ligand 
    char ligandFilePrefix[] = "test-ligands-sdf/ligand";

    //Makes new sdf file, creates pdbqt file, deletes sdf file
    if (strstr(ligandsPath, ".sdf") != NULL){
        allLigands = fopen(ligandsPath, "r+");
        int cnt = 1;

        //There's gotta be a better way of doing this without this repeat code
        strcpy(ligandFileName, ligandFilePrefix);
        char * buffer = itoa(cnt, 10);
        strcat(ligandFileName, buffer);
        strcat(ligandFileName, "H.sdf\0");
        FILE * newLigand = fopen(ligandFileName, "w");

        while (fgets(line, MAX_BUFFER_LENGTH, allLigands) != NULL){
            if (strchr(line, '$') == NULL){
                fprintf(newLigand, "%s", line);
            } else {
                fclose(newLigand);

                //Make pdbqt file
                char command[MAX_BUFFER_LENGTH];
                strcpy(command, "python3 ~/Programs/Meeko/scripts/mk_prepare_ligand.py -i ");
                strcat(command, ligandFileName);
                strcat(command, " -o ");
                //Exchanges .sdf for .pdbqt in final file name
                char pdbqtLigand[MAX_BUFFER_LENGTH];
                strcpy(pdbqtLigand, "test-ligands-pdbqt/ligand");
                strcat(pdbqtLigand, buffer);
                strcat(pdbqtLigand, ".pdbqt");
                strcat(command, pdbqtLigand);

                //Execute
                system(command);

                cnt++;

                //Make new ligand file
                strcpy(ligandFileName, ligandFilePrefix);
                buffer = itoa(cnt, 10);
                strcat(ligandFileName, buffer);
                strcat(ligandFileName, "H.sdf\0");
                newLigand = fopen(ligandFileName, "w");
            }
        }

        //For last file - which shouldn't exist in the first place
        fclose(newLigand);

    } else {
        fprintf(stderr, "File not in .sdf format\n");
        exit(1);
    }

    fclose(allLigands);

    //Remove unnecessary files
    system("rm -r test-ligands-sdf");

    printf("Complete\n");
    printf("Running vina...");

    //Run vina
    ligandDir = opendir("test-ligands-pdbqt");
    int cnt = 1;
    while ((dir = readdir(ligandDir)) != NULL){
        char vinaOut[MAX_BUFFER_LENGTH];
        strcpy(vinaOut, "results/vina-out-ligand");
        char * buffer = itoa(cnt, 10);
        strcat(vinaOut, buffer);
        strcat(vinaOut, ".pdbqt");

        if (strstr(dir->d_name, ".pdbqt") != NULL) {
            char command[MAX_BUFFER_LENGTH];
            strcpy(command, "vina --receptor ");
            strcat(command, receptorPath);
            strcat(command, " --ligand test-ligands-pdbqt/");
            strcat(command, dir->d_name);
            strcat(command, " --config ");
            strcat(command, configPath);
            strcat(command, " --exhaustiveness=32 --out ");
            strcat(command, vinaOut);

            system(command);

            cnt++;
        }
    }

    closedir(ligandDir);

    printf("Complete\n");
}