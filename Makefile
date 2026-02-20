CXX = g++
LD = g++
CXXFLAGS = -std=c++23 -O2 -fsanitize=address,undefined

TASKS = $(wildcard task_*)

define TASK_rule
exec_$(1) := T_$(patsubst task_%,%,$(1))
sources_$(1) := $(wildcard $(1)/*.cpp)
headers_$(1) := $(wildcard $(1)/*.hpp)

$$(exec_$(1)): $$(sources_$(1)) $$(headers_$(1))
	$$(CXX) $$(sources_$(1)) $$(CXXFLAGS) -o $$@
endef

$(foreach task,$(TASKS),$(eval $(call TASK_rule,$(task))))

EXECS = $(foreach task,$(TASKS),T_$(patsubst task_%,%,$(task)))

all: $(EXECS)

NUMBERS = $(patsubst task_%,%,$(TASKS))
.PHONY: $(NUMBERS)
$(NUMBERS): %: T_%
	@#

clean:
	rm -f $(EXECS)

.PHONY: all clean