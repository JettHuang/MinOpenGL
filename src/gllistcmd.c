/* \brief
 *		build the gl-operation table, and execute the handler.
 *
 * \author	jett huang
 * \date	2010-9-8
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "glcontext.h"
#include "glvertexmachine.h"
#include "gllistcmd.h"


/* the type of callback function */
typedef void (*PtrCmdHandler)(GLContext *, OpParam *);

/* \brief
 *		command mapping to ptr function.
 */
static PtrCmdHandler s_cmdTable[] = {
	glopEnable
};

/* \brief
 *		execute the command, this function is the interface of operation library.
 * \param 
 *		cmd		(in)	pointer to op-command
 * \ret
 *		return 0 if succeed, otherwise return none-zero.
 */
int execute_command(PtrOpCommand cmd)
{
	if (!cmd) return 1;
	if (cmd->_opcode >= sizeof(s_cmdTable)/sizeof(s_cmdTable[0]))
	{
		printf("ERROR IN GL_CORE: invalid opcode\n");
		return 1;
	}

	if (!s_cmdTable[cmd->_opcode]) {
		printf("WARNING IN GL_CORE: no handler in cmdTable\n");
		return 1;
	}

	/* TODO: 
		(*s_cmdTable[cmd->_opcode])( );
	*/
	return 0;
}