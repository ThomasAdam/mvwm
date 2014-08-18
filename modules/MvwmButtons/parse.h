/* -*-c-*- */
/*
 * MvwmButtons, copyright 1996, Jarl Totland
 *
 * This module, and the entire GoodStuff program, and the concept for
 * interfacing this module to the Window Manager, are all original work
 * by Robert Nation
 *
 * Copyright 1993, Robert Nation. No guarantees or warantees or anything
 * are provided or implied in any way whatsoever. Use this program at your
 * own risk. Permission to use this program for any purpose is given,
 * as long as the copyright is kept intact.
 *
 */

void ParseConfiguration(button_info *ub);
void parse_window_geometry(char *geom, int is_button_geometry);
char *expand_action(char *in_action, button_info *b);
