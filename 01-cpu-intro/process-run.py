#! /usr/bin/env python

from __future__ import print_function

import random
from optparse import OptionParser


# seed the random library with some arbitrary value
# defaulting to 42
def random_seed(seed=42):
    random.seed(seed)


# states, and meta

# process switch behavior
SCHED_SWITCH_ON_IO = "SWITCH_ON_IO"
SCHED_SWITCH_ON_END = "SWITCH_ON_END"

# io finished behavior
IO_RUN_IMMEDIATE = "IO_RUN_IMMEDIATE"
IO_RUN_LATER = "IO_RUN_LATER"

# process states
STATE_READY = "READY"
STATE_RUNNING = "RUNNING"
STATE_WAIT = "BLOCKED"
STATE_DONE = "DONE"

# members of process structure
# used to display info
PROC_CODE = "code_"
PROC_PC = "pc_"
PROC_ID = "pid_"
PROC_STATE = "proc_state_"

# things a process can do
DO_COMPUTE = "cpu"
DO_IO = "io"

# io state
DO_IO_DONE = "io_done"

# min, max values for change, and random-number generation
MIN_LIMIT = 0
MAX_LIMIT = 100


class scheduler:
    def __init__(self, process_switch_behavior, io_done_behavior, io_length):
        # keep set of instructions for each of the processes
        self.proc_info = {}
        self.process_switch_behavior = process_switch_behavior
        self.io_done_behavior = io_done_behavior
        self.io_length = io_length
        return

    def new_process(self):
        proc_id = len(self.proc_info) # there is hard dependency on this logic
        self.proc_info[proc_id] = {}
        self.proc_info[proc_id][PROC_PC] = 0
        self.proc_info[proc_id][PROC_ID] = proc_id
        self.proc_info[proc_id][PROC_CODE] = []
        self.proc_info[proc_id][PROC_STATE] = STATE_READY  # process is in READY state
        return proc_id

    # program looks like this:
    #   c7,i,c1,i
    # which means
    #   compute for 7, then i/o, then compute for 1, then i/o
    def load_program(self, program):
        # create a new process, with associated DS
        proc_id = self.new_process()

        # iterate through the list of processes
        for line in program.split(","):
            # find whether compute or io
            # add to the list of PROC_CODE
            # eg: list = [DO_COMPUTE, DO_COMPUTE, DO_IO, DO_IO_DONE, DO_COMPUTE]
            opcode = line[0]
            if opcode == "c":  # compute
                num = int(line[1:])
                for i in range(num):
                    self.proc_info[proc_id][PROC_CODE].append(DO_COMPUTE)

            elif opcode == "i":
                # IO and IO_DONE come in paris
                self.proc_info[proc_id][PROC_CODE].append(DO_IO)
                # add one compute to HANDLE the I/O completion
                self.proc_info[proc_id][PROC_CODE].append(DO_IO_DONE)

            else:
                print("bad opcode %s (should be c or i)" % opcode)
                exit(1)

    # seems like a different version of load_program
    def load(self, program_description):
        # create a new process, with associated DS
        proc_id = self.new_process()

        # parse the inputs
        # and checked for correctness
        tmp = program_description.split(":")
        if len(tmp) != 2:
            print("Bad description (%s): Must be number <x:y>" % program_description)
            print("  where X is the number of instructions")
            print("  and Y is the percent change that an instruction is CPU not IO")
            exit(1)

        # based on the probability of cpu/io, the compute/io list of PROC_CODE
        # eg: 2:50 -> [DO_COMPUTE, DO_IO]
        # eg: 3:33 -> [DO_IO, DO_COMPUTE, DO_IO]
        # TODO: Find where the x, y chars are split?
        # oh! x, y are not even part of inputs
        num_instructions, chance_cpu = int(tmp[0]), float(tmp[1]) / 100.0
        for i in range(num_instructions):
            if random.random(0, 100) < chance_cpu:
                self.proc_info[proc_id][PROC_CODE].append(DO_COMPUTE)
            else:
                # IO and IO_DONE come in pairs
                self.proc_info[proc_id][PROC_CODE].append(DO_IO)
                # add one compute to HANDLE the I/O completion
                self.proc_info[proc_id][PROC_CODE].append(DO_IO_DONE)

    # move process state to STATE_READY, if the state if "expected"
    # theoretically, expected = STARTED, BLOCKED
    # no other states can be moved to ready
    def move_to_ready(self, expected, pid=-1):
        # if no pid is provided, use the current process
        # but, where is this curr_proc defined?
        if pid == -1:
            pid = self.curr_proc
        # enforce that the PROC_STATE is the same as what's expected
        assert self.proc_info[pid][PROC_STATE] == expected
        self.proc_info[pid][PROC_STATE] = STATE_READY
        return

    # move process state to WAIT
    # theoretically, the process is moved to when IO is issued
    def move_to_wait(self, expected):
        assert self.proc_info[self.curr_proc][PROC_STATE] == expected
        self.proc_info[self.curr_proc][PROC_STATE] = STATE_WAIT
        return

    # move process to RUNNING
    # theoretically, the process is moved to running, when IO is complete
    def move_to_running(self, expected):
        assert self.proc_info[self.curr_proc][PROC_STATE] == expected
        self.proc_info[self.curr_proc][PROC_STATE] = STATE_RUNNING
        return

    # move process to DONE
    # theoretically, the process is moved to done, when all it's instructions
    # are executed, the IO is also done
    def move_to_done(self, expected):
        assert self.proc_info[self.curr_proc][PROC_STATE] == expected
        self.proc_info[self.curr_proc][PROC_STATE] = STATE_DONE
        return

    # this is the process switcher
    def next_proc(self, pid=-1):
        # if pid is provided, set the curr_proc to pid
        # and, move the process to running, if it can be moved that is
        if pid != -1:
            self.curr_proc = pid # Here, curr_proc is SET to pid
            self.move_to_running(STATE_READY)
            return

        # if pid is not provided, that implies that there is some curr_proc
        # in that case, iterate from the curr_proc + 1 to the max(pid)
        # find the first process with STATE_READY, and move to running
        for pid in range(self.curr_proc + 1, len(self.proc_info)):
            if self.proc_info[pid][PROC_STATE] == STATE_READY:
                self.curr_proc = pid
                self.move_to_running(STATE_READY)
                return

        # if the above does not work, circle back to the beginning of the pid list
        # iterate from beginning to curr_proc
        # find the first process with STATE_READY, and move to running
        for pid in range(0, self.curr_proc + 1):
            if self.proc_info[pid][PROC_STATE] == STATE_READY:
                self.curr_proc = pid
                self.move_to_running(STATE_READY)
                return
        return

    # since self.proc_info keeps a dict of all processes
    # we can easily get the number of processes from its length
    def get_num_processes(self):
        return len(self.proc_info)

    # from a particular process, we can get the length
    # from the list of process instructions, i.e., PROC_CODE
    def get_num_instructions(self, pid):
        return len(self.proc_info[pid][PROC_CODE])

    # get the instruction pointed at by the index from
    # the list of PROC_CODEs
    def get_instruction(self, pid, index):
        return self.proc_info[pid][PROC_CODE][index]

    # iterate through the dict of proc_info, and any process
    # not in the STATE_DONE is active
    def get_num_active(self):
        num_active = 0
        for pid in range(len(self.proc_info)):
            if self.proc_info[pid][PROC_STATE] != STATE_DONE:
                num_active += 1
        return num_active

    # iterate through the dict of proc_info, and any process
    # in STATE_READY, or STATE_RUNNING is 'runnable'
    def get_num_runnable(self):
        num_active = 0
        for pid in range(len(self.proc_info)):
            if (
                self.proc_info[pid][PROC_STATE] == STATE_READY
                or self.proc_info[pid][PROC_STATE] == STATE_RUNNING
            ):
                num_active += 1
        return num_active

    # iterate though the list of processes and their IO completion time
    # if the IO time is > than current time, then that process is still in IO
    # count that process
    def get_ios_in_flight(self, current_time):
        num_in_flight = 0
        for pid in range(len(self.proc_info)):
            for t in self.io_finish_times[pid]: # where is io_finish_times defined?
                if t > current_time:
                    num_in_flight += 1
        return num_in_flight

    # this is not yet implemented, not even sure what, and why this is
    def check_for_switch(self):
        return

    # not sure yet what this is, but I'm sure this has some significance
    def space(self, num_columns):
        for i in range(num_columns):
            print("%10s" % " ", end="")

    # check if a process is done
    # if the list of PROC_CODE is empty
    # move the process to done
    # and, switch to the next proc
    def check_if_done(self):
        if len(self.proc_info[self.curr_proc][PROC_CODE]) == 0:
            if self.proc_info[self.curr_proc][PROC_STATE] == STATE_RUNNING:
                self.move_to_done(STATE_RUNNING)
                self.next_proc()
        return

    # the scheduler runner
    def run(self):
        # clock tick counter from 0
        clock_tick = 0

        # if there is no proc, stop the scheduler
        if len(self.proc_info) == 0:
            return

        # track outstanding IOs, per process
        # dict containing the io_finish_times
        # in k,v pairs of pid: list()
        # the finish times are in clock_tick(s)
        self.io_finish_times = {}
        for pid in range(len(self.proc_info)):
            self.io_finish_times[pid] = []

        # make first one active
        self.curr_proc = 0
        self.move_to_running(STATE_READY)

        # OUTPUT: headers for each column
        # column based output, similar to how it's in UNIX CLI tools O/P
        print("%s" % "Time", end="")
        for pid in range(len(self.proc_info)):
            print("%14s" % ("PID:%2d" % (pid)), end="")
        print("%14s" % "CPU", end="")
        print("%14s" % "IOs", end="")
        print("")

        # init statistics
        io_busy = 0
        cpu_busy = 0

        while self.get_num_active() > 0:
            # every loop run is one clock cycle
            clock_tick += 1

            # check for io finish
            io_done = False
            # iterate over all processes
            for pid in range(len(self.proc_info)):
                # for a process, if the clock_tick is in the list of io_finish_times
                # that means, that one IO is done
                if clock_tick in self.io_finish_times[pid]:
                    # flag io_done
                    io_done = True
                    # move process to ready, since IO's done
                    self.move_to_ready(STATE_WAIT, pid)
                    # depending on the IO behaviour, do it IMMEDIATELY
                    # or do it after a while
                    if self.io_done_behavior == IO_RUN_IMMEDIATE:
                        # IO_RUN_IMMEDIATE
                        # if the curr_proc is not the pid
                        if self.curr_proc != pid:
                            # check if the pid-process is in STATE_RUNNING
                            if (
                                self.proc_info[self.curr_proc][PROC_STATE]
                                == STATE_RUNNING
                            ):
                                # and move it do STATE_READY
                                self.move_to_ready(STATE_RUNNING)
                        # schedule to the next process
                        self.next_proc(pid)
                    else:
                        # IO_RUN_LATER
                        if (
                            self.process_switch_behavior == SCHED_SWITCH_ON_END
                            and self.get_num_runnable() > 1
                        ):
                            # this means the process that issued the io should be run
                            self.next_proc(pid)
                        if self.get_num_runnable() == 1:
                            # this is the only thing to run: so run it
                            self.next_proc(pid)
                    self.check_if_done()

            # if current proc is RUNNING and has an instruction, execute it
            instruction_to_execute = ""
            if (
                self.proc_info[self.curr_proc][PROC_STATE] == STATE_RUNNING
                and len(self.proc_info[self.curr_proc][PROC_CODE]) > 0
            ):
                instruction_to_execute = self.proc_info[self.curr_proc][PROC_CODE].pop(
                    0
                )
                cpu_busy += 1

            # OUTPUT: print what everyone is up to
            if io_done:
                print("%3d*" % clock_tick, end="")
            else:
                print("%3d " % clock_tick, end="")
            for pid in range(len(self.proc_info)):
                if pid == self.curr_proc and instruction_to_execute != "":
                    print("%14s" % ("RUN:" + instruction_to_execute), end="")
                else:
                    print("%14s" % (self.proc_info[pid][PROC_STATE]), end="")

            # CPU output here: if no instruction executes, output a space, otherwise a 1
            if instruction_to_execute == "":
                print("%14s" % " ", end="")
            else:
                print("%14s" % "1", end="")

            # IO output here:
            num_outstanding = self.get_ios_in_flight(clock_tick)
            if num_outstanding > 0:
                print("%14s" % str(num_outstanding), end="")
                io_busy += 1
            else:
                print("%10s" % " ", end="")
            print("")

            # if this is an IO start instruction, switch to waiting state
            # and add an io completion in the future
            if instruction_to_execute == DO_IO:
                self.move_to_wait(STATE_RUNNING)
                self.io_finish_times[self.curr_proc].append(
                    clock_tick + self.io_length + 1
                )
                if self.process_switch_behavior == SCHED_SWITCH_ON_IO:
                    self.next_proc()

            # ENDCASE: check if currently running thing is out of instructions
            self.check_if_done()
        return (cpu_busy, io_busy, clock_tick)


#
# PARSE ARGUMENTS
#

parser = OptionParser()
parser.add_option(
    "-s",
    "--seed",
    default=0,
    help="the random seed",
    action="store",
    type="int",
    dest="seed",
)
parser.add_option(
    "-P",
    "--program",
    default="",
    help="more specific controls over programs",
    action="store",
    type="string",
    dest="program",
)
parser.add_option(
    "-l",
    "--processlist",
    default="",
    help="a comma-separated list of processes to run, in the form X1:Y1,X2:Y2,... where X is the number of instructions that process should run, and Y the chances (from 0 to 100) that an instruction will use the CPU or issue an IO (i.e., if Y is 100, a process will ONLY use the CPU and issue no I/Os; if Y is 0, a process will only issue I/Os)",
    action="store",
    type="string",
    dest="process_list",
)
parser.add_option(
    "-L",
    "--iolength",
    default=5,
    help="how long an IO takes",
    action="store",
    type="int",
    dest="io_length",
)
parser.add_option(
    "-S",
    "--switch",
    default="SWITCH_ON_IO",
    help="when to switch between processes: SWITCH_ON_IO, SWITCH_ON_END",
    action="store",
    type="string",
    dest="process_switch_behavior",
)
parser.add_option(
    "-I",
    "--iodone",
    default="IO_RUN_LATER",
    help="type of behavior when IO ends: IO_RUN_LATER, IO_RUN_IMMEDIATE",
    action="store",
    type="string",
    dest="io_done_behavior",
)
parser.add_option(
    "-c",
    help="compute answers for me",
    action="store_true",
    default=False,
    dest="solve",
)
parser.add_option(
    "-p",
    "--printstats",
    help="print statistics at end; only useful with -c flag (otherwise stats are not printed)",
    action="store_true",
    default=False,
    dest="print_stats",
)
(options, args) = parser.parse_args()

random_seed(options.seed)

assert (
    options.process_switch_behavior == SCHED_SWITCH_ON_IO
    or options.process_switch_behavior == SCHED_SWITCH_ON_END
)
assert (
    options.io_done_behavior == IO_RUN_IMMEDIATE
    or options.io_done_behavior == IO_RUN_LATER
)

s = scheduler(
    options.process_switch_behavior, options.io_done_behavior, options.io_length
)

if options.program != "":
    for p in options.program.split(":"):
        s.load_program(p)
else:
    # example process description (10:100,10:100)
    for p in options.process_list.split(","):
        s.load(p)

assert options.io_length >= 0

if options.solve == False:
    print("Produce a trace of what would happen when you run these processes:")
    for pid in range(s.get_num_processes()):
        print("Process %d" % pid)
        for inst in range(s.get_num_instructions(pid)):
            print("  %s" % s.get_instruction(pid, inst))
        print("")
    print("Important behaviors:")
    print("  System will switch when ", end="")
    if options.process_switch_behavior == SCHED_SWITCH_ON_IO:
        print("the current process is FINISHED or ISSUES AN IO")
    else:
        print("the current process is FINISHED")
    print("  After IOs, the process issuing the IO will ", end="")
    if options.io_done_behavior == IO_RUN_IMMEDIATE:
        print("run IMMEDIATELY")
    else:
        print("run LATER (when it is its turn)")
    print("")
    exit(0)

(cpu_busy, io_busy, clock_tick) = s.run()

if options.print_stats:
    print("")
    print("Stats: Total Time %d" % clock_tick)
    print(
        "Stats: CPU Busy %d (%.2f%%)" % (cpu_busy, 100.0 * float(cpu_busy) / clock_tick)
    )
    print(
        "Stats: IO Busy  %d (%.2f%%)" % (io_busy, 100.0 * float(io_busy) / clock_tick)
    )
    print("")
