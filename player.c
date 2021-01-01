#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

player_t *player_create() {
    player_t *temp;

    temp = malloc(sizeof(player_t));
    temp->ac = 0;
    temp->class = "cDefault";
    temp->cname = "cnDefault";
    temp->cp = 0;
    temp->gp = 0;
    temp->hp = 0;
    temp->name = "nDefault";
    temp->sp = 0;

    return temp;
}

void player_free(player_t *player) {
    free(player->class);
    free(player->cname);
    free(player->name);
    free(player);
}

void player_handle_p(player_t player) {
    float gpTot;

    gpTot = player.gp + player.sp / 10.0 + player.sp / 100.0;
    printf("%s (%s), %s, AC: %d, HP: %d, ", player.name, player.cname, player.class, player.ac, player.hp);
    printf("GP: %.2f (GP: %d, SP: %d, CP: %d)\n", gpTot, player.gp, player.sp, player.cp);
}

void player_handle_pa(player_t player, int ac) {
    if (player.ac == ac) {
        player_handle_p(player);
    }
}

void player_handle_page(player_t player, int ac) {
    if (player.ac >= ac) {
        player_handle_p(player);
    }
}

void player_handle_pagt(player_t player, int ac) {
    if (player.ac > ac) {
        player_handle_p(player);
    }
}

void player_handle_pale(player_t player, int ac) {
    if (player.ac <= ac) {
        player_handle_p(player);
    }
}

void player_handle_palt(player_t player, int ac) {
    if (player.ac < ac) {
        player_handle_p(player);
    }
}

void player_handle_pc(player_t player, const char *class) {
    if (strstr(player.class, class) != NULL) {
        player_handle_p(player);
    }
}

void player_handle_pcn(player_t player, const char *cname) {
    if (strstr(player.cname, cname) != NULL) {
        player_handle_p(player);
    }
}

void player_handle_ph(player_t player, int hp) {
    if (player.hp == hp) {
        player_handle_p(player);
    }
}

void player_handle_phge(player_t player, int hp) {
    if (player.hp >= hp) {
        player_handle_p(player);
    }
}

void player_handle_phgt(player_t player, int hp) {
    if (player.hp > hp) {
        player_handle_p(player);
    }
}

void player_handle_phle(player_t player, int hp) {
    if (player.hp <= hp) {
        player_handle_p(player);
    }
}

void player_handle_phlt(player_t player, int hp) {
    if (player.hp < hp) {
        player_handle_p(player);
    }
}

void player_handle_pn(player_t player, const char *name) {
    if (strstr(player.name, name) != NULL) {
        player_handle_p(player);
    }
}