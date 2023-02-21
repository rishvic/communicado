#include "tftpack/send.h"

#include <arpa/inet.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <task/task.h>

#include "tftpack/constants.h"

int SendData(int otherfd, uint16_t blknum, uint8_t *buf, size_t len) {
  int written;
  uint16_t op_n, blknum_n;
  uint8_t *msg;
  size_t msglen;

  op_n = htons(TFTP_OP_DATA);
  blknum_n = htons(blknum);
  msglen = 4 + len;
  msg = (uint8_t *)malloc(msglen);
  memcpy(msg, &op_n, 2);
  memcpy(msg + 2, &blknum_n, 2);
  memcpy(msg + 4, buf, len);

  written = fdwrite(otherfd, msg, msglen);
  free(msg);
  return written;
}

int SendAck(int otherfd, uint16_t blknum) {
  int written;
  uint16_t op_n, blknum_n;
  uint8_t msg[4];
  const size_t kMsgLen = 4;

  op_n = htons(TFTP_OP_ACK);
  blknum_n = htons(blknum);
  memcpy(msg, &op_n, 2);
  memcpy(msg + 2, &blknum_n, 2);

  written = fdwrite(otherfd, msg, kMsgLen);
  return written;
}
