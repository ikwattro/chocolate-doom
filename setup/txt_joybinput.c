// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// Copyright(C) 2006 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doomkeys.h"
#include "joystick.h"

#include "txt_joybinput.h"
#include "txt_gui.h"
#include "txt_io.h"
#include "txt_label.h"
#include "txt_window.h"

#define JOYSTICK_INPUT_WIDTH 10

static void OpenPromptWindow(txt_joystick_input_t *joystick_input)
{
    txt_window_t *window;
    txt_label_t *label;

    window = TXT_NewWindow(NULL);
    TXT_SetWindowAction(window, TXT_HORIZ_LEFT, NULL);
    TXT_SetWindowAction(window, TXT_HORIZ_CENTER, 
                        TXT_NewWindowAbortAction(window));
    TXT_SetWindowAction(window, TXT_HORIZ_RIGHT, NULL);
    
    label = TXT_NewLabel("Press the new joystick button...");

    TXT_AddWidget(window, label);
    TXT_SetWidgetAlign(label, TXT_HORIZ_CENTER);
}

static void TXT_JoystickInputSizeCalc(TXT_UNCAST_ARG(joystick_input))
{
    TXT_CAST_ARG(txt_joystick_input_t, joystick_input);

    // All joystickinputs are the same size.

    joystick_input->widget.w = JOYSTICK_INPUT_WIDTH;
    joystick_input->widget.h = 1;
}

static void GetJoystickButtonDescription(int button, char *buf)
{
    sprintf(buf, "BUTTON #%i", button + 1);
}

static void TXT_JoystickInputDrawer(TXT_UNCAST_ARG(joystick_input), int selected)
{
    TXT_CAST_ARG(txt_joystick_input_t, joystick_input);
    char buf[20];
    int i;

    if (*joystick_input->variable == -1)
    {
        strcpy(buf, "");
    }
    else
    {
        GetJoystickButtonDescription(*joystick_input->variable, buf);
    }

    if (selected)
    {
        TXT_BGColor(TXT_COLOR_GREY, 0);
    }
    else
    {
        TXT_BGColor(TXT_COLOR_BLUE, 0);
    }

    TXT_FGColor(TXT_COLOR_BRIGHT_WHITE);
    
    TXT_DrawString(buf);
    
    for (i=strlen(buf); i<JOYSTICK_INPUT_WIDTH; ++i)
    {
        TXT_DrawString(" ");
    }
}

static void TXT_JoystickInputDestructor(TXT_UNCAST_ARG(joystick_input))
{
}

static int TXT_JoystickInputKeyPress(TXT_UNCAST_ARG(joystick_input), int joystick)
{
    TXT_CAST_ARG(txt_joystick_input_t, joystick_input);

    if (joystick == KEY_ENTER)
    {
        // Open a window to prompt for the new joystick press

        OpenPromptWindow(joystick_input);

        return 1;
    }

    return 0;
}

static void TXT_JoystickInputMousePress(TXT_UNCAST_ARG(widget), int x, int y, int b)
{
    TXT_CAST_ARG(txt_joystick_input_t, widget);
            
    // Clicking is like pressing enter

    if (b == TXT_MOUSE_LEFT)
    {
        TXT_JoystickInputKeyPress(widget, KEY_ENTER);
    }
}

txt_widget_class_t txt_joystick_input_class =
{
    TXT_JoystickInputSizeCalc,
    TXT_JoystickInputDrawer,
    TXT_JoystickInputKeyPress,
    TXT_JoystickInputDestructor,
    TXT_JoystickInputMousePress,
    NULL,
};

txt_joystick_input_t *TXT_NewJoystickInput(int *variable)
{
    txt_joystick_input_t *joystick_input;

    joystick_input = malloc(sizeof(txt_joystick_input_t));

    TXT_InitWidget(joystick_input, &txt_joystick_input_class);
    joystick_input->variable = variable;

    return joystick_input;
}
