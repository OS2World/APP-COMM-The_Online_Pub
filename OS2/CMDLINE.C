/******************************************************************************
CMDLINE.C    Handle doorkit-specific command line parameters. */
Copyright 1993 - 1998 Paul Sidorsky, All Rights Reserved
Based on sample code by Joel Downer, author of Doors/2.

This manual handles some extra command line options for the OS/2 version of
TOP.
******************************************************************************/

#include "top.h"

/* ProcessCmdLine() - Process extra command line options.
   Parameters:  ac - Number of arguments (argc).
                av - String array of arguments (argv).
   Returns:  Nothing. */
void ProcessCmdLine(XINT ac, char *av[])
   {
#ifdef __OS2__
   short ctr; /* Counter. */

   /* Don't bother processing if there's only one argument. */
   if (ac > 1)
      {
      /* Process each argument. */
      for (ctr = 1; ctr < ac; ctr++)
         {
         /* Port number mode (as opposed to handle mode). */
         if ((!strcmpi(av[ctr], "-PORT")) || (!strcmpi(av[ctr], "/PORT")))
            od_control.use_port = TRUE;
         /* Nice mode (low CPU usage). */
         else if ((!strcmpi(av[ctr], "-NICE")) || (!strcmpi(av[ctr], "/NICE")))
            od_control.od_timing = TIMING_NICE;
         /* Nasty mode (hogs the CPU). */
         else if ((!strcmpi(av[ctr], "-NASTY")) || (!strcmpi(av[ctr], "/NASTY")))
            od_control.od_timing = TIMING_NASTY;
         }
      }
#endif
   }

