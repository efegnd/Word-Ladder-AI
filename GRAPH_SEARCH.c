#include <stdio.h>
#include <stdlib.h>
#include "GRAPH_SEARCH.h"
#include "data_types.h"

int main(void)
{
    Node  root, *goal;
    State *goal_state = NULL;
    enum METHODS method;
    int   Max_Level = 0, level;
    float alpha     = 0.5f;

    /* Auto-select dictionary based on WORD_LENGTH */
#if   WORD_LENGTH == 3
    Load_Dictionary("words3.txt");
#elif WORD_LENGTH == 6
    Load_Dictionary("words6.txt");
#else
    printf("ERROR: No dictionary defined for WORD_LENGTH=%d.\n", WORD_LENGTH);
    exit(-1);
#endif

    printf("\n1 --> Breadth-First Search\n");
    printf("2 --> Uniform-Cost Search\n");
    printf("3 --> Depth-First Search\n");
    printf("4 --> Depth-Limited Search\n");
    printf("5 --> Iterative Deepening Search\n");
    printf("6 --> Greedy Search\n");
    printf("7 --> A* Search\n");
    printf("8 --> Generalized A* Search\n");
    printf("Select method: ");
    scanf("%d", (int *)&method);

    if (method == DepthLimitedSearch) {
        printf("Enter maximum depth level: ");
        scanf("%d", &Max_Level);
    }
    if (method == GeneralizedAStarSearch) {
        printf("Enter alpha (0.0 = Uniform Cost, 0.5 = A*, 1.0 = Greedy): ");
        scanf("%f", &alpha);
        if (alpha < 0.0f || alpha > 1.0f) {
            printf("WARNING: alpha clamped to [0,1].\n");
            if (alpha < 0.0f) alpha = 0.0f;
            if (alpha > 1.0f) alpha = 1.0f;
        }
    }

    root.parent          = NULL;
    root.path_cost       = 0.0f;
    root.action          = (enum ACTIONS)NO_ACTION;
    root.Number_of_Child = 0;

    printf("\n======== INITIAL STATE ========\n");
    root.state = *(Create_State());

    if (PREDETERMINED_GOAL_STATE) {
        printf("======== GOAL STATE ========\n");
        goal_state = Create_State();
    }

    /* Compute heuristic for root if needed */
    if (method == GreedySearch || method == AStarSearch ||
        method == GeneralizedAStarSearch) {
        root.state.h_n = Compute_Heuristic_Function(&root.state, goal_state);
        if (PREDETERMINED_GOAL_STATE)
            goal_state->h_n = 0.0f;
    }

    printf("\n======== SEARCH STARTED ========\n");

    switch (method) {
        case BreastFirstSearch:
        case GreedySearch:
            goal = First_GoalTest_Search_TREE(method, &root, goal_state);
            break;
        case DepthFirstSearch:
        case DepthLimitedSearch:
            goal = DepthType_Search_TREE(method, &root, goal_state, Max_Level);
            break;
        case IterativeDeepeningSearch:
            goal = FAILURE;
            for (level = 0; level <= MAX_SEARCHED_NODE && goal == FAILURE; level++) {
                goal = DepthType_Search_TREE(method, &root, goal_state, level);
                if (goal != FAILURE)
                    printf("Goal found at depth level %d.\n", level);
            }
            break;
        case UniformCostSearch:
        case AStarSearch:
        case GeneralizedAStarSearch:
            goal = First_InsertFrontier_Search_TREE(method, &root, goal_state, alpha);
            break;
        default:
            printf("ERROR: Unknown method.\n");
            Free_Dictionary();
            exit(-1);
    }

    Show_Solution_Path(goal);
    Free_Dictionary();
    return 0;
}
