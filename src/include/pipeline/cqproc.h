/* Copyright (c) 2013-2015 PipelineDB */
/*-------------------------------------------------------------------------
 *
 * cqproc.h
 *	  prototypes for cqproc.c.
 *
 * src/include/pipeline/cqproc.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef CQPROC_H
#define CQPROC_H

#include "nodes/parsenodes.h"
#include "postmaster/bgworker.h"

typedef enum
{
	CQCombiner,
	CQWorker
} CQProcessType;

typedef struct CQProcState
{
	uint32 key; /* key must be the first field */
	int32 pg_count;
	int32 pg_size;
	bool active;
	BackgroundWorkerHandle combiner;
	/* TODO(usmanm): Make this dynamic to support parallelism */
	BackgroundWorkerHandle worker;
	bool worker_done;
} CQProcState;

extern void InitCQProcTable(void);

extern CQProcState* GetCQProcState(int32 id);
extern int GetProcessGroupSize(int32 id);
extern int GetProcessGroupSizeFromCatalog(RangeVar* rv);
extern int GetProcessGroupCount(int32 id);
extern void DecrementProcessGroupCount(int32 id);
extern void IncrementProcessGroupCount(int32 id);
extern bool *GetActiveFlagPtr(int32 id);
extern void SetActiveFlag(int32 id, bool flag);

extern CQProcState* EntryAlloc(int32 key, int pg_size);
extern void EntryRemove(int32 key);

extern bool WaitForCQProcsToStart(int32 id);
extern void WaitForCQProcsToTerminate(int32 id);
extern void TerminateCQProcs(int32 id);
extern bool DidCQWorkerCrash(int32 id);
extern void SetCQWorkerDoneFlag(int32 cq_id);

bool RunContinuousQueryProcess(CQProcessType ptype, const char *cvname, struct ContinuousViewState *state, BackgroundWorkerHandle *bg_handle);

#endif   /* CQPROC_H */