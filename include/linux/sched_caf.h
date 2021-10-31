/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_CAF_H
#define _LINUX_SCHED_CAF_H

#include <linux/sched.h>

enum task_boost_type {
	TASK_BOOST_NONE = 0,
	TASK_BOOST_ON_MID,
	TASK_BOOST_ON_MAX,
	TASK_BOOST_STRICT_MAX,
	TASK_BOOST_END,
};

enum task_event {
	PUT_PREV_TASK   = 0,
	PICK_NEXT_TASK  = 1,
	TASK_WAKE       = 2,
	TASK_MIGRATE    = 3,
	TASK_UPDATE     = 4,
	IRQ_UPDATE      = 5,
};

/* Note: this need to be in sync with migrate_type_names array */
enum migrate_types {
	GROUP_TO_RQ,
	RQ_TO_GROUP,
};

#if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_SCHED_WALT)
extern int sched_isolate_cpu(int cpu);
extern int sched_unisolate_cpu(int cpu);
extern int sched_unisolate_cpu_unlocked(int cpu);
#else
static inline int sched_isolate_cpu(int cpu)
{
	return 0;
}

static inline int sched_unisolate_cpu(int cpu)
{
	return 0;
}

static inline int sched_unisolate_cpu_unlocked(int cpu)
{
	return 0;
}
#endif

struct cpu_cycle_counter_cb {
	u64 (*get_cpu_cycle_counter)(int cpu);
};

DECLARE_PER_CPU_READ_MOSTLY(int, sched_load_boost);

#ifdef CONFIG_QCOM_HYP_CORE_CTL
extern int hh_vcpu_populate_affinity_info(u32 cpu_index, u64 cap_id);
extern int hh_vpm_grp_populate_info(u64 cap_id, int virq_num);
#else
static inline int hh_vcpu_populate_affinity_info(u32 cpu_index, u64 cap_id)
{
	return 0;
}
static inline int  hh_vpm_grp_populate_info(u64 cap_id, int virq_num)
{
	return 0;
}
#endif /* CONFIG_QCOM_HYP_CORE_CTL */

#ifdef CONFIG_SCHED_WALT
extern void walt_task_dead(struct task_struct *p);
extern int
register_cpu_cycle_counter_cb(struct cpu_cycle_counter_cb *cb);
extern void
sched_update_cpu_freq_min_max(const cpumask_t *cpus, u32 fmin, u32 fmax);
extern void free_task_load_ptrs(struct task_struct *p);
extern int set_task_boost(int boost, u64 period);
extern void walt_update_cluster_topology(void);

#define RAVG_HIST_SIZE_MAX  5
#define NUM_BUSY_BUCKETS 10

#define WALT_LOW_LATENCY_PROCFS	BIT(0)
#define WALT_LOW_LATENCY_BINDER	BIT(1)

struct walt_task_struct {
	/*
	 * 'mark_start' marks the beginning of an event (task waking up, task
	 * starting to execute, task being preempted) within a window
	 *
	 * 'sum' represents how runnable a task has been within current
	 * window. It incorporates both running time and wait time and is
	 * frequency scaled.
	 *
	 * 'sum_history' keeps track of history of 'sum' seen over previous
	 * RAVG_HIST_SIZE windows. Windows where task was entirely sleeping are
	 * ignored.
	 *
	 * 'demand' represents maximum sum seen over previous
	 * sysctl_sched_ravg_hist_size windows. 'demand' could drive frequency
	 * demand for tasks.
	 *
	 * 'curr_window_cpu' represents task's contribution to cpu busy time on
	 * various CPUs in the current window
	 *
	 * 'prev_window_cpu' represents task's contribution to cpu busy time on
	 * various CPUs in the previous window
	 *
	 * 'curr_window' represents the sum of all entries in curr_window_cpu
	 *
	 * 'prev_window' represents the sum of all entries in prev_window_cpu
	 *
	 * 'pred_demand' represents task's current predicted cpu busy time
	 *
	 * 'busy_buckets' groups historical busy time into different buckets
	 * used for prediction
	 *
	 * 'demand_scaled' represents task's demand scaled to 1024
	 */
	u64				mark_start;
	u32				sum, demand;
	u32				coloc_demand;
	u32				sum_history[RAVG_HIST_SIZE_MAX];
	u32				*curr_window_cpu, *prev_window_cpu;
	u32				curr_window, prev_window;
	u32				pred_demand;
	u8				busy_buckets[NUM_BUSY_BUCKETS];
	u16				demand_scaled;
	u16				pred_demand_scaled;
	u64				active_time;
	int				boost;
	bool				wake_up_idle;
	bool				misfit;
	bool				rtg_high_prio;
	u8				low_latency;
	u64				boost_period;
	u64				boost_expires;
	u64				last_sleep_ts;
	u32				init_load_pct;
	u32				unfilter;
	u64				last_wake_ts;
	u64				last_enqueued_ts;
	struct walt_related_thread_group __rcu	*grp;
	struct list_head		grp_list;
	u64				cpu_cycles;
	cpumask_t			cpus_requested;
	bool				iowaited;
};

#else
static inline void walt_task_dead(struct task_struct *p) { }

static inline int
register_cpu_cycle_counter_cb(struct cpu_cycle_counter_cb *cb)
{
	return 0;
}

static inline void free_task_load_ptrs(struct task_struct *p) { }

static inline void sched_update_cpu_freq_min_max(const cpumask_t *cpus,
					u32 fmin, u32 fmax) { }

static inline void set_task_boost(int boost, u64 period) { }
static inline void walt_update_cluster_topology(void) { }
#endif /* CONFIG_SCHED_WALT */

#ifdef CONFIG_SMP
extern bool cpupri_check_rt(void);
#else
static inline bool cpupri_check_rt(void)
{
	return false;
}
#endif

#ifdef CONFIG_SCHED_WALT
#define PF_WAKE_UP_IDLE	1
static inline u32 sched_get_wake_up_idle(struct task_struct *p)
{
	return p->wts.wake_up_idle;
}

static inline int sched_set_wake_up_idle(struct task_struct *p,
						int wake_up_idle)
{
	p->wts.wake_up_idle = !!wake_up_idle;
	return 0;
}

static inline void set_wake_up_idle(bool enabled)
{
	current->wts.wake_up_idle = enabled;
}
#else
static inline u32 sched_get_wake_up_idle(struct task_struct *p)
{
	return 0;
}

static inline int sched_set_wake_up_idle(struct task_struct *p,
						int wake_up_idle)
{
	return 0;
}

static inline void set_wake_up_idle(bool enabled) {}
#endif

#endif
