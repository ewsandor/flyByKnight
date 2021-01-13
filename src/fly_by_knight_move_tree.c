/*
 fly_by_knight_move_tree.c
 Fly by Knight - Chess Engine
 Edward Sandor
 January 2021
 
 Move Tree manipulation for Fly by Knight
*/

#include <farewell_to_king.h>

#include "fly_by_knight.h"
#include "fly_by_knight_analysis.h"
#include "fly_by_knight_error.h"
#include "fly_by_knight_move_tree.h"


/**
 * @brief Initializes node with given move.  If NULL move passed, 
 * 
 * @param node   Node to be initialized
 * @param parent Parent node, NULL if root
 * @param move   Move to init node with, NULL if root
 */
void fbk_init_move_tree_node(fbk_move_tree_node_s * node, fbk_move_tree_node_s * parent, const ftk_move_s * move)
{
  FBK_ASSERT_MSG(node != NULL, "NULL node passed");

  FBK_ASSERT_MSG(true == fbk_mutex_init(&node->lock), "Failed to init node mutex");
  FBK_ASSERT_MSG(true == fbk_mutex_lock(&node->lock), "Failed to lock node mutex");

  node->parent = parent;

  if(move && FTK_MOVE_VALID(*move))
  {
    node->move = *move;
  }
  else
  {
    ftk_invalidate_move(&node->move);
  }
  
  FBK_ASSERT_MSG(true == fbk_mutex_unlock(&node->lock), "Failed to unlock node mutex");
}

/**
 * @brief Applies move to given game
 * 
 * @param node Node to apply
 * @param game Game to modify with move tree node
 */
bool fbk_apply_move_tree_node(fbk_move_tree_node_s * node, ftk_game_s * game)
{
  ftk_result_e result;
  ftk_move_s   move;

  FBK_ASSERT_MSG(game != NULL, "Null game passed");
  FBK_ASSERT_MSG(node != NULL, "Null node passed");

  FBK_ASSERT_MSG(true == fbk_mutex_lock(&node->lock), "Failed to lock node mutex");
  move = node->move;
  FBK_ASSERT_MSG(true == fbk_mutex_unlock(&node->lock), "Failed to unlock node mutex");

  result = ftk_move_forward(game, &move);

  return (FTK_SUCCESS == result);
}

/**
 * @brief Reverts move from given game
 * 
 * @param node Node to undo
 * @param game Game to modify with move tree node
 */
bool fbk_undo_move_tree_node(fbk_move_tree_node_s * node, ftk_game_s * game)
{
  ftk_result_e result;
  ftk_move_s   move;

  FBK_ASSERT_MSG(game != NULL, "Null game passed");
  FBK_ASSERT_MSG(node != NULL, "Null node passed");

  FBK_ASSERT_MSG(true == fbk_mutex_lock(&node->lock), "Failed to lock node mutex");
  move = node->move;
  FBK_ASSERT_MSG(true == fbk_mutex_unlock(&node->lock), "Failed to unlock node mutex");

  result = ftk_move_backward(game, &move);

  return (FTK_SUCCESS == result);
}

/**
 * @brief Evaluates node represented by given game
 * 
 * @param node Node to evaluate
 * @param game Game representing this node (Assumes move is already applied)
 */
void fbk_evaluate_move_tree_node(fbk_move_tree_node_s * node, const ftk_game_s * game)
{
  unsigned int i;
  bool evaluated_now = false;
  ftk_move_list_s move_list;

  FBK_ASSERT_MSG(true == fbk_mutex_lock(&node->lock), "Failed to lock node mutex");

  if(false == node->evaluated)
  {
    /* Score position, compound == base as no child nodes evaluated */
    node->base_score = fbk_score_game(game);
    node->compound_score = node->base_score;

    /* Init child nodes */
    ftk_get_move_list(game, &move_list);
    node->child_count = move_list.count;

    if(node->child_count > 0)
    {
      node->child = malloc(node->child_count*sizeof(fbk_move_tree_node_s));

      for(i = 0; i < node->child_count; i++)
      {
        fbk_init_move_tree_node(&node->child[i], node, &move_list.move[i]);
      }
    }
    /* else - mate in X logic? */

    evaluated_now = true;
    node->evaluated = true;
  }

  FBK_ASSERT_MSG(true == fbk_mutex_unlock(&node->lock), "Failed to unlock node mutex");

  if(evaluated_now)
  {
    /* Cleanup if evaluated in this call, else evaluated previously */
    ftk_delete_move_list(&move_list);
  }
}