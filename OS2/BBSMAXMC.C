/******************************************************************************
BBSMAXMC.C   Implements Maximus for OS/2 v3.xx BBS functions.
Copyright 1993 - 1998 Paul Sidorsky, All Rights Reserved
Based on Scott Dudley's example code in MAX_CHAT.C and MCP.H.

The module contains all of the functions for interfacing with Maximus for OS/2
v3.xx.  Two calls are used from BBSMAX.C.

NOTE:  This module contains OS/2 specific code and should not be included in a
DOS or Win32 build.
******************************************************************************/
#include "top.h"

/* The last MCP page is stored to compensate for Maximus thinking there
   are two "copies" of each node.  See TOP/2's MAXOS2V3.DOC for details. */
char *lastmcppage;

/* max_mcpinit() - Initialize pointers to Maximus MCP interface functions.
   Paremeters:  None.
   Returns:  Nothing.
*/
void max_mcpinit(void)
    {

    bbs_call_loaduseron = max_loadmcpstatus;
    bbs_call_saveuseron = max_savemcpstatus;
    bbs_call_processmsgs = max_processmcpmsgs;
    bbs_call_page = max_mcppage;
    bbs_call_setexistbits = max_setexistbits;
    bbs_call_login = max_mcplogin;
    bbs_call_logout = max_mcplogout;
    bbs_call_openfiles = max_mcpopenpipe;
    bbs_call_updateopenfiles = max_mcpopenpipe;
    bbs_call_pageedit = max_shortpageeditor;

    lastmcppage = malloc(4096);
    lastmcppage[0] = '\0';

    }

/* max_loadmcpstatus() - Retrieves a node's status from the MCP.
   Parameters:  nodenum - Node number to load the data for.
                userdata - Pointer to generic BBS data structure to fill.
   Returns:  TRUE if an error occurred, FALSE if successful.
*/
char max_loadmcpstatus(XINT nodenum, bbsnodedata_typ *userdata)
    {
    struct _mcpcstat cs; /* Maximus MCP node status buffer. */
    USHORT si; /* Size of the message.  Not used. */

    /* Maximus doesn't support Node 0. */
    if (nodenum == 0)
        {
        return 1;
        }

    /* Retreive the status from the MCP. */
    McpSendMsg(hpMCP, PMSG_QUERY_ACTIVE, (BYTE *) &nodenum, 2);
    McpGetMsg(hpMCP, (BYTE *) &cs, &si, sizeof(struct _mcpcstat));

    /* Abort if nobody's online (blank name). */
    if (!cs.username[0])
        {
        return 1;
        }

    /* Copy the information to the generic BBS structure. */
    strcpy(userdata->handle, cs.username);
    strcpy(userdata->realname, cs.username);
    userdata->node = nodenum;
    strcpy(userdata->statdesc, cs.status);
    userdata->quiet = !cs.avail;

    return 0;
    }

/* max_savemcpstatus() - Saves a node status to the MCP.
   Parameters:  nodenum - Node number to save the data for.
                userdata - Pointer to generic BBS data structure to use.
   Returns:  TRUE if an error occurred, FALSE if successful.
*/
char max_savemcpstatus(XINT nodenum, bbsnodedata_typ *userdata)
    {
    struct _mcpcstat cs; /* Maximus MCP node status buffer. */

    /* The node number is not used because the MCP will know which node is
       sending the message. */
    (void) nodenum;

    /* Initialize the node status data. */
    memset(&cs, 0, sizeof(struct _mcpcstat));
    strcpy(cs.username,
           cfg.usehandles ? userdata->handle : userdata->realname);
    strcpy(cs.status, userdata->statdesc);
    cs.avail = 1;

    /* Set the status using the MCP. */
    McpSendMsg(hpMCP, PMSG_SET_STATUS, (BYTE *) &cs,
               sizeof(struct _mcpcstat));

    return 0;
    }

/* max_processmpcmsgs() - Reads and displays messages from the MCP.
   Parameters:  None.
   Returns:  Number of messages processed.  (0 on error.)
*/
XINT max_processmcpmsgs(void)
    {
    ULONG bytes, status; /* Size and status holders. */
    AVAILDATA ad; /* OS/2 pipe availability data. */
    char tbuf[11], *msgbuf; /* Temporary buffer, message buffer. */
    struct _mcpcdat cd; /* Maximus MCP message information buffer. */

    /* Check the pipe to see if there's something to get. */
    if (DosPeekNPipe(hpMCP, tbuf, 1, &bytes, &ad, &status))
        {
        return 0;
        }
    if (!bytes)
        {
        return 0;
        }

    /* Allocate the buffer for the incoming message. */
    msgbuf = malloc(ad.cbmessage);
    if (msgbuf == NULL)
        {
        return 0;
        }

    if (!DosRead(hpMCP, msgbuf, ad.cbmessage, &bytes))
        {
        if (* (USHORT *) msgbuf == RPMSG_GOT_MSG)
            {
            /* Retrieve the incoming message. */
            memcpy(&cd, msgbuf + sizeof(USHORT), sizeof(struct _mcpcdat));
            memmove(msgbuf,
                    msgbuf + sizeof(USHORT) + sizeof(struct _mcpcdat),
                    min(ad.cbmessage, cd.len));
            /* Don't display this message if it's the same as the last one.
               See beginning of this file for explanation. */
            if (!strcmp(lastmcppage, msgbuf))
                {
                lastmcppage[0] = '\0';
                dofree(msgbuf);
                return 0;
                }
            /* Save this message for duplicate detection. */
            strcpy(lastmcppage, msgbuf);
            if (cd.type == CMSG_HEY_DUDE)
                {
                /* Display the message to the user. */
                max_showmeccastring(msgbuf);
                dofree(msgbuf);
                return 1;
                }
            else
                {
                /* All other message types are ignored. */
                dofree(msgbuf);
                return 0;
                }
            }
        }

    dofree(msgbuf);

    return 0;
    }

/* max_mcppage() - Sends a page to a Maximus MCP node.
   Parameters:  nodenum - Node number to page.
                pagebuf - Text to send.
   Returns:  TRUE if successful, FALSE if an error occurred.
   Notes:  pagebuf should contain the text to send, and it must also be big
           enough to have the page header and footer appended.
*/
char max_mcppage(XINT nodenum, unsigned char *pagebuf)
    {
    XINT len, slen, slenf; /* Length, header length, footer length. */
    char *buf; /* Output buffer. */
    struct _mcpcdat *pcd; /* Maximus MCP node information buffer. */
    ULONG bytes; /* Size holder. */


    /* Get the header and footer lengths. */
    itoa(od_control.od_node, outnum[0], 10);
    slen = strlen(top_output(OUT_STRINGNF, getlang("MaxPageHeader"),
                             cfg.usehandles ? user.handle : user.realname,
                             outnum[0]));
    slenf = strlen(top_output(OUT_STRINGNF, getlang("MaxPageFooter")));
    len = strlen(pagebuf) + 1 + slen + slenf;
    /* Allocate the memory to store the entire message. */
    buf = malloc(len + sizeof(USHORT) + sizeof(struct _mcpcdat));
    if (buf == NULL)
        {
        od_log_write(top_output(OUT_STRING, getlang("LogOutOfMemory")));
	    return -1;
    	}

    /* Prepare the message information. */
    * (USHORT *) buf = PMSG_MAX_SEND_MSG;
    pcd = (struct _mcpcdat *) (buf + sizeof(USHORT));
    pcd->tid = od_control.od_node;
    pcd->dest_tid = nodenum;
    pcd->type = CMSG_HEY_DUDE;
    pcd->len = len;

    /* Copy the page header, then the page itself, then the footer. */
    itoa(od_control.od_node, outnum[0], 10);
    memcpy(buf + sizeof(USHORT) + sizeof(struct _mcpcdat),
           top_output(OUT_STRINGNF, getlang("MaxPageHeader"),
                      cfg.usehandles ? user.handle : user.realname,
                      outnum[0]), slen);
    memmove(buf + sizeof(USHORT) + sizeof(struct _mcpcdat) + slen, pagebuf,
            strlen(pagebuf));
    memmove(buf + sizeof(USHORT) + sizeof(struct _mcpcdat) + slen +
            strlen(pagebuf),
            top_output(OUT_STRINGNF, getlang("MaxPageFooter")), slenf);

    /* Write the page to the pipe. */
    itoa(nodenum, outnum[0], 10);
    if (DosWrite(hpMCP, buf, len + sizeof(USHORT) + sizeof(struct _mcpcdat),
                 &bytes) != 0)
        {
        top_output(OUT_SCREEN, getlang("CantPage"), outnum[0]);
        return -1;
        }

    free(buf);

    top_output(OUT_SCREEN, getlang("Paged"), outnum[0]);
    return 0;
    }

/* max_mcplogin() - Maximus MCP initialization when TOP is started.
   Parameters:  None.
   Returns:  Nothing.
*/
void max_mcplogin(void)
    {
     /* Generic BBS data buffer, login check BBS data buffer.*/
    bbsnodedata_typ bd, logintmp;
    int res;

    /* Check if the node is already logged in RA/SBBS. */
    res = max_loadmcpstatus(od_control.od_node, &logintmp);
    if (!res)
        {
        /* Set the do not disturb flag. */
        node->quiet = bd.quiet = logintmp.quiet;
        save_node_data(od_control.od_node, node);
        }

    /* Copy the user information into the generic BBS data buffer. */
    strcpy(bd.realname, user.realname);
    strcpy(bd.handle, user.handle);
    bd.node = od_control.od_node;
    strcpy(bd.statdesc,
           top_output(OUT_STRING, getlang("NodeStatus")));

    /* Send the status information to the MCP. */
    max_savemcpstatus(od_control.od_node, &bd);

    }

/* max_mcplogout() - Maximus deinitialization when TOP is exited.
   Parameters:  None.
   Returns:  Nothing.
*/
void max_mcplogout(void)
    {

    /* Tell the MCP we're leaving. */
    if (hpMCP)
        {
        McpSendMsg(hpMCP, PMSG_EOT, NULL, 0);
        DosClose(hpMCP);
        hpMCP = 0;
        }

    dofree(lastmcppage);

    }

/* max_mcpopenpipe() - Establishes connection to the Maximus MCP. */
   Parameters:  None.
   Returns:  Number of errors that occurred, or 0 on success.
*/
XINT max_mcpopenpipe(void)
    {
    ULONG rc, action; /* Result code, action buffer (unused). */
    unsigned char tid; /* Task ID (node number). */

    /* Don't open the MCP if it's already open.  This can happen in LAN
       mode when the node number is changed. */
    if (hpMCP)
        {
        return 0;
        }

    /* Setup the pipe name. */
    strcpy(outbuf, cfg.bbsmultipath);
    strcat(outbuf, "maximus");
    tid = od_control.od_node;

    /* Open the pipe. */
    rc = DosOpen(outbuf, &hpMCP, &action, 0L, FILE_NORMAL,
                 OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                 OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READWRITE |
                 OPEN_FLAGS_NOINHERIT, NULL);
    if (rc == 0)
        {
        /* Tell the MCP we're here. */
        McpSendMsg(hpMCP, PMSG_HELLO, &tid, 1);
        }
    else
        {
        hpMCP = 0;
        return 1;
        }

    return 0;
    }

