/* $Id: layer3.h,v 0.9 2001/03/03 18:17:15 kkeil Exp $
 *
 * This file is (c) under GNU PUBLIC LICENSE
 *
 */

#include <linux/hisaxif.h>
#include <linux/skbuff.h>
#include "fsm.h"
#ifdef MEMDBG
#include "memdbg.h"
#endif

#define SBIT(state) (1<<state)
#define ALL_STATES  0x03ffffff

#define PROTO_DIS_EURO	0x08

#define L3_DEB_WARN	0x01
#define L3_DEB_PROTERR	0x02
#define L3_DEB_STATE	0x04
#define L3_DEB_CHARGE	0x08
#define L3_DEB_CHECK	0x10
#define L3_DEB_SI	0x20

#define FLG_L2BLOCK	1
#define FLG_PTP		2

typedef struct _L3Timer {
	struct _l3_process	*pc;
	struct timer_list	tl;
	int			event;
} L3Timer_t;

typedef struct _l3_process {
	struct _l3_process	*prev;
	struct _l3_process	*next;
	struct _layer3		*l3;
	int			callref;
	int			state;
	L3Timer_t		timer;
	int			n303;
	u_int			id;
	int			bc;
	u_char			obuf[MAX_DFRAME_LEN];
	u_char			*op;
	int			err;
	union {
		ALERTING_t		ALERTING;
		CALL_PROCEEDING_t	CALL_PROCEEDING;
		CONGESTION_CONTROL_t	CONGESTION_CONTROL;
		CONNECT_t		CONNECT;
		CONNECT_ACKNOWLEDGE_t	CONNECT_ACKNOWLEDGE;
		DISCONNECT_t		DISCONNECT;
		FACILITY_t		FACILITY;
		INFORMATION_t		INFORMATION;
		NOTIFY_t		NOTIFY;
		PROGRESS_t		PROGRESS;
		RELEASE_t		RELEASE;
		RELEASE_COMPLETE_t	RELEASE_COMPLETE;
		RESTART_t		RESTART;
		RESUME_ACKNOWLEDGE_t	RESUME_ACKNOWLEDGE;
		RESUME_REJECT_t		RESUME_REJECT;
		SETUP_t			SETUP;
		SETUP_ACKNOWLEDGE_t	SETUP_ACKNOWLEDGE;
		STATUS_t		STATUS;
		STATUS_ENQUIRY_t	STATUS_ENQUIRY;
		SUSPEND_ACKNOWLEDGE_t	SUSPEND_ACKNOWLEDGE;
		SUSPEND_REJECT_t	SUSPEND_REJECT;
		USER_INFORMATION_t	USER_INFORMATION;
	} para;
} l3_process_t;

typedef struct _layer3 {
	struct _layer3	*prev;
	struct _layer3	*next;
	struct FsmInst	l3m;
	struct FsmTimer	l3m_timer;
	l3_process_t	*proc;
	l3_process_t	*global;
	l3_process_t	*dummy;
	int		(*p_mgr)(l3_process_t *, u_int, void *);
	u_int		id;
	int		debug;
	u_int		Flag;
	hisaxinstance_t	inst;
	struct sk_buff_head squeue;
} layer3_t;

struct stateentry {
	int state;
	int primitive;
	void (*rout) (l3_process_t *, u_char, void *);
};

extern int l3_msg(layer3_t *, u_int, int, int, void *);
extern struct sk_buff *l3_alloc_skb(int);
extern void newl3state(l3_process_t *, int);
extern void L3InitTimer(l3_process_t *, L3Timer_t *);
extern void L3DelTimer(L3Timer_t *);
extern int L3AddTimer(L3Timer_t *, int, int);
extern void StopAllL3Timer(l3_process_t *);
extern void release_l3_process(l3_process_t *);
extern l3_process_t *getl3proc(layer3_t *, int);
extern l3_process_t *getl3proc4id(layer3_t *, int);
extern l3_process_t *new_l3_process(layer3_t *, int, int);
extern u_char *findie(u_char *, int, u_char, int);
extern int hisax_l3up(l3_process_t *, u_int, int, void *);
extern int getcallref(u_char * p);
extern int newcallref(void);
extern void init_l3(layer3_t *);
extern void release_l3(layer3_t *);
extern void HiSaxl3New(void);
extern void HiSaxl3Free(void);
extern void l3_debug(layer3_t *, char *, ...);