/*
  +----------------------------------------------------------------------+
  | Swoole                                                               |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.0 of the Apache license,    |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.apache.org/licenses/LICENSE-2.0.html                      |
  | If you did not receive a copy of the Apache2.0 license and are unable|
  | to obtain it through the world-wide-web, please send a note to       |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Tianfeng Han  <mikan.tenny@gmail.com>                        |
  +----------------------------------------------------------------------+
*/

#include "swoole.h"

#ifdef HAVE_KQUEUE

#include <sys/uio.h>

int swoole_sendfile(int out_fd, int in_fd, off_t *offset, size_t size)
{
	off_t sent_bytes;
	int ret;

	do_sendfile:
	ret = sendfile(in_fd, out_fd, *offset, size, 0, &sent_bytes, 0);
	if (ret == -1)
	{
		if (errno == EAGAIN)
		{
			*offset += sent_bytes;
			return sent_bytes;
		}
		else if (errno == EINTR)
		{
			goto do_sendfile;
		}
		else
		{
			return -1;
		}
	}
	else if (ret == 0)
	{
		*offset += size;
		return size;
	}
	else
	{
		swWarn("sendfile failed. Error: %s[%d]", strerror(errno), errno);
		return SW_ERR;
	}
	return SW_OK;
}
#endif
