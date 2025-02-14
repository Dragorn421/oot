all:
	$(MAKE) -C bin2c
	$(MAKE) -C png2raw
# must build n64texconv before build_from_png
	$(MAKE) -C n64texconv
	$(MAKE) -C build_from_png

clean:
	$(MAKE) -C bin2c clean
	$(MAKE) -C png2raw clean
	$(MAKE) -C n64texconv clean
	$(MAKE) -C build_from_png clean

distclean: clean
	$(MAKE) -C n64texconv distclean

.PHONY: all clean distclean
