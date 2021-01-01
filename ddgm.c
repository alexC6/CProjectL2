#include "ddg.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xmlstring.h>

#define TRUE 1
#define FALSE 0
#define NBE 21

typedef int BOOLEEN;

/* Affichage de la liste des commandes */
void ddgm_h();

/* Affichage de la version et du copyright */
void ddgm_v();

/* Mise en structure du fichier XML */
ddg_t *ddgm_parse_ddg(xmlDocPtr, xmlNodePtr);

/* Fonction auxiliaire à la précédente permettant le remplissage de la partie date de la structure ddg */
void ddgm_parse_date(xmlDocPtr, xmlNodePtr, ddg_t *);

/* Fonction auxiliaire à la précédente permettant le remplissage du tableau des joueurs de la structure ddg */
int ddgm_parse_players(xmlDocPtr, xmlNodePtr, ddg_t *);

/* Découpage de la commande en un ou deux opérateurs */
void command_cut(char *, char *, char *);

/* Somme de la valeur ASCII des caractères d'une chaine de caractères */
int str_sum(char *);

/* Vérifie qu'une commande saisie existe à partir de la somme de sa chaine de caractère */
BOOLEEN command_test(int, int *);

int main(int argc, char *argv[]) {
    ddg_t *ddg;
    xmlDocPtr file;
    xmlNodePtr node;
    int arg1, opNum, i;
    char strCommand[15], op1[5], op2[11], *endptr;
    int command_list[NBE] = {100, 103, 104, 109, 110, 112, 113, 118, 209, 211, 216, 222, 321, 413, 418, 420, 425, 428, 433, 435, 440};

    if (argc < 2) {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }

    /* Initialisation de la structure XML puis de la structure ddg */
    file = xmlParseFile(argv[1]);

    if (file == NULL) {
        printf("I/O warning : failed to load external entity \"%s\"\n", argv[1]);
        return 1;
    }

    node = xmlDocGetRootElement(file);

    if (xmlStrcmp(node->name, (const xmlChar *) "ddg") == 0) {
        ddg = ddgm_parse_ddg(file, node);
    }
    else {
        printf("Bad file\n");
        printf("DDG not found\n");
    }

    if (ddg == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    /* Lancement de l'"interface" du programme */
    do {
        /* A chaque tour, on initialise le contenu des chaines contenant les opérateurs pour éviter tout problème
         * Le programme récupère la chaine de caractère saisie par l'utilisateur
         * Il découpe ensuite cette chaine en deux opérateurs, puis fait la somme des caractères du premier
         * Et on récupère la valeur numérique du second
         * La suite varie en fonction de la valeur de l'opérateur 1 (arg1)
         * Pour les commandes à double paramètre, on vérifie la présence du second
         * Pour les commandes à paramètre numérique, on vérifie que le second paramètre contient des chiffres
         */
        for (i = 0; i < 5; i++) {
            op1[i] = '\0';
        }

        for (i = 0; i < 11; i++) {
            op2[i] = '\0';
        }

        for (i = 0; i < 5; i++) {
            strCommand[i] = '\0';
        }

        printf("DDGM> ");
        fgets(strCommand, 15, stdin);
        command_cut(strCommand, op1, op2);
        arg1 = str_sum(op1);
        opNum = (int) strtol(op2, &endptr, 10);

        if (!(command_test(arg1, command_list))) {
            fprintf(stderr, "Invalid command\n");
            continue;
        }

        if ((arg1 > 200) && (strlen(op2) == 0)) {
            fprintf(stderr, "Missing parameter for %s command\n", op1);
            continue;
        }

        if (((arg1 == 209) || (arg1 == 216) || (arg1 > 400)) && (endptr == op2)) {
            fprintf(stderr, "Invalid parameter for %s command\n", op1);
            continue;
        }

        switch (arg1) {
            case 100:
                ddg_handle_d(*ddg);
                break;
            case 103:
                ddg_handle_g(*ddg);
                break;
            case 109:
                ddg_handle_m(*ddg);
                break;
            case 104:
                ddgm_h();
                break;
            case 110:
                ddg_handle_n(*ddg);
                break;
            case 112:
                ddg_handle_p(*ddg);
                break;
            case 209:
                ddg_handle_pa(*ddg, opNum);
                break;
            case 413:
                ddg_handle_page(*ddg, opNum);
                break;
            case 428:
                ddg_handle_pagt(*ddg, opNum);
                break;
            case 418:
                ddg_handle_pale(*ddg, opNum);
                break;
            case 433:
                ddg_handle_palt(*ddg, opNum);
                break;
            case 211:
                ddg_handle_pc(*ddg, op2);
                break;
            case 321:
                ddg_handle_pcn(*ddg, op2);
                break;
            case 216:
                ddg_handle_ph(*ddg, opNum);
                break;
            case 420:
                ddg_handle_phge(*ddg, opNum);
                break;
            case 435:
                ddg_handle_phgt(*ddg, opNum);
                break;
            case 425:
                ddg_handle_phle(*ddg, opNum);
                break;
            case 440:
                ddg_handle_phlt(*ddg, opNum);
                break;
            case 222:
                ddg_handle_pn(*ddg, op2);
                break;
            case 118:
                ddgm_v();
                break;
        }
    } while (arg1 != 113);

    ddg_free(ddg);

    xmlFreeDoc(file);

    xmlCleanupParser();

    return 0;
}

void ddgm_v() {
	printf("DDGM (Dungeons and Dragons Game Manager) 20191211\n");
	printf("\nCopyright (C) 2019 Alexandre Coulais and Quentin Hourcadette.\n");
	printf("\nWritten by Alexandre Coulais <alexandre.coulais@etud.univ-pau.fr> and Quentin Hourcadette <quentin.hourcadette@etud.univ-pau.fr>\n");
}

void ddgm_h() {
	printf("d: Prints the DDG date\n");
	printf("g: Prints the DDG\n");
	printf("m: Prints the DDG dungeon master name\n");
	printf("h: Prints this help\n");
	printf("n: Prints the DDG name\n");
	printf("p: Prints the DDG players\n");
	printf("pa AC: Prints the DDG players with the armor class equal to AC\n");
	printf("page AC: Prints the DDG players with the armor class greater than or equal to AC\n");
	printf("pagt AC: Prints the DDG players with the armor class greater than AC\n");
	printf("pale AC: Prints the DDG players with the armor class less than or equal to AC\n");
	printf("palt AC: Prints the DDG players with the armor class less than AC\n");
	printf("pc CLASS: Prints the DDG players with the class containing CLASS\n");
	printf("pcn CNAME: Prints the DDG players with the character name containing CNAME\n");
	printf("ph HP: Prints the DDG players with the hit points equal to HP\n");
	printf("phge HP: Prints the DDG players with the hit points greater than or equal to HP\n");
	printf("phgt HP: Prints the DDG players with the hit points greater than HP\n");
	printf("phle HP: Prints the DDG players with the hit points less than or equal to HP\n");
	printf("phlt HP: Prints the DDG players with the hit points less than HP\n");
	printf("pn NAME: Prints the DDG players with the name containing NAME\n");
	printf("v: Prints the DDGM version\n");
	printf("q: Quits DDGM\n");
}

/* Cette fonction, comme les auxiliaires qui la composent, repose sur le principe suivant :
 * - une initialisation du pointeur node sur l'emplacement de la structure XML souhaité pour le remplissage
 * - une boucle contenant des if imbriqués. A chaque tour de boucle un if différent est exécuté
 * - à la fin de chaque tour de boucle, on déplace le node sur l'emplacement suivant pour que la condition
 * du if suivant soit vraie 
 */
ddg_t *ddgm_parse_ddg(xmlDocPtr file, xmlNodePtr node) {
    xmlChar *str = NULL;
    ddg_t *ddg = ddg_create();

    if (ddg == NULL) {
        return NULL;
    }

    ddg->name = (char *) xmlGetProp(node, (const xmlChar *) "name");
    node = node->children;
    
    while (node != NULL) {
        if (xmlStrcmp(node->name, (const xmlChar *) "date") == 0) {
            ddgm_parse_date(file, node, ddg);
        }
        else if (xmlStrcmp(node->name, (const xmlChar *) "dmname") == 0) {
            str = xmlNodeListGetString(file, node->children, 1);
            ddg->dmname = (char *) str;
        }
        else if (xmlStrcmp(node->name, (const xmlChar *) "players") == 0) {
            if (ddgm_parse_players(file, node, ddg) == 1) {
                return NULL;
            }
        }

        node = node->next;
    }

    return ddg;
}

void ddgm_parse_date(xmlDocPtr file, xmlNodePtr node, ddg_t *ddg) {
    xmlChar *integer = NULL;
    node = node->children;

    while (node != NULL) {
        if (xmlStrcmp(node->name, (const xmlChar *) "day") == 0) {
            integer = xmlNodeListGetString(file, node->children, 1);
            ddg->day = (int) strtol((const char *) integer, NULL, 10);

            free(integer);
        }
        else if (xmlStrcmp(node->name, (const xmlChar *) "month") == 0) {
            integer = xmlNodeListGetString(file, node->children, 1);
            ddg->month = (int) strtol((const char *) integer, NULL, 10);

            free(integer);
        }
        else if (xmlStrcmp(node->name, (const xmlChar *) "year") == 0) {
            integer = xmlNodeListGetString(file, node->children, 1);
            ddg->year = (int) strtol((const char *) integer, NULL, 10);

            free(integer);
        }

        node = node->next;
    }
}

int ddgm_parse_players(xmlDocPtr file, xmlNodePtr nodeS, ddg_t *ddg) {
    BOOLEEN fin;
    player_t *temp;
    xmlChar *str = NULL;
    xmlChar *integer = NULL;
    xmlNodePtr nodeMoney, nodePlayer;

    xmlNodePtr node = nodeS->children;
    fin = FALSE;

    while (!(fin)) {
        temp = player_create();
        nodePlayer = node;

        while (nodePlayer != NULL) {
            if (xmlStrcmp(nodePlayer->name, (const xmlChar *) "player") == 0) {
                temp->name = (char *) xmlGetProp(nodePlayer, (const xmlChar *) "name");
                nodePlayer = nodePlayer->children;
            }
            if (xmlStrcmp(nodePlayer->name, (const xmlChar *) "ac") == 0) {
                integer = xmlNodeListGetString(file, nodePlayer->children, 1);
                temp->ac = (int) strtol((const char *) integer, NULL, 10);

                free(integer);
            }
            else if (xmlStrcmp(nodePlayer->name, (const xmlChar *) "class") == 0) {
                str = xmlNodeListGetString(file, nodePlayer->children, 1);
                temp->class = (char *) str;
            }
            else if (xmlStrcmp(nodePlayer->name, (const xmlChar *) "cname") == 0) {
                str = xmlNodeListGetString(file, nodePlayer->children, 1);
                temp->cname = (char *) str;
            }
            else if (xmlStrcmp(nodePlayer->name, (const xmlChar *) "hp") == 0) {
                integer = xmlNodeListGetString(file, nodePlayer->children, 1);
                temp->hp = (int) strtol((const char *) integer, NULL, 10);

                free(integer);
            }
            else if (xmlStrcmp(nodePlayer->name, (const xmlChar *) "money") == 0) {
                nodeMoney = nodePlayer->children;

                while (nodeMoney != NULL) {
                    if (xmlStrcmp(nodeMoney->name, (const xmlChar *) "cp") == 0) {
                        integer = xmlNodeListGetString(file, nodeMoney->children, 1);
                        temp->cp = (int) strtol((const char *) integer, NULL, 10);

                        free(integer);
                    }
                    else if (xmlStrcmp(nodeMoney->name, (const xmlChar *) "gp") == 0) {
                        integer = xmlNodeListGetString(file, nodeMoney->children, 1);
                        temp->gp = (int) strtol((const char *) integer, NULL, 10);

                        free(integer);
                    }
                    else if (xmlStrcmp(nodeMoney->name, (const xmlChar *) "sp") == 0) {
                        integer = xmlNodeListGetString(file, nodeMoney->children, 1);
                        temp->sp = (int) strtol((const char *) integer, NULL, 10);

                        free(integer);
                    }

                    nodeMoney = nodeMoney->next;
                }
            }

            nodePlayer = nodePlayer->next;
        }

        node = node->next;

        if (node->next->next != NULL) {
            node = node->next;
        }
        else {
            fin = TRUE;
        }

        if (ddg_add_player(ddg, temp) == 1) {
            fprintf(stderr, "Memory allocation error\n");
            return 1;
        }
    }

    return 0;
}

/* Cette fonction découpe la chaine passée en paramètre en deux opérateurs
 * Lorsqu'elle rencontre un espace, elle commence à remplir l'opérateur 2
 */
void command_cut(char *pStr, char *pOp1, char *pOp2) {
    int i, j, op;
    
    i = 0;
    j = 0;
    op = 1;

    while (i < strlen(pStr) && pStr[i] != '\n') {
        if (pStr[i] == ' ') {
            i++;
            pOp1[i] = '\0';
            op = 2;
        }

        if (op == 1) {
            pOp1[i] = pStr[i];
        }
        else {
            pOp2[j] = pStr[i];
            j++;
        }

        i++;
    }

    pOp2[j] = '\0';
}

int str_sum(char *pStr) {
    int i, sum;

    for (i = 0, sum = 0; i < strlen(pStr); i++) {
        sum += (int) pStr[i];
    }

    return sum;
}

BOOLEEN command_test(int pCommand, int *pList) {
    int i;
    BOOLEEN trouve;

    i = 0;
    trouve = FALSE;

    while ((i < NBE) && (trouve == FALSE) && (pCommand >= pList[i])) {
        if (pCommand == pList[i]) {
            trouve = TRUE;
        }
        else {
            i++;
        }
    }

    return trouve;
}