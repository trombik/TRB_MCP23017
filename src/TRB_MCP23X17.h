/*
 * Copyright (c) 2018 Tomoyuki Sakurai <y@trombik.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * \brief Register addresses (IOCON_BANK_MODE == 0, i.e. addresses are
 * sequential, which is the default)
 */
#if !defined(_TRB_MCP23X17_h)
#define _TRB_MCP23X17_h

#define	MCP23x17_IODIR		(0x00)
#define	MCP23x17_IPOL		(0x02)
#define	MCP23x17_GPINTEN	(0x04)
#define	MCP23x17_DEFVAL		(0x06)
#define	MCP23x17_INTCON		(0x08)
#define	MCP23x17_IOCON		(0x0A)
#define	MCP23x17_GPPU		(0x0C)
#define	MCP23x17_INTF		(0x0E)
#define	MCP23x17_INTCAP		(0x10)
#define	MCP23x17_GPIO		(0x12)
#define	MCP23x17_OLAT		(0x14)

#define	MCP23x17_IODIRA		MCP23x17_IODIR
#define	MCP23x17_IPOLA		MCP23x17_IPOL
#define	MCP23x17_GPINTENA	MCP23x17_GPINTEN
#define	MCP23x17_DEFVALA	MCP23x17_DEFVAL
#define	MCP23x17_INTCONA	MCP23x17_INTCON
#define	MCP23x17_IOCONA		MCP23x17_IOCON
#define	MCP23x17_GPPUA		MCP23x17_GPPU
#define	MCP23x17_INTFA		MCP23x17_INTF
#define	MCP23x17_INTCAPA	MCP23x17_INTCAP
#define	MCP23x17_GPIOA		MCP23x17_GPIO
#define	MCP23x17_OLATA		MCP23x17_OLAT

#define	MCP23x17_IODIRB		(MCP23x17_IODIR + 1)
#define	MCP23x17_IPOLB		(MCP23x17_IPOL + 1)
#define	MCP23x17_GPINTENB	(MCP23x17_GPINTEN + 1)
#define	MCP23x17_DEFVALB	(MCP23x17_DEFVAL + 1)
#define	MCP23x17_INTCONB	(MCP23x17_INTCON + 1)
#define	MCP23x17_IOCONB		MCP23x17_INTCON
#define	MCP23x17_GPPUB		(MCP23x17_GPPU + 1)
#define	MCP23x17_INTFB		(MCP23x17_INTF + 1)
#define	MCP23x17_INTCAPB	(MCP23x17_INTCAP + 1)
#define	MCP23x17_GPIOB		(MCP23x17_GPIO + 1)
#define	MCP23x17_OLATB		(MCP23x17_OLAT + 1)

/*
 * \brief Intrrupt conditions
 */
typedef enum
{
	/** cause an interrupt to occur if a pin changes to the opposite state */
	ON_CHANGE,
	/**  cause an interrupt to occur if the corresponding input pin differs
	 * from the register bit */
	ON_CHANGE_FROM_REG
} mcp_int_condition_t;

/*
 * \brief IOCON register bits
 */
#define	IOCON_UNUSED	(0x01)
#define	IOCON_INTPOL	(0x02)
#define	IOCON_ODR	(0x04)
#define	IOCON_HAEN	(0x08)
#define	IOCON_DISSLW	(0x10)
#define	IOCON_SEQOP	(0x20)
#define	IOCON_MIRROR	(0x40)
#define	IOCON_BANK_MODE	(0x80)

/*
 * \brief SEQOP values in IOCON
 */
/** Sequential operation enabled, address pointer increments. */
#define	IOCON_SEQOP_ENABLED	(0)
/** Sequential operation disabled, address pointer does not increments. */
#define	IOCON_SEQOP_DISABLED	(1)

#endif
