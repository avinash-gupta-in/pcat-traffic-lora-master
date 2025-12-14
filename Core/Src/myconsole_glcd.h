/*
 * myconsole_glcd.h
 *
 *  Created on: Dec 10, 2025
 *      Author: gmail
 */

#ifndef SRC_MYCONSOLE_GLCD_H_
#define SRC_MYCONSOLE_GLCD_H_

#define CONSOLE_WIDTH 	42
#define CONSOLE_HEIGHT 	16

void InitConsole();
void ClearConsole();
void DrawConsole();
void ConsoleAppendText(const char *str);
void ConsolePutChar(char ch);

#endif /* SRC_MYCONSOLE_GLCD_H_ */
