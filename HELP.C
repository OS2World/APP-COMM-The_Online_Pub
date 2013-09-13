/******************************************************************************
HELP.C       HELP command processor.

    Copyright 1993 - 2000 Paul J. Sidorsky

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Processes the HELP command to display help files.
******************************************************************************/
#include "top.h"

/* help_proc() - Process the HELP command.
   Parameters:  helpwds - Number of words in the command string.
   Returns:  Nothing.
*/
void help_proc(XINT helpwds)
    { // Will need to change the order to the order proc'd later.
    char hused = 0; /* Flag if a help file has been shown or not. */
    unsigned char *helpfile = NULL; /* Name of help file to show. */

    /* In this function, basically every command is checked to see if it
       follows the HELP command.  If a match is found, the name of the
       file to show is set, as is a flag. */

    /* Due to the simplicity of this function, comments have been kept
       to a minimum. */

    /* Regular commands. */
    if (!hused && (helpwds < 2 ||
                   checkcmdmatch(get_word(1), getlang("CmdsHelp"))))
        {
        hused = 1;
        helpfile = "HELP_HLP";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsQuit")))
        {
        hused = 1;
        helpfile = "HELPQUIT";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsCommands")))
        {
        hused = 1;
        helpfile = "HELPCMDS";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsColourHelp")))
        {
        hused = 1;
        helpfile = "HELPCHLP";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsTime")))
        {
        hused = 1;
        helpfile = "HELPTIME";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsWho")))
        {
        hused = 1;
        helpfile = "HELP_WHO";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsProfile")))
        {
        hused = 1;
        helpfile = "HELPPROF";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsLookUp")))
        {
        hused = 1;
        helpfile = "HELPLKUP";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsNodes")))
        {
        hused = 1;
        helpfile = "HELPNODS";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsPage")))
        {
        hused = 1;
        helpfile = "HELPPAGE";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsForget")))
        {
        hused = 1;
        helpfile = "HELP_FGT";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsRemember")))
        {
        hused = 1;
        helpfile = "HELPRMBR";
        } // May need slight mods below
    /* Whispers have two command forms; each is checked. */
    if (!hused && (checkcmdmatch(get_word(1), getlang("CmdsWhisperLong1")) ||
                   checkcmdmatch(get_word(1), getlang("WhisperShortChar"))))
        {
        hused = 1;
        helpfile = "HELPWHIS";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsGA")))
        {
        hused = 1;
        helpfile = "HELP_GA1";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsGA2")))
        {
        hused = 1;
        helpfile = "HELP_GA2";
        } // May need slight mods below
    /* Directed messages have two command forms; each is checked. */
    if (!hused && (checkcmdmatch(get_word(1), getlang("CmdsDirectedLong1")) ||
                   checkcmdmatch(get_word(1), getlang("DirectedShortChar"))))
        {
        hused = 1;
        helpfile = "HELP_DIR";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsJoin")))
        {
        hused = 1;
        helpfile = "HELPJOIN";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsPrivChat")))
        {
        hused = 1;
        helpfile = "HELPCHAT";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsPrivDenyChat")))
        {
        hused = 1;
        helpfile = "HELPDENY";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsLogOff")))
        {
        hused = 1;
        helpfile = "HELPLGOF";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsConfList")))
        {
        hused = 1;
        helpfile = "HELPCLST";
        }

    /* Multi-part commands.  If a valid secondary command is recognized
       then its specific help file will be shown, otherwise a general help
       file for the multi-part command will be shown. */
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsAction")))
        {
        hused = 1;
        helpfile = "HELPACT0";
        if (helpwds >= 2)
            {
            if (checkcmdmatch(get_word(2), getlang("CmdsActionList")))
                helpfile = "HELPACT1";
            if (checkcmdmatch(get_word(2), getlang("CmdsActionOn")))
                helpfile = "HELPACT2";
            if (checkcmdmatch(get_word(2), getlang("CmdsActionOff")))
                helpfile = "HELPACT3";
            }
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsChange")))
        {
        hused = 1;
        helpfile = "HELPCHG0";
        if (helpwds >= 2)
            {
            if (checkcmdmatch(get_word(2), getlang("CmdsChangeHandle")))
                helpfile = "HELPCHG1";
            if (checkcmdmatch(get_word(2), getlang("CmdsChangeEMessage")))
                helpfile = "HELPCHG2";
            if (checkcmdmatch(get_word(2), getlang("CmdsChangeXMessage")))
                helpfile = "HELPCHG3";
            if (checkcmdmatch(get_word(2), getlang("CmdsChangeSex")))
                helpfile = "HELPCHG4";
            if (checkcmdmatch(get_word(2), getlang("CmdsChangeChatMode")))
                helpfile = "HELPCHG5";
            if (checkcmdmatch(get_word(2), getlang("CmdsChangeListed")))
                helpfile = "HELPCHG6";
            }
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsSysop")))
        {
        hused = 1;
        helpfile = "HELPSYS0";
        if (helpwds >= 2)
            {
            if (checkcmdmatch(get_word(2), getlang("CmdsSysopClear")))
                helpfile = "HELPSYS1";
            if (checkcmdmatch(get_word(2), getlang("CmdsSysopToss")))
                helpfile = "HELPSYS2";
            if (checkcmdmatch(get_word(2), getlang("CmdsSysopZap")))
                helpfile = "HELPSYS3";
            if (checkcmdmatch(get_word(2), getlang("CmdsSysopSetSec")))
                helpfile = "HELPSYS4";
            }
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsModerator")))
        {
        hused = 1;
        helpfile = "HELPMOD0";
        if (helpwds >= 2)
            {
            if (checkcmdmatch(get_word(2), getlang("CmdsModTopicChg")))
                helpfile = "HELPMOD1";
            if (checkcmdmatch(get_word(2), getlang("CmdsModModChg")))
                helpfile = "HELPMOD2";
            if (checkcmdmatch(get_word(2), getlang("CmdsModBan")))
                helpfile = "HELPMOD3";
            if (checkcmdmatch(get_word(2), getlang("CmdsModUnBan")))
                helpfile = "HELPMOD4";
            if (checkcmdmatch(get_word(2), getlang("CmdsModInvite")))
                helpfile = "HELPMOD5";
            if (checkcmdmatch(get_word(2), getlang("CmdsModUnInvite")))
                helpfile = "HELPMOD6";
            }
        }

    /* Non-command help topics. */
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsHelpTopics")))
        {
        hused = 1;
        helpfile = "HELP_TOP";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsHelpGeneral")))
        {
        hused = 1;
        helpfile = "HELP_GEN";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsHelpActions")))
        {
        hused = 1;
        helpfile = "HELPACTS";
        }
    if (!hused && checkcmdmatch(get_word(1), getlang("CmdsHelpChannels")))
        {
        hused = 1;
        helpfile = "HELPCHAN";
        }

    if (hused)
        {
        /* Show the help file if one was set. */
        show_helpfile(helpfile);
        }
    else
        {
        /* Unknown command. */
        top_output(OUT_SCREEN, getlang("NoHelpAvailable"));
        }

    }

/* show_helpfile() - Displays a help file to the screen.
   Parameters:  hlpname - Base name of the help file to show.
   Returns:  Nothing.
   Notes:  This function is really just a wrapper for show_file() that
           adds in the help prefix and suffix as well.
*/
void show_helpfile(unsigned char *hlpname)
    {

    top_output(OUT_SCREEN, getlang("HelpPrefix"));
    show_file(hlpname, SCRN_RESETCOL);
    top_output(OUT_SCREEN, getlang("HelpSuffix"));

    }
