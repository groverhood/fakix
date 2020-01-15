include ./Make.defaults

define \n


endef
MODULES := boot kernel

all: $(MODULES)
	$(foreach mod, $(MODULES), $(MAKE) -C $(mod)${\n})

clean:
	find $(MODULES) \( -name "*.o" -o -name "*.generated.*" -o -name "*.bin" \) -a -delete