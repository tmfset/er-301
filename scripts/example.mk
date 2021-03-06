include scripts/env.mk

MODNAME := example
src_dir = $(mods_dir)/$(MODNAME)
asset_dir = $(src_dir)/assets

includes += . $(mods_dir) $(arch_dir) $(lua_dir)

include scripts/mod-builder.mk
