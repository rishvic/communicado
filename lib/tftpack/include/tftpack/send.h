#ifndef COMMUNICADO_TFTPACK_SEND_H_
#define COMMUNICADO_TFTPACK_SEND_H_

#include <stddef.h>
#include <stdint.h>

int SendData(int otherfd, uint16_t blknum, uint8_t *buf, size_t len);

int SendAck(int otherfd, uint16_t blknum);

#endif /* COMMUNICADO_TFTPACK_SEND_H_ */
