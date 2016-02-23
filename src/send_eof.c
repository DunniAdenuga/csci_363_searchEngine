/* send_eof.c - send_eof */
#ifndef __SEND_EOF
#define __SEND_EOF

#include <sys/socket.h>

/*------------------------------------------------------------------------
 * send_eof - signal EOF by shutting down send side of connection
 *------------------------------------------------------------------------
 */
int
send_eof(int conn)
{
	return shutdown(conn, 1);
}

#endif
