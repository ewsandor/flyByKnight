/*
 fly_by_knight_analysis.h
 Fly by Knight - Chess Engine
 Edward Sandor
 January 2021
 
 Gama analysis for Fly by Knight
*/

#ifndef _FLY_BY_KNIGHT_ANALYSIS_H_
#define _FLY_BY_KNIGHT_ANALYSIS_H_

#include "fly_by_knight_types.h"

/**
 * @brief Score game position for white or black advantage
 * 
 * @param game      to analyze
 * @return fbk_score_t 
 */
fbk_score_t fbk_score_game(const ftk_game_s * game);

#endif //_FLY_BY_KNIGHT_ANALYSIS_H_