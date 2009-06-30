#
# Copyright (c) 2008 Google Inc. All rights reserved.
#
#
.PHONY: x
PYFILES = \
  x86-64.gr.py \
  x86-64.float.py \
  x86-64.int.py \
  x86-64.misc.py \
  x86-64.string.py \
  x86-64.asm.py \
#

VICTIM = x86-64.int.py

.PHONY: all shard
all: $(PYFILES:%.py=%.pyout) Makefile

%.pyout: %.py plug.py x86-64.gr.py Makefile
	python $< > $@

.PHONY: shard
shard: $(VICTIM) shardgrammar
	./shardgrammar $<

shardgrammar: shardgrammar.cc Makefile
	g++ -g -Wall -o $@ $<

.PHONY: clean
clean:
	rm -rf shardgrammar tmp/ *.pyout shards/
