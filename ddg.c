#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ddg.h"

ddg_t *ddg_create() {
    ddg_t *temp;

    temp = malloc(sizeof(ddg_t));
    
    if (temp != NULL) {
        temp->nplayers = 0;
        temp->players = NULL;
    }

    return temp;
}

int ddg_add_player(ddg_t *ddg, player_t *player) {
    if (ddg->players == NULL) {
        ddg->nplayers = 1;
        ddg->players = malloc(sizeof(player_t*));
    }
    else {
        ddg->nplayers++;
        ddg->players = realloc(ddg->players, ddg->nplayers * sizeof(player_t*));
    }

    if (ddg->players == NULL) {
        return 1;
    }

    ddg->players[ddg->nplayers - 1] = player;
    
    return 0;
}

void ddg_free(ddg_t *ddg) {
    int i;

    for (i = 0; i < ddg->nplayers; i++) {
        player_free(ddg->players[i]);
    }

    free(ddg->dmname);
    free(ddg->players);
    free(ddg->name);
    free(ddg);
}

void ddg_handle_d(ddg_t ddg) {
    printf("%d-%d-%d\n", ddg.year, ddg.month, ddg.day);
}

void ddg_handle_g(ddg_t ddg) {
    printf("%s, %s, %d-%d-%d, %d player(s)\n", ddg.name, ddg.dmname, ddg.year, ddg.month, ddg.day, ddg.nplayers);
}

void ddg_handle_m(ddg_t ddg) {
    printf("%s\n", ddg.dmname);
}

void ddg_handle_n(ddg_t ddg) {
    printf("%s\n", ddg.name);
}

void ddg_handle_p(ddg_t ddg) {
    int i;
    
    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_p(*(ddg.players[i]));
    }
}

void ddg_handle_pa(ddg_t ddg, int ac) {
    int i;
    
    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_pa(*(ddg.players[i]), ac);
    }
}

void ddg_handle_page(ddg_t ddg, int ac) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_page(*(ddg.players[i]), ac);
    }
}

void ddg_handle_pagt(ddg_t ddg, int ac) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_pagt(*(ddg.players[i]), ac);
    }
}

void ddg_handle_pale(ddg_t ddg, int ac) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_pale(*(ddg.players[i]), ac);
    }
}

void ddg_handle_palt(ddg_t ddg, int ac) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_palt(*(ddg.players[i]), ac);
    }
}

void ddg_handle_pc(ddg_t ddg, const char *class) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_pc(*(ddg.players[i]), class);
    }
}

void ddg_handle_pcn(ddg_t ddg, const char *cname) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_pcn(*(ddg.players[i]), cname);
    }
}

void ddg_handle_ph(ddg_t ddg, int hp) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_ph(*(ddg.players[i]), hp);
    }
}

void ddg_handle_phge(ddg_t ddg, int hp) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_phge(*(ddg.players[i]), hp);
    }
}

void ddg_handle_phgt(ddg_t ddg, int hp) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_phgt(*(ddg.players[i]), hp);
    }
}

void ddg_handle_phle(ddg_t ddg, int hp) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_phle(*(ddg.players[i]), hp);
    }
}

void ddg_handle_phlt(ddg_t ddg, int hp) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_phlt(*(ddg.players[i]), hp);
    }
}

void ddg_handle_pn(ddg_t ddg, const char *name) {
    int i;

    for (i = 0; i < ddg.nplayers; i++) {
        player_handle_pn(*(ddg.players[i]), name);
    }
}
