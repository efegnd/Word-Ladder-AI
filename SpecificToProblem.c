#include "GRAPH_SEARCH.h"
#include "data_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ===== DICTIONARY ===== */

static char **dict       = NULL;
static int    dict_size  = 0;

void Load_Dictionary(const char *filename)
{
    FILE *fp;
    char  buf[32];
    int   capacity = 8000, i;

    dict = (char **)malloc((size_t)capacity * sizeof(char *));
    if (!dict) { printf("ERROR: malloc failed in Load_Dictionary.\n"); exit(-1); }

    fp = fopen(filename, "r");
    if (!fp) { printf("ERROR: Cannot open dictionary file '%s'.\n", filename); exit(-1); }

    while (fscanf(fp, "%31s", buf) == 1) {
        if ((int)strlen(buf) != WORD_LENGTH) continue;
        for (i = 0; buf[i]; i++) buf[i] = (char)toupper((unsigned char)buf[i]);

        if (dict_size >= capacity) {
            capacity *= 2;
            dict = (char **)realloc(dict, (size_t)capacity * sizeof(char *));
            if (!dict) { printf("ERROR: realloc failed.\n"); exit(-1); }
        }
        dict[dict_size] = (char *)malloc((size_t)(WORD_LENGTH + 1));
        if (!dict[dict_size]) { printf("ERROR: malloc failed.\n"); exit(-1); }
        strcpy(dict[dict_size], buf);
        dict_size++;
    }
    fclose(fp);
    printf("Dictionary loaded: %d words of length %d.\n", dict_size, WORD_LENGTH);
    if (dict_size == 0) {
        printf("WARNING: Dictionary is empty. Check '%s' and WORD_LENGTH.\n", filename);
    }
}

void Free_Dictionary(void)
{
    int i;
    for (i = 0; i < dict_size; i++) free(dict[i]);
    free(dict);
    dict      = NULL;
    dict_size = 0;
}

int Is_In_Dictionary(const char *word)
{
    int i;
    for (i = 0; i < dict_size; i++)
        if (strcmp(dict[i], word) == 0) return TRUE;
    return FALSE;
}

/* ===== COMPULSORY PROBLEM-SPECIFIC FUNCTIONS ===== */

State* Create_State(void)
{
    State *state = (State *)malloc(sizeof(State));
    if (!state) Warning_Memory_Allocation();

    do {
        int i;
        printf("Enter a %d-letter English word: ", WORD_LENGTH);
        if (scanf("%6s", state->word) != 1) {
            printf("ERROR: Input failed.\n");
            exit(-1);
        }
        for (i = 0; state->word[i]; i++)
            state->word[i] = (char)toupper((unsigned char)state->word[i]);
        state->word[WORD_LENGTH] = '\0';

        if ((int)strlen(state->word) != WORD_LENGTH)
            printf("  --> Must be exactly %d letters. Try again.\n", WORD_LENGTH);
        else if (!Is_In_Dictionary(state->word))
            printf("  --> '%s' is not in the dictionary. Try again.\n", state->word);

    } while ((int)strlen(state->word) != WORD_LENGTH || !Is_In_Dictionary(state->word));

    state->h_n = 0.0f;
    return state;
}

void Print_State(const State *const state)
{
    printf("%s", state->word);
}

/* action = position * 26 + letter_index */
void Print_Action(const enum ACTIONS action)
{
    int  pos    = (int)action / 26;
    char letter = (char)('A' + (int)action % 26);
    printf("pos%d -> '%c'", pos, letter);
}

int Result(const State *const parent_state, const enum ACTIONS action,
           Transition_Model *const trans_model)
{
    int  pos    = (int)action / 26;
    char letter = (char)('A' + (int)action % 26);
    char new_word[7];

    /* Reject if position is outside current word length */
    if (pos >= WORD_LENGTH) return FALSE;

    /* Reject if the letter is identical (no real change) */
    if (parent_state->word[pos] == letter) return FALSE;

    strcpy(new_word, parent_state->word);
    new_word[pos] = letter;

    /* Reject if the resulting word is not in the dictionary */
    if (!Is_In_Dictionary(new_word)) return FALSE;

    strcpy(trans_model->new_state.word, new_word);
    trans_model->new_state.h_n = 0.0f;
    trans_model->step_cost     = 1.0f;
    return TRUE;
}

/* Hamming distance: number of positions where letters differ */
float Compute_Heuristic_Function(const State *const state, const State *const goal)
{
    int i, diff = 0;
    for (i = 0; i < WORD_LENGTH; i++)
        if (state->word[i] != goal->word[i]) diff++;
    return (float)diff;
}

int Goal_Test(const State *const state, const State *const goal_state)
{
    if (PREDETERMINED_GOAL_STATE)
        return Compare_States(state, goal_state);
    return TRUE;
}
