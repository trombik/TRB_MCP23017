/*
 * \brief Register addresses (IOCON_BANK_MODE == 0, i.e. addresses are
 * sequential, which is the default)
 */
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
