TOPTARGETS:= all clean
SUBDIRS := 01-Context-Creation/. 02-Drawing/.

TOPTARGEST: $(SUBDIRS)
$(SUBDIRS):
	echo $(MAKECMDGOALS) && $(MAKE) -C $@

.PHONY: $(TOPTARGETS) $(SUBDIRS)
